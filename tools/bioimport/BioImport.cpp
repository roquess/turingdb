#include "MsgCommon.h"
#include "MsgImport.h"
#include "Neo4JHttpRequest.h"
#include "Neo4JSchemaExtract.h"
#include "Node.h"
#include "NodeType.h"
#include "Property.h"
#include "TimerStat.h"
#include "ToolInit.h"

#include "BioLog.h"
#include "DB.h"
#include "FileUtils.h"
#include "Neo4JInstance.h"
#include "import/json/JsonParser.h"
#include <optional>
#include <string>

#define BIOIMPORT_TOOL_NAME "bioimport"

using namespace Log;

bool importNeo4j(const ToolInit& tool, const FileUtils::Path& filepath) {
    BioLog::echo("Loading Neo4j dump file: " + filepath.string());
    // Neo4JInstance instance;
    // instance.setup();
    // instance.importDumpedDB(filepath);
    // instance.start();

    FileUtils::Path jsonDir{tool.getOutputsDir() / "json"};

    FileUtils::Path jsonPath{tool.getOutputsDir() / jsonDir / "nodes.json"};

    if (!FileUtils::exists(jsonDir)) {
        if (!FileUtils::createDirectory(jsonDir)) {
            BioLog::log(msg::ERROR_FAILED_TO_CREATE_DIRECTORY() << jsonDir);
            return false;
        }
    }

    // Neo4JHttpRequest req(jsonPath);
    // req.setStatement("MATCH (n) RETURN n, labels(n);");

    // if (!req.exec()) {
    //     BioLog::log(msg::ERROR_IMPORT_FAILED_BAD_CURL_REQUEST()
    //                 << "MATCH (n) RETURN n, labels(n);");
    //     return false;
    // }

    JsonParser parser{};

    {
        TimerStat timer{"JsonParser: parsing properties.json"};
        if (!parser.parse("/home/dev/properties.json", JsonParser::Format::Neo4j4_Properties)) {
            BioLog::log(msg::ERROR_IMPORT_FAILED_BAD_JSON() << jsonPath);
            return false;
        }
    }

    //{
    //    TimerStat timer{"JsonParser: parsing nodes"};
    //    if (!parser.parse("/home/dev/nodes.json", JsonParser::Format::Neo4j4_Nodes)) {
    //        BioLog::log(msg::ERROR_IMPORT_FAILED_BAD_JSON() << jsonPath);
    //        return false;
    //    }
    //}

    //{
    //    TimerStat timer{"JsonParser: parsing edges"};
    //    if (!parser.parse("/home/dev/edges.json", JsonParser::Format::Neo4j4_Edges)) {
    //        BioLog::log(msg::ERROR_IMPORT_FAILED_BAD_JSON() << jsonPath);
    //        return false;
    //    }
    //}

    db::DB* db{parser.getDB()};
    // db::DBAccessor accessor{db};
    for (db::NodeType* nt : db->nodeTypes()) {
        BioLog::echo("NodeType: " +
                     nt->getName().getSharedString()->getString());
        //    db::ComponentType* compType = nt->getBaseComponent();

        //    for (db::Property* p : compType->getProperties()) {
        //        const auto& pName =
        //        p->getName().getSharedString()->getString(); BioLog::echo("  "
        //        + pName);
        //    }
    }

    delete db;

    // instance.destroy();
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
    return EXIT_SUCCESS;
}
