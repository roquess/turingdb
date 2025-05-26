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
                            CommitHash commit,
                            ChangeID change) {
    QueryInterpreter interp(&_systemManager, _jobSystem.get());
    return interp.execute(query, graphName, mem, callback, commit, change);
}

QueryStatus TuringDB::query(std::string_view query,
                            std::string_view graphName,
                            LocalMemory* mem,
                            CommitHash commit,
                            ChangeID change) {
    QueryInterpreter interp(&_systemManager, _jobSystem.get());
    return interp.execute(query, graphName, mem, [](const auto&){}, commit, change);
}
