#include "PipeUtils.h"

#include <stdlib.h>
#include <spdlog/spdlog.h>

#include "SystemManager.h"
#include "JobSystem.h"
#include "Neo4j/ParserConfig.h"
#include "Neo4jImporter.h"
#include "PlannerContext.h"
#include "QueryPlanner.h"
#include "MemoryManager.h"

#include "Time.h"
#include "LogUtils.h"
#include "LogSetup.h"
#include "PerfStat.h"
#include "Panic.h"

using namespace db;

PipeSample::PipeSample(const std::string& sampleName)
    : _sampleName(sampleName)
{
    LogSetup::setupLogFileBacked(sampleName + ".log");
    PerfStat::init(sampleName + ".perf");
    spdlog::set_level(spdlog::level::info);
    _system = std::make_unique<db::SystemManager>();
    _jobSystem = std::make_unique<JobSystem>();
    _jobSystem->initialize();
}

PipeSample::~PipeSample() {
    PerfStat::destroy();
}

std::string PipeSample::getTuringHome() const {
    const char* envStr = std::getenv("TURING_HOME");
    if (!envStr) {
        panic("Environment variable TURING_HOME not found");
    }

    return envStr;
}

bool PipeSample::loadJsonDB(const std::string& jsonDir) {
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
        }
    );

    logt::ElapsedTime(Milliseconds(Clock::now() - t0).count(), "ms");
    if (!res) {
        spdlog::error("Could not load db");
    }

    return res;
}

bool PipeSample::compileQuery(const std::string& queryStr) {
    _mem = std::make_unique<MemoryManager>();
    _planCtxt = std::make_unique<PlannerContext>(_system->getDefaultDB(), _mem.get());
    _queryPlanner = std::make_unique<QueryPlanner>(_ctxt.get(), _query);
    return false;
}

bool PipeSample::execute() {
    return false;
}