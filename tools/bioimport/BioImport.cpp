#include "MsgCommon.h"
#include "MsgImport.h"
#include "Neo4JHttpRequest.h"
#include "Neo4JSchemaExtract.h"
#include "Network.h"
#include "Node.h"
#include "NodeType.h"
#include "PerfStat.h"
#include "Property.h"
#include "PropertyType.h"
#include "TimerStat.h"
#include "ToolInit.h"

#include "BioLog.h"
#include "DB.h"
#include "FileUtils.h"
#include "Neo4JInstance.h"
#include "import/json/JsonParser.h"
#include <iostream>
#include <optional>
#include <string>

#define BIOIMPORT_TOOL_NAME "bioimport"

using namespace Log;

bool importNeo4j(const ToolInit& tool, const FileUtils::Path& filepath) {
    BioLog::echo("Loading Neo4j dump file: " + filepath.string());
    TimerStat timer{"Neo4j: import"};
    Neo4JInstance instance;
    instance.setup();
    instance.importDumpedDB(filepath);
    instance.start();
    const FileUtils::Path& outDir = tool.getOutputsDir();

    FileUtils::Path jsonDir = outDir / "json";
    FileUtils::Path nodePropertiesFile = jsonDir / "nodeProperties.json";
    FileUtils::Path nodesFile = jsonDir / "nodes.json";
    FileUtils::Path edgePropertiesFile = jsonDir / "edgeProperties.json";
    FileUtils::Path edgesFile = jsonDir / "edges.json";

    if (!FileUtils::exists(jsonDir)) {
        if (!FileUtils::createDirectory(jsonDir)) {
            BioLog::log(msg::ERROR_FAILED_TO_CREATE_DIRECTORY() << jsonDir);
            return false;
        }
    }

    JsonParser parser{};
    Neo4JHttpRequest req({});

    // NODE TYPE PROPERTIES
    {
        std::string statement = "CALL apoc.meta.nodeTypeProperties();";
        TimerStat timer{"Neo4j: requesting nodeProperties"};
        req.setStatement(statement);

        if (!req.exec()) {
            instance.destroy();
            BioLog::log(msg::ERROR_IMPORT_FAILED_BAD_CURL_REQUEST()
                        << statement);
            return false;
        }
    }
    req.writeToFile(nodePropertiesFile);

    {
        TimerStat timer{"JsonParser: parsing nodeProperties"};
        if (!parser.parse(req.getData(),
                          JsonParser::Format::Neo4j4_NodeProperties)) {
            instance.destroy();
            // Error
            return false;
        }
    }

    // NODES
    {
        std::string statement = "MATCH(n) RETURN labels(n), ID(n), properties(n);";
        TimerStat timer{"Neo4j: requesting nodes"};
        req.setStatement(statement);

        if (!req.exec()) {
            instance.destroy();
            BioLog::log(msg::ERROR_IMPORT_FAILED_BAD_CURL_REQUEST()
                        << statement);
            return false;
        }
    }
    req.writeToFile(nodesFile);

    {
        TimerStat timer{"JsonParser: parsing nodes"};
        if (!parser.parse(req.getData(), JsonParser::Format::Neo4j4_Nodes)) {
            instance.destroy();
            // Error
            return false;
        }
    }

    // EDGE TYPE PROPERTIES
    {
        std::string statement = "CALL apoc.meta.relTypeProperties();";
        TimerStat timer{"Neo4j: requesting edge properties"};
        req.setStatement(statement);

        if (!req.exec()) {
            instance.destroy();
            BioLog::log(msg::ERROR_IMPORT_FAILED_BAD_CURL_REQUEST()
                        << statement);
            return false;
        }
    }
    req.writeToFile(edgePropertiesFile);

    {
        TimerStat timer{"JsonParser: parsing edge properties"};
        if (!parser.parse(req.getData(), JsonParser::Format::Neo4j4_EdgeProperties)) {
            instance.destroy();
            // Error
            return false;
        }
    }

    // EDGES
    {
        std::string statement = "MATCH (n1)-[e]-(n2) RETURN type(e), "
                                "ID(n1), ID(n2), properties(e)";
        TimerStat timer{"Neo4j: requesting edges"};
        req.setStatement(statement);

        if (!req.exec()) {
            instance.destroy();
            BioLog::log(msg::ERROR_IMPORT_FAILED_BAD_CURL_REQUEST()
                        << statement);
            return false;
        }
    }
    req.writeToFile(edgesFile);

    {
        TimerStat timer{"JsonParser: parsing edges"};
        if (!parser.parse(req.getData(), JsonParser::Format::Neo4j4_Edges)) {
            // Error
            return false;
        }
    }

    instance.destroy();
    std::cout << "press any key to continue";
    std::cin.get();
    return true;
}

int main(int argc, const char** argv) {
    ToolInit toolInit(BIOIMPORT_TOOL_NAME);

    ArgParser& argParser = toolInit.getArgParser();
    argParser.addOption("neo4j", "imports a neo4j.dump file", true);

    toolInit.init(argc, argv);

    std::optional<std::string> neo4j_file;

    for (const auto& option : argParser.options()) {
        const auto& optName = option.first;
        if (optName == "neo4j") {
            neo4j_file = option.second;
        }
    }

    if (neo4j_file) {
        importNeo4j(toolInit, neo4j_file.value());
    }

    BioLog::printSummary();
    BioLog::destroy();
    PerfStat::destroy();
    return EXIT_SUCCESS;
}
