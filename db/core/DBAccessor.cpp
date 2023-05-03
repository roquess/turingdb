#include "DBAccessor.h"

using namespace db;

DBAccessor::DBAccessor(const DB* db)
    : _db(db)
{
}

DBAccessor::~DBAccessor() {
}
