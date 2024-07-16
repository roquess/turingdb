#include "vmutils.h"

#include "DB.h"
#include "DBAccess.h"
#include "LogSetup.h"
#include "LogUtils.h"
#include "Neo4j/ParserConfig.h"
#include "Neo4jImporter.h"
#include "PerfStat.h"
#include "Time.h"

VMSample VMSample::createSample(const std::string& sampleName) {
    LogSetup::setupLogFileBacked(sampleName + ".log");
    PerfStat::init(sampleName + ".perf");
    spdlog::set_level(spdlog::level::info);
    const std::string turingHome = std::getenv("TURING_HOME");

    auto jobSystem = std::make_unique<JobSystem>();
    auto system = std::make_unique<db::SystemManager>();
    auto assembler = std::make_unique<db::Assembler>();
    auto vm = std::make_unique<db::VM>(system.get());
    auto program = std::make_unique<db::Program>();

    jobSystem->initialize();

    spdlog::info("== Init VM ==");
    auto t0 = Clock::now();
    vm->initialize();
    logt::ElapsedTime(Microseconds(Clock::now() - t0).count(), "us");

    return VMSample {
        ._turingHome = turingHome,
        ._sampleDir = turingHome + "/samples/" + sampleName,
        ._jobSystem = std::move(jobSystem),
        ._system = std::move(system),
        ._assembler = std::move(assembler),
        ._vm = std::move(vm),
        ._program = std::move(program),
    };
}

void VMSample::destroy() {
    PerfStat::destroy();
}

bool VMSample::loadJsonDB(std::string_view jsonDir) const {
    spdlog::info("== Loading DB ==");
    spdlog::info("Path: {}", jsonDir);
    auto t0 = Clock::now();

    const bool res = db::Neo4jImporter::importJsonDir(
        *_jobSystem,
        _system->getDefaultDB(),
        db::nodeCountLimit,
        db::edgeCountLimit,
        db::Neo4jImporter::ImportJsonDirArgs {
            ._jsonDir = jsonDir,
        });

    logt::ElapsedTime(Milliseconds(Clock::now() - t0).count(), "ms");
    if (!res) {
        spdlog::error("Could not load db");
    }

    return res;
}

bool VMSample::generateFromFile(std::string_view programPath) const {
    spdlog::info("== Code generation ==");
    spdlog::info("Path: {}", programPath);
    auto t0 = Clock::now();

    const bool res = _assembler->generateFromFile(*_program, programPath);

    logt::ElapsedTime(Milliseconds(Clock::now() - t0).count(), "ms");
    if (!res) {
        spdlog::error("Could generate byte code");
    }

    return res;
}

void VMSample::execute() const {
    spdlog::info("== Execution ==");
    auto t0 = Clock::now();

    _vm->exec(_program.get());
    logt::ElapsedTime(Milliseconds(Clock::now() - t0).count(), "ms");
}

void VMSample::printOutput(std::initializer_list<std::string_view> colNames,
                           uint8_t outRegister,
                           size_t maxLineCount,
                           size_t colSize) const {

    const auto& out = getOutput(outRegister);

    if (out.empty()) {
        spdlog::error("Output is empty");
        return;
    }

    std::string str;
    for (const auto& name : colNames) {
        str += fmt::format("{1:>{0}}", colSize, name);
    }

    str += "\n";
    for (size_t i = 0; i < out[0].size(); i++) {
        for (size_t j = 0; j < out.size(); j++) {
            if (out[j][i] > 5000000) {
                str += fmt::format("{1:>{0}}", colSize, "...");
            } else {
                str += fmt::format("{1:>{0}}", colSize, out[j][i]);
            }
        }
        str += '\n';
        if (i == maxLineCount) {
            str += "...\n";
            break;
        }
    }
    spdlog::info("\n{}", str);
    spdlog::info("NLines in output: {}", out[0].size());
}

db::EntityID VMSample::findNode(const std::string& ptName, const std::string& prop) const {
    spdlog::info("== Finding node ==");
    spdlog::info("Prop: [{}]: {}", ptName, prop);
    auto t0 = Clock::now();

    auto* db = _system->getDefaultDB();
    const auto access = db->access();
    const auto& propTypes = db->getMetadata()->propTypes();
    db::PropertyType pt = propTypes.get(ptName);

    if (!pt._id.isValid()) {
        spdlog::error("Property type {} does not exist", ptName);
        return {};
    }

    const auto nhs_nos = access.scanNodeProperties<db::types::String>(pt._id);
    auto it = nhs_nos.begin();
    for (; it.isValid(); ++it) {
        const auto& v = it.get();
        if (v == prop) {
            logt::ElapsedTime(Milliseconds(Clock::now() - t0).count(), "ms");
            return it.getCurrentNodeID();
        }
    }

    spdlog::error("Could not find node with prop {} of propType {}",
                  prop, ptName);
    return {};
}

const db::OutputWriter::Output& VMSample::getOutput(uint8_t reg) const {
    return _vm->readRegister<db::OutputWriter>(reg)->getResult();
}
