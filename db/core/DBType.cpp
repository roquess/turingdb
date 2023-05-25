#include "DBType.h"

using namespace db;

DBType::DBType(DBIndex index, StringRef name)
    : DBObject(index),
    _name(name)
{
}

DBType::~DBType() {
}
