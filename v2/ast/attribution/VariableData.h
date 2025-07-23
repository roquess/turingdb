#pragma once

#include <variant>
#include <memory>

namespace db {

struct PropertyExpressionData;
struct NodeLabelExpressionData;
struct LiteralExpressionData;

class VariableData {
public:
    template <typename T>
    using UniquePtr = std::unique_ptr<T, void (*)(T*)>;

    using Variant = std::variant<std::monostate,
                                 UniquePtr<PropertyExpressionData>,
                                 UniquePtr<NodeLabelExpressionData>,
                                 UniquePtr<LiteralExpressionData>>;
    VariableData();
    ~VariableData();

    explicit VariableData(Variant&& data);

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
    static VariableData create(Args&&... args) {
        return VariableData {T::create(std::forward<Args>(args)...)};
    }

private:
    Variant _data;
};


}
