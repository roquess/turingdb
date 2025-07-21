#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "types/PatternEntity.h"

namespace db {

class MapLiteral;

class PatternEdge : public PatternEntity {
public:
    using EdgeTypeVector = std::vector<std::string_view>;

    PatternEdge() = default;
    ~PatternEdge() override = default;

    PatternEdge(const PatternEdge&) = default;
    PatternEdge(PatternEdge&&) = default;
    PatternEdge& operator=(const PatternEdge&) = default;
    PatternEdge& operator=(PatternEdge&&) = default;

    static std::unique_ptr<PatternEdge> create() {
        return std::make_unique<PatternEdge>();
    }

    const std::vector<std::string_view>& types() const {
        return _types.value();
    }

    std::vector<std::string_view>& types() {
        return _types.value();
    }

    bool hasTypes() const {
        return _types.has_value();
    }

    void setTypes(std::optional<EdgeTypeVector>&& types) {
        _types = std::move(types);
    }

private:
    std::optional<EdgeTypeVector> _types;
};

}
