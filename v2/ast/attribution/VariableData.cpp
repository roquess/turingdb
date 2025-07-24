#include "VariableData.h"

using namespace db;

VariableData::VariableData() = default;

VariableData::~VariableData() = default;

VariableData::VariableData(VariableType type)
    : _type(type)
{
}


