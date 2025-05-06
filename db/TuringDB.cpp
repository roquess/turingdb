#include "TuringDB.h"

#include "QueryInterpreter.h"
#include "JobSystem.h"

using namespace db;

TuringDB::TuringDB()
    : _jobSystem(JobSystem::create())
{
}

TuringDB::~TuringDB()
{
}

QueryStatus TuringDB::query(std::string_view query,
                            std::string_view graphName,
                            LocalMemory* mem,
                            QueryCallback callback,
                            CommitHash hash) {
    QueryInterpreter interp(&_systemManager, _jobSystem.get());
    return interp.execute(query, graphName, mem, callback, hash);
}

QueryStatus TuringDB::query(std::string_view query,
                            std::string_view graphName,
                            LocalMemory* mem,
                            CommitHash hash) {
    QueryInterpreter interp(&_systemManager, _jobSystem.get());
    return interp.execute(query, graphName, mem, [](const auto&){}, hash);
}
