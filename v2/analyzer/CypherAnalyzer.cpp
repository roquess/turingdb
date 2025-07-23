#include "CypherAnalyzer.h"

#include <spdlog/fmt/bundled/core.h>

#include "AnalyzeException.h"
#include "CypherAST.h"

#include "attribution/DeclContext.h"
#include "attribution/VariableDecl.h"
#include "attribution/VariableType.h"
#include "attribution/ExpressionData.h"

#include "expressions/AtomExpression.h"
#include "expressions/BinaryExpression.h"
#include "expressions/NodeLabelExpression.h"
#include "expressions/PathExpression.h"
#include "expressions/PropertyExpression.h"
#include "expressions/StringExpression.h"
#include "expressions/UnaryExpression.h"

#include "types/SinglePartQuery.h"

#include "statements/Match.h"
#include "statements/Skip.h"
#include "statements/Limit.h"
#include "statements/Return.h"

using namespace db;

CypherAnalyzer::CypherAnalyzer(std::unique_ptr<CypherAST> ast,
                               GraphView graphView)
    : _ast(std::move(ast)),
      _graphView(graphView),
      _graphMetadata(graphView.metadata()) {
}

CypherAnalyzer::~CypherAnalyzer() = default;

void CypherAnalyzer::analyze() {
    for (const auto& query : _ast->queries()) {
        _ctxt = &query->getRootContext();

        if (const auto* q = dynamic_cast<SinglePartQuery*>(query.get())) {
            analyze(*q);
        } else {
            throw AnalyzeException("Unsupported query type");
        }
    }
}

void CypherAnalyzer::analyze(const SinglePartQuery& query) {
    for (const auto* statement : query.getStatements()) {
        if (const auto* s = dynamic_cast<const Match*>(statement)) {
            fmt::print("Analyzing match statement\n");
            analyze(*s);
        }

        else if (const auto* s = dynamic_cast<const Return*>(statement)) {
            analyze(*s);
        } else {
            throw AnalyzeException("Unsupported statement type");
        }
    }
}

void CypherAnalyzer::analyze(const Match& matchSt) {
    if (matchSt.isOptional()) {
        throw AnalyzeException("OPTIONAL MATCH not supported");
    }

    const auto& pattern = matchSt.getPattern();
    analyze(pattern);

    if (matchSt.hasLimit()) {
        throw AnalyzeException("LIMIT not supported");
    }

    if (matchSt.hasSkip()) {
        throw AnalyzeException("SKIP not supported");
    }

    if (!matchSt.hasPattern()) {
        throw AnalyzeException("MATCH statement must have a pattern");
    }
}

void CypherAnalyzer::analyze(const Skip& skipSt) {
    throw AnalyzeException("SKIP not supported");
}

void CypherAnalyzer::analyze(const Limit& limitSt) {
    throw AnalyzeException("LIMIT not supported");
}

void CypherAnalyzer::analyze(const Return& returnSt) {
    throw AnalyzeException("RETURN not supported");
}

void CypherAnalyzer::analyze(const Pattern& pattern) {
    fmt::print("Analyzing pattern\n");
    for (const auto& element : pattern.elements()) {
        analyze(*element);
    }

    if (pattern.hasWhere()) {
        analyze(pattern.getWhere().getExpression());
    }

    fmt::print("Done analyzing pattern\n");
}

void CypherAnalyzer::analyze(const PatternElement& element) {
    fmt::print("Analyzing pattern element\n");
    const auto& entities = element.getEntities();

    for (const auto& entity : entities) {
        fmt::print("Analyzing pattern entity\n");
        if (auto* node = dynamic_cast<NodePattern*>(entity)) {
            analyze(*node);
        } else if (auto* edge = dynamic_cast<EdgePattern*>(entity)) {
            analyze(*edge);
        }
    }

    fmt::print("Done analyzing pattern element\n");
}

void CypherAnalyzer::analyze(NodePattern& node) {
    const VariableDecl var = node.hasSymbol()
                               ? _ctxt->createNamedVariable(node.symbol()._name, VariableType::Node)
                               : _ctxt->createUnnamedVariable(VariableType::Node);

    node.setID(var.id());
}

void CypherAnalyzer::analyze(EdgePattern& edge) {
    const VariableDecl var = edge.hasSymbol()
                               ? _ctxt->createNamedVariable(edge.symbol()._name, VariableType::Edge)
                               : _ctxt->createUnnamedVariable(VariableType::Edge);

    edge.setID(var.id());
}

void CypherAnalyzer::analyze(Expression& expr) {
    fmt::print("Analyzing expression\n");
    if (auto* e = dynamic_cast<BinaryExpression*>(&expr)) {
        analyze(*e);
    } else if (auto* e = dynamic_cast<UnaryExpression*>(&expr)) {
        analyze(*e);
    } else if (auto* e = dynamic_cast<AtomExpression*>(&expr)) {
        analyze(*e);
    } else if (auto* e = dynamic_cast<PropertyExpression*>(&expr)) {
        analyze(*e);
    } else if (auto* e = dynamic_cast<StringExpression*>(&expr)) {
        analyze(*e);
    } else if (auto* e = dynamic_cast<NodeLabelExpression*>(&expr)) {
        analyze(*e);
    } else if (auto* e = dynamic_cast<PathExpression*>(&expr)) {
        analyze(*e);
    }
}

void CypherAnalyzer::analyze(BinaryExpression& expr) {
    auto& lhs = expr.left();
    auto& rhs = expr.right();

    if (!lhs.id().valid()) {
        analyze(lhs);
    }

    if (!rhs.id().valid()) {
        analyze(rhs);
    }

    throw AnalyzeException("Binary expressions not supported");
}

void CypherAnalyzer::analyze(UnaryExpression& expr) {
    auto& operand = expr.right();

    if (!operand.id().valid()) {
        analyze(operand);
    }

    throw AnalyzeException("Unary expressions not supported");
}

void CypherAnalyzer::analyze(AtomExpression& expr) {
    throw AnalyzeException("Atom expressions not supported");
}

void CypherAnalyzer::analyze(PropertyExpression& expr) {
    const auto& qualifiedName = expr.name();

    if (qualifiedName.size() != 2) {
        throw AnalyzeException("Only length 2 property expressions are supported");
    }

    std::string_view varName = qualifiedName.get(0);
    std::string_view propName = qualifiedName.get(1);

    const VariableDecl var = _ctxt->getVariable(varName);

    if (var.type() != VariableType::Node && var.type() != VariableType::Edge) {
        throw AnalyzeException(fmt::format("Variable '{}' is not a node or edge", varName));
    }

    const std::optional<PropertyType> propType = _graphMetadata.propTypes().get(propName);

    if (!propType) {
        throw AnalyzeException(fmt::format("Property '{}' not found", propName));
    }

    VariableType type = VariableType::Invalid;

    switch (propType->_valueType) {
        case ValueType::UInt64:
        case ValueType::Int64: {
            type = VariableType::Integer;
        } break;
        case ValueType::Bool: {
            type = VariableType::Bool;
        } break;
        case ValueType::Double: {
            type = VariableType::Double;
        } break;
        case ValueType::String: {
            type = VariableType::String;
        } break;
        case ValueType::Invalid:
        case ValueType::_SIZE:
            throw AnalyzeException("Invalid property type");
    }

    VariableDecl exprVar = _ctxt->createUnnamedVariable(type);
    expr.setID(exprVar.id());
    exprVar.setData(VariableData::create<PropertyExpressionData>(var));
}

void CypherAnalyzer::analyze(StringExpression& expr) {
    auto& lhs = expr.left();
    auto& rhs = expr.right();

    if (!lhs.id().valid()) {
        analyze(lhs);
    }

    if (!rhs.id().valid()) {
        analyze(rhs);
    }

    const VariableDecl lhsVar = _ctxt->getUnnamedVariable(lhs.id());
    const VariableDecl rhsVar = _ctxt->getUnnamedVariable(rhs.id());

    if (lhsVar.type() != VariableType::String || rhsVar.type() != VariableType::String) {
        throw AnalyzeException("String expressions operands must be strings");
    }

    VariableDecl exprVar = _ctxt->createUnnamedVariable(VariableType::Bool);
    expr.setID(exprVar.id());
}

void CypherAnalyzer::analyze(NodeLabelExpression& expr) {
    const auto& labelMap = _graphMetadata.labels();

    VariableDecl var = _ctxt->getOrCreateNamedVariable(expr.symbol()._name, VariableType::Node);
    expr.setID(var.id());

    VariableData data = VariableData::create<NodeLabelExpressionData>(var);
    NodeLabelExpressionData& labelData = data.as<NodeLabelExpressionData>();
    LabelSet& labelset = labelData._labels;

    for (const auto& label : expr.labels()) {
        const std::optional<LabelID> labelID = labelMap.get(label);
        if (!labelID) {
            throw AnalyzeException(fmt::format("Unknown label: {}", label));
        }

        labelset.set(labelID.value());
    }

    var.setData(std::move(data));
}

void CypherAnalyzer::analyze(PathExpression& expr) {
    throw AnalyzeException("Path expressions not supported");
}
