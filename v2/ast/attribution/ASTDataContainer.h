#pragma once

#include <vector>

#include "attribution/ASTNodeID.h"
#include "attribution/VariableType.h"
#include "attribution/VariableData.h"

namespace db {

class ASTDataContainer {
public:
    ASTDataContainer();
    ~ASTDataContainer();

    ASTDataContainer(const ASTDataContainer&) = delete;
    ASTDataContainer& operator=(const ASTDataContainer&) = delete;
    ASTDataContainer(ASTDataContainer&&) = delete;
    ASTDataContainer& operator=(ASTDataContainer&&) = delete;

    std::pair<ASTNodeID, VariableData*> newVariable(VariableType type, std::string_view name = "") {
        auto id = _data.size();
        auto& data = _data.emplace_back(type);
        _names.push_back(name);

        return std::make_pair(id, &data);
    }

    const VariableData& getData(ASTNodeID id) const {
        return _data[id.value()];
    }

    VariableData& getData(ASTNodeID id) {
        return _data[id.value()];
    }

    void setData(ASTNodeID id, VariableData&& data) {
        _data[id.value()] = std::move(data);
    }

    std::string_view getName(ASTNodeID id) const {
        return _names[id.value()];
    }

private:
    std::vector<VariableData> _data;
    std::vector<std::string_view> _names;
};

}
