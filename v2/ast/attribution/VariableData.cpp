#include "VariableData.h"

using namespace db;

VariableData::VariableData()
    : _data(std::monostate {})
{
}

VariableData::~VariableData() = default;


VariableData::VariableData(Variant&& data)
    : _data(std::move(data))
{
}


