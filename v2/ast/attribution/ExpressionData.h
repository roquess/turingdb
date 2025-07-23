#pragma once

#include "attribution/VariableDecl.h"
#include "metadata/LabelSet.h"

namespace db {

struct PropertyExpressionData {
    VariableDecl _var;

    using UniquePtr = std::unique_ptr<PropertyExpressionData, void (*)(PropertyExpressionData*)>;

    static UniquePtr create(VariableDecl var) {
        return UniquePtr {
            new PropertyExpressionData {var},
            &cleanUp};
    }

    static void cleanUp(PropertyExpressionData* ptr) {
        delete ptr;
    }
};

struct NodeLabelExpressionData {
    VariableDecl _var;
    LabelSet _labels;

    using UniquePtr = std::unique_ptr<NodeLabelExpressionData, void (*)(NodeLabelExpressionData*)>;

    static UniquePtr create(VariableDecl var) {
        return UniquePtr {
            new NodeLabelExpressionData {var},
            &cleanUp};
    }

    static void cleanUp(NodeLabelExpressionData* ptr) {
        delete ptr;
    }
};

}
