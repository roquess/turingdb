#include "BioLog.h"
#include "DB.h"
#include "FileUtils.h"
#include "JobSystem.h"
#include "MsgCommon.h"
#include "MsgImport.h"
#include "Neo4jImporter.h"
#include "PerfStat.h"
#include "ToolInit.h"

using namespace Log;
using namespace db;

enum class ImportType {
    NEO4J,
    NEO4J_URL,
    JSON_NEO4J,
    // GML,
    // CSV,
};

struct ImportData {
    ImportType type;
    std::string path;
    std::string networkName;
    std::string primaryKey;
    std::string url;
    std::string urlSuffix;
    uint64_t port = 0;
    std::string username;
    std::string password;
};

using Clock = std::chrono::high_resolution_clock;

template <typename T>
float duration(auto& t0, auto& t1) {
    return std::chrono::duration<float, T>(t1 - t0).count();
}

static constexpr size_t edgeCountPerQuery = 100000;
static constexpr size_t nodeCountPerQuery = 100000;

int main(int argc, const char** argv) {
    ToolInit toolInit("bioimport2");

    ArgParser& argParser = toolInit.getArgParser();

    argParser.addOption(
        "neo4j",
        "Imports a .dump file (default network name: \"my_file\")",
        "my_file.dump");

    argParser.addOption(
        "json-neo4j",
        "Imports json files from a json/ directory (default network name: \"my_json_dir\")",
        "my_json_dir");

    toolInit.init(argc, argv);

    auto db = std::make_unique<DB>();
    JobSystem jobSystem;
    jobSystem.initialize();

    auto t0 = Clock::now();

    std::vector<ImportData> importData;

    for (const auto& option : argParser.options()) {
        const auto& optName = option.first;
        if (optName == "neo4j") {
            if (!FileUtils::exists(option.second)) {
                BioLog::log(msg::ERROR_DIRECTORY_NOT_EXISTS()
                            << option.second);
                return EXIT_FAILURE;
            }

            importData.emplace_back(ImportData {
                .type = ImportType::NEO4J,
                .path = option.second,
            });
        } else if (optName == "json-neo4j") {
            if (!FileUtils::exists(option.second)) {
                BioLog::log(msg::ERROR_DIRECTORY_NOT_EXISTS()
                            << option.second);
                return EXIT_FAILURE;
            }
            importData.emplace_back(ImportData {
                .type = ImportType::JSON_NEO4J,
                .path = option.second,
            });
        }
    }

    const bool noPathsGiven = importData.empty();

    if (noPathsGiven) {
        BioLog::log(msg::ERROR_IMPORT_NO_PATH_GIVEN());
        argParser.printHelp();
        return EXIT_SUCCESS;
    }

    for (const auto& data : importData) {

        switch (data.type) {
            case ImportType::NEO4J_URL: {
                if (!Neo4jImporter::import(jobSystem,
                                           db.get(),
                                           data.path,
                                           nodeCountPerQuery,
                                           edgeCountPerQuery)) {
                    BioLog::echo("Something went wrong during neo4j import");
                    return 1;
                }
                break;
            }
            case ImportType::JSON_NEO4J: {
                // neo4jImport.importJsonNeo4j(data.path, networkName);
                break;
            }
        }
    }

    auto t1 = Clock::now();
    float dur = duration<std::ratio<1, 1>>(t0, t1);
    BioLog::echo("Elapsed time: " + std::to_string(dur) + " s");

    return EXIT_SUCCESS;
}
