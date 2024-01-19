#include "DBObject.h"

using namespace db;

DBObject::DBObject(DBIndex index)
    : _index(index)
{
}

DBObject::~DBObject() {
}
