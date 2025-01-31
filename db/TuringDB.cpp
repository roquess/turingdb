#include "TuringDB.h"

using namespace db;

TuringDB::TuringDB()
    : _interp(&_systemManager)
{
}

TuringDB::~TuringDB()
{
}

QueryStatus TuringDB::query(std::string_view query,
                             std::string_view graphName,
                             LocalMemory* mem,
                             QueryCallback callback) {
    return _interp.execute(query, graphName, mem, callback);
}

QueryStatus TuringDB::query(std::string_view query,
                             std::string_view graphName,
                             LocalMemory* mem) {
    return _interp.execute(query, graphName, mem, [](const auto&){});
}
