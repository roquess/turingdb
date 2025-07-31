#include "spdlog/spdlog.h"

#include "TuringDB.h"
#include "SystemManager.h"
#include "LocalMemory.h"
#include "Graph.h"
#include "Path.h"
#include "QueryInterpreter.h"
#include "GraphDumper.h"
#include "GraphLoader.h"
#include "SimpleGraph.h"
#include "versioning/Transaction.h"

using namespace db;


int main() {
    // Load and dump SimpleGraph to ensure there is no string index
    TuringDB _db;
    LocalMemory _mem;
    SystemManager& sysMan = _db.getSystemManager();
    Graph* _builtGraph = sysMan.createGraph("simple");

    auto _interp =
        std::make_unique<QueryInterpreter>(&_db.getSystemManager(), &_db.getJobSystem());

    std::unique_ptr<Graph> _loadedGraph;
    fs::Path _workingPath = fs::Path{"out/dumps"};
    
    GraphDumper dumper;
    auto res = dumper.dump(*_builtGraph, _workingPath);
    if (!res) {
        spdlog::error("Failed to dump graph:\n" + res.error().fmtMessage());
        return EXIT_FAILURE;
    }

    _loadedGraph = Graph::createEmptyGraph();
    const auto loadRes = GraphLoader::load(_loadedGraph.get(), _workingPath);
    if (!loadRes) {
        spdlog::error("Failed to dump graph:\n" + res.error().fmtMessage());
        return EXIT_FAILURE;
    }

    // Query the graph and assert there was an exception
    std::string query = "MATCH (n{name~=\"Remy\"}) return n.name";
}
