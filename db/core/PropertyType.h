#pragma once

#include "DBType.h"

namespace db {

class ValueType;
class Writeback;

class PropertyType : public DBType {
public:
    friend Writeback;

    ValueType* getValueType() const { return _valType; }

private:
    DBObject* _owner {nullptr};
    ValueType* _valType {nullptr};

    PropertyType(DBIndex index,
                 DBObject* owner,
                 StringRef name,
                 ValueType* valType);
    ~PropertyType();
};

}
