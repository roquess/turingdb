#pragma once

#include <vector>

#include "attribution/DeclID.h"
#include "attribution/VariableType.h"
#include "attribution/VariableData.h"

namespace db {

class DeclContainer {
public:
    DeclContainer();
    ~DeclContainer();

    DeclContainer(const DeclContainer&) = delete;
    DeclContainer& operator=(const DeclContainer&) = delete;
    DeclContainer(DeclContainer&&) = delete;
    DeclContainer& operator=(DeclContainer&&) = delete;

    DeclID newVariable(VariableType type) {
        auto id = _types.size();
        _types.push_back(type);
        _data.emplace_back(std::monostate {});

        return id;
    }

    const VariableType& getType(DeclID id) const {
        return _types[id.value()];
    }

    const VariableData& getData(DeclID id) const {
        return _data[id.value()];
    }

    VariableData& getData(DeclID id) {
        return _data[id.value()];
    }

    void setData(DeclID id, VariableData&& data) {
        _data[id.value()] = std::move(data);
    }

private:
    std::vector<VariableType> _types;
    std::vector<VariableData> _data;
};

}
