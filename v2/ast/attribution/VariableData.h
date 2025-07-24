#pragma once

#include <variant>
#include <memory>

#include "ASTException.h"
#include "attribution/VariableType.h"

namespace db {

class VariableDecl;

struct PropertyExpressionData;
struct NodeLabelExpressionData;
struct LiteralExpressionData;
struct NodePatternData;
struct EdgePatternData;

class VariableData {
public:
    template <typename T>
    using UniquePtr = std::unique_ptr<T, void (*)(T*)>;

    using Variant = std::variant<std::monostate,
                                 UniquePtr<PropertyExpressionData>,
                                 UniquePtr<NodeLabelExpressionData>,
                                 UniquePtr<LiteralExpressionData>,
                                 UniquePtr<NodePatternData>,
                                 UniquePtr<EdgePatternData>>;
    VariableData();
    ~VariableData();

    VariableData(VariableType type);

    VariableData(const VariableData&) = delete;
    VariableData(VariableData&&) = default;
    VariableData& operator=(const VariableData&) = delete;
    VariableData& operator=(VariableData&&) = default;

    template <typename T>
    bool is() const {
        return std::holds_alternative<std::unique_ptr<T, void (*)(T*)>>(_data);
    }

    template <typename T>
    T& as() {
        return *std::get<UniquePtr<T>>(_data);
    }

    template <typename T>
    const T& as() const {
        return *std::get<UniquePtr<T>>(_data);
    }

    template <typename T, typename... Args>
    T& emplace(Args&&... args) {
        return *_data.emplace<UniquePtr<T>>(T::create(std::forward<Args>(args)...));
    }

    VariableType type() const {
        return _type;
    }

    const VariableDecl& decl() const {
        if (_decl == nullptr) {
            throw ASTException("VariableData has no variable declaration");
        }

        return *_decl;
    }

    void setDecl(VariableDecl* decl) {
        _decl = decl;
    }

private:
    Variant _data;
    VariableType _type {};
    VariableDecl* _decl {nullptr};
};


}
