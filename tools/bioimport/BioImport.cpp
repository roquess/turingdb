#include "ToolInit.h"

#include <string>
#include <iostream>

#include "Neo4JInstance.h"
#include "Neo4JHttpRequest.h"
#include "Neo4JSchemaExtract.h"

#include "BioLog.h"

#define BIOIMPORT_TOOL_NAME     "import"

using namespace Log;

void neo4jImport(const std::filesystem::path& neo4jDir) {
    Neo4JInstance neo4j(neo4jDir);
    
    if (!neo4j.setup()) {
        BioLog::printSummary();
        BioLog::destroy();
        return;
    }

    const std::string dbPath = "/turing/data/reactome/graph.db";
    if (!neo4j.importDBDir(dbPath)) {
        BioLog::printSummary();
        BioLog::destroy();
        return;
    }

    if (!neo4j.start()) {
        BioLog::printSummary();
        BioLog::destroy();
        return;
    }

    if (!neo4j.changePassword("neo4j", "roger")) {
        BioLog::echo("failed to change initial password");
        BioLog::printSummary();
        BioLog::destroy();
        return;
    }

    /*
    Neo4JHttpRequest req(neo4jDir/"nodes.json");
    req.setHost("localhost");
    req.setPort(7474);
    req.setUser("neo4j");
    req.setPassword("roger");
    req.setStatement("MATCH (n) RETURN n;");
    */

    /*
    Neo4JHttpRequest req(neo4jDir/"relations.json");
    req.setHost("localhost");
    req.setPort(7474);
    req.setUser("neo4j");
    req.setPassword("roger");
    req.setStatement("MATCH (n)-[r]-(m) RETURN n.dbId,n.schemaClass,type(r),m.dbId,m.schemaClass;");
    */

    Neo4JHttpRequest req(neo4jDir/"types.json");
    req.setHost("localhost");
    req.setPort(7474);
    req.setUser("neo4j");
    req.setPassword("roger");
    req.setStatement("MATCH (n) RETURN DISTINCT labels(n);");

    req.exec();

    neo4j.stop();
}

void extract() {
    Neo4JSchemaExtract extract("nodes.json", "relations.json");
    const bool res = extract.extractRelations();
    if (!res) {
        return;
    }

    std::string destStr;
    for (const auto& entry : extract.getRelations()) {
        BioLog::echo("===== " + entry.first + " =======");
        for (const auto& relEntry : entry.second) {
            destStr.clear();
            for (const auto& dest : relEntry.second) {
                destStr += " ";
                destStr += dest;
            }
            BioLog::echo(relEntry.first + ":" + destStr);
        }
        BioLog::echo("==========\n");
    }
}

int main(int argc, char** argv) {
    ToolInit toolInit(BIOIMPORT_TOOL_NAME);
    toolInit.init();

    neo4jImport(toolInit.getOutputsDir());
    //extract();

    BioLog::printSummary();
    BioLog::destroy();
    return EXIT_SUCCESS;
}
