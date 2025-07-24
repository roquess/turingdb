#include "CypherAnalyzer.h"

#include <spdlog/fmt/bundled/core.h>

#include "AnalyzeException.h"
#include "CypherAST.h"

#include "attribution/DeclContext.h"
#include "attribution/ASTNodeDataStructs.h"
#include "attribution/VariableDecl.h"
#include "attribution/VariableType.h"

#include "expressions/AtomExpression.h"
#include "expressions/BinaryExpression.h"
#include "expressions/NodeLabelExpression.h"
#include "expressions/PathExpression.h"
#include "expressions/PropertyExpression.h"
#include "expressions/StringExpression.h"
#include "expressions/UnaryExpression.h"

#include "types/SinglePartQuery.h"
#include "types/Projection.h"

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
    const auto& projection = returnSt.getProjection();
    if (projection.isDistinct()) {
        throw AnalyzeException("DISTINCT not supported");
    }

    if (projection.hasSkip()) {
        throw AnalyzeException("SKIP not supported");
    }

    if (projection.hasLimit()) {
        throw AnalyzeException("LIMIT not supported");
    }

    if (projection.isAll()) {
        return;
    }

    for (Expression* item : projection.items()) {
        analyze(*item);
    }
}

void CypherAnalyzer::analyze(const Pattern& pattern) {
    const auto& dataContainer = _ast->dataContainer();

    for (const auto& element : pattern.elements()) {
        analyze(*element);
    }

    if (pattern.hasWhere()) {
        auto& whereExpr = pattern.getWhere().getExpression();
        analyze(whereExpr);

        const auto& whereData = dataContainer.getData(whereExpr.id());

        if (whereData.type() != VariableType::Bool) {
            throw AnalyzeException("WHERE expression must be a boolean");
        }
    }
}

void CypherAnalyzer::analyze(const PatternElement& element) {
    const auto& entities = element.getEntities();

    for (const auto& entity : entities) {
        if (auto* node = dynamic_cast<NodePattern*>(entity)) {
            analyze(*node);
        } else if (auto* edge = dynamic_cast<EdgePattern*>(entity)) {
            analyze(*edge);
        } else {
            throw AnalyzeException("Unsupported pattern entity type");
        }
    }
}

void CypherAnalyzer::analyze(NodePattern& node) {
    auto [varID, varData, varDecl] = node.hasSymbol()
                                       ? getOrCreateDecl(VariableType::NodePattern, node.symbol()._name)
                                       : createDecl(VariableType::NodePattern);

    auto& data = varData->emplace<NodePatternData>();

    if (node.hasLabels()) {
        const auto& labelMap = _graphMetadata.labels();

        for (const auto& label : node.labels()) {
            const std::optional<LabelID> labelID = labelMap.get(label);

            if (!labelID) {
                throw AnalyzeException(fmt::format("Unknown label: {}", label));
            }

            data._labelConstraints.set(labelID.value());
        }
    }

    constexpr auto compatible = [](ValueType vt, VariableType exprType) {
        switch (exprType) {
            case VariableType::Null:
            case VariableType::NodePattern:
            case VariableType::EdgePattern:
                return false;
            case VariableType::Integer:
                return vt == ValueType::Int64 || vt == ValueType::UInt64 || vt == ValueType::Double;
            case VariableType::Double:
                return vt == ValueType::Double;
            case VariableType::String:
            case VariableType::Char:
                return vt == ValueType::String;
            case VariableType::Bool:
                return vt == ValueType::Bool;
            case VariableType::List:
            case VariableType::Map:
                return false;
            case VariableType::Invalid:
            case VariableType::_SIZE:
                throw AnalyzeException("Invalid variable type");
        }

        return false;
    };

    if (node.hasProperties()) {
        const auto& propTypeMap = _graphMetadata.propTypes();

        for (const auto& [propName, expr] : node.properties()) {
            const std::optional<PropertyType> propType = propTypeMap.get(propName);

            if (!propType) {
                throw AnalyzeException(fmt::format("Unknown property: {}", propName));
            }

            data._exprConstraints.emplace_back(propType.value(), expr);

            if (!compatible(propType->_valueType, varData->type())) {
                throw AnalyzeException(fmt::format("Cannot evaluate node property: types '{}' and '{}' are incompatible",
                                                   ValueTypeName::value(propType->_valueType),
                                                   VariableTypeName::value(varData->type())));
            }
        }
    }

    node.setID(varID);
}

void CypherAnalyzer::analyze(EdgePattern& edge) {
    auto [varID, varData, varDecl] = edge.hasSymbol()
                                       ? getOrCreateDecl(VariableType::EdgePattern, edge.symbol()._name)
                                       : createDecl(VariableType::EdgePattern);

    auto& data = varData->emplace<EdgePatternData>();

    if (edge.hasTypes()) {
        const auto& edgeTypeMap = _graphMetadata.edgeTypes();

        for (const auto& et : edge.types()) {
            const std::optional<EdgeTypeID> etID = edgeTypeMap.get(et);

            if (!etID) {
                throw AnalyzeException(fmt::format("Unknown edge type: {}", et));
            }

            data._edgeTypeConstraints.push_back(etID.value());
        }
    }

    constexpr auto compatible = [](ValueType vt, VariableType exprType) {
        switch (exprType) {
            case VariableType::Null:
            case VariableType::NodePattern:
            case VariableType::EdgePattern:
                return false;
            case VariableType::Integer:
                return vt == ValueType::Int64 || vt == ValueType::UInt64 || vt == ValueType::Double;
            case VariableType::Double:
                return vt == ValueType::Double;
            case VariableType::String:
            case VariableType::Char:
                return vt == ValueType::String;
            case VariableType::Bool:
                return vt == ValueType::Bool;
            case VariableType::List:
            case VariableType::Map:
                return false;
            case VariableType::Invalid:
            case VariableType::_SIZE:
                throw AnalyzeException("Invalid variable type");
        }

        return false;
    };

    if (edge.hasProperties()) {
        const auto& propTypeMap = _graphMetadata.propTypes();

        for (const auto& [propName, expr] : edge.properties()) {
            const std::optional<PropertyType> propType = propTypeMap.get(propName);

            if (!propType) {
                throw AnalyzeException(fmt::format("Unknown property: {}", propName));
            }

            data._exprConstraints.emplace_back(propType.value(), expr);

            if (!compatible(propType->_valueType, varData->type())) {
                throw AnalyzeException(fmt::format("Cannot evaluate edge property: types '{}' and '{}' are incompatible",
                                                   ValueTypeName::value(propType->_valueType),
                                                   VariableTypeName::value(varData->type())));
            }
        }
    }

    edge.setID(varID);
}

void CypherAnalyzer::analyze(Expression& expr) {
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
    } else {
        throw AnalyzeException("Unsupported expression type");
    }
}

void CypherAnalyzer::analyze(BinaryExpression& expr) {
    auto& lhs = expr.left();
    auto& rhs = expr.right();

    analyze(lhs);
    analyze(rhs);

    auto& dataContainer = _ast->dataContainer();

    const VariableData& lhsVarData = dataContainer.getData(lhs.id());
    const VariableData& rhsVarData = dataContainer.getData(rhs.id());

    const VariableType lhsType = lhsVarData.type();
    const VariableType rhsType = rhsVarData.type();

    VariableType type = VariableType::Invalid;

    constexpr auto listOrMap = [](VariableType a, VariableType b) {
        return a == VariableType::List
            || a == VariableType::Map
            || b == VariableType::List
            || b == VariableType::Map;
    };

    constexpr auto compatible = [](VariableType a, VariableType b) {
        return a == b
            || (a == VariableType::Integer && b == VariableType::Double)
            || (a == VariableType::Double && b == VariableType::Integer)
            || (a == VariableType::String && b == VariableType::Char)
            || (a == VariableType::Char && b == VariableType::String);
    };

    switch (expr.getBinaryOperator()) {
        case BinaryOperator::Or:
        case BinaryOperator::Xor:
        case BinaryOperator::And: {
            // Binary operation
            if (lhsType != VariableType::Bool || rhsType != VariableType::Bool) {
                throw AnalyzeException("Operands must be booleans");
            }

            type = VariableType::Bool;
        } break;

        case BinaryOperator::NotEqual:
        case BinaryOperator::Equal: {
            case BinaryOperator::LessThan:
            case BinaryOperator::GreaterThan:
            case BinaryOperator::LessThanOrEqual:
            case BinaryOperator::GreaterThanOrEqual: {
                if (listOrMap(lhsType, rhsType)) {
                    throw AnalyzeException("Lists and maps cannot be compared");
                }

                if (!compatible(lhsType, rhsType)) {
                    throw AnalyzeException("Operands must be compatible types");
                }

                type = VariableType::Bool;
            } break;

            case BinaryOperator::Add:
            case BinaryOperator::Sub:
            case BinaryOperator::Mult:
            case BinaryOperator::Div:
            case BinaryOperator::Mod:
            case BinaryOperator::Pow: {
                if (!compatible(lhsType, rhsType)) {
                    throw AnalyzeException("Operands must be compatible types");
                }

                if (lhsType == VariableType::Double || rhsType == VariableType::Double) {
                    type = VariableType::Double;
                } else if (lhsType == VariableType::String || rhsType == VariableType::String) {
                    type = VariableType::String;
                } else {
                    type = lhsType;
                }

            } break;

            case BinaryOperator::In: {
                if (rhsType != VariableType::List) {
                    throw AnalyzeException("IN operand must be a list or map");
                }

                if (lhsType == VariableType::List || lhsType == VariableType::Map) {
                    throw AnalyzeException("Left operand must be a scalar");
                }

                type = VariableType::Bool;
            } break;
        }
    }

    const auto [exprID, data, varDecl] = createDecl(type);
    expr.setID(exprID);
}

void CypherAnalyzer::analyze(UnaryExpression& expr) {
    auto& dataContainer = _ast->dataContainer();

    auto& operand = expr.right();
    analyze(operand);

    const auto& operandVarData = dataContainer.getData(operand.id());
    VariableType type = VariableType::Invalid;

    switch (expr.getUnaryOperator()) {
        case UnaryOperator::Not: {
            if (operandVarData.type() != VariableType::Bool) {
                throw AnalyzeException("NOT operand must be a boolean");
            }

            type = VariableType::Bool;
        } break;

        case UnaryOperator::Minus:
        case UnaryOperator::Plus: {
            if (operandVarData.type() == VariableType::Integer) {
                type = VariableType::Integer;
            } else if (operandVarData.type() == VariableType::Double) {
                type = VariableType::Double;
            } else {
                throw AnalyzeException("Operand must be an integer or double");
            }

        } break;
    }

    const auto [varID, data, varDecl] = createDecl(type);
    expr.setID(varID);
}

void CypherAnalyzer::analyze(AtomExpression& expr) {
    const auto& atom = expr.value();

    if (const auto* symbol = std::get_if<Symbol>(&atom)) {
        VariableDecl& var = _ctxt->getVariable(symbol->_name);
        const auto [varID, varData] = _ast->dataContainer().newVariable(var.type());
        varData->setDecl(&var);

        expr.setID(varID);

    } else if (const auto* literal = std::get_if<Literal>(&atom)) {
        VariableType type = VariableType::Invalid;

        switch (literal->type()) {
            case Literal::type<std::monostate>(): {
            } break;
            case Literal::type<bool>(): {
                type = VariableType::Bool;
            } break;
            case Literal::type<int64_t>(): {
                type = VariableType::Integer;
            } break;
            case Literal::type<double>(): {
                type = VariableType::Double;
            } break;
            case Literal::type<std::string_view>(): {
                type = VariableType::String;
            } break;
            case Literal::type<char>(): {
                type = VariableType::Char;
            } break;
            case Literal::type<MapLiteral*>(): {
                type = VariableType::Map;
            } break;
        }

        const auto [varID, varData, varDecl] = createDecl(type);
        varData->emplace<LiteralExpressionData>(literal);
        expr.setID(varID);

    } else if ([[maybe_unused]] const auto* param = std::get_if<Parameter>(&atom)) {
        throw AnalyzeException("Parameters not supported");
    }
}

void CypherAnalyzer::analyze(PropertyExpression& expr) {
    const auto& qualifiedName = expr.name();

    if (qualifiedName.size() != 2) {
        throw AnalyzeException("Only length 2 property expressions are supported");
    }

    std::string_view varName = qualifiedName.get(0);
    std::string_view propName = qualifiedName.get(1);

    const VariableDecl& var = _ctxt->getVariable(varName);

    if (var.type() != VariableType::NodePattern && var.type() != VariableType::EdgePattern) {
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

    const auto [varID, varData, varDecl] = createDecl(type);
    expr.setID(varID);
    varData->emplace<PropertyExpressionData>(var);
}

void CypherAnalyzer::analyze(StringExpression& expr) {
    auto& dataContainer = _ast->dataContainer();

    auto& lhs = expr.left();
    auto& rhs = expr.right();

    if (!lhs.id().valid()) {
        analyze(lhs);
    }

    if (!rhs.id().valid()) {
        analyze(rhs);
    }

    const auto& lhsVarData = dataContainer.getData(lhs.id());
    const auto& rhsVarData = dataContainer.getData(rhs.id());

    if (lhsVarData.type() != VariableType::String || rhsVarData.type() != VariableType::String) {
        throw AnalyzeException("String expressions operands must be strings");
    }

    const auto [varID, data, varDecl] = createDecl(VariableType::Bool);
    expr.setID(varID);
}

void CypherAnalyzer::analyze(NodeLabelExpression& expr) {
    const auto& labelMap = _graphMetadata.labels();

    const VariableDecl& var = _ctxt->getVariable(expr.symbol()._name);
    const auto [varID, varData, varDecl] = createDecl(VariableType::Bool);
    expr.setID(varID);

    auto& data = varData->emplace<NodeLabelExpressionData>(var);
    LabelSet& labelset = data._labels;

    for (const auto& label : expr.labels()) {
        const std::optional<LabelID> labelID = labelMap.get(label);
        if (!labelID) {
            throw AnalyzeException(fmt::format("Unknown label: {}", label));
        }

        labelset.set(labelID.value());
    }
}

void CypherAnalyzer::analyze(PathExpression& expr) {
    throw AnalyzeException("Path expressions not supported");
}

std::tuple<ASTNodeID, VariableData*, VariableDecl*> CypherAnalyzer::getOrCreateDecl(VariableType type, std::string_view name) {
    auto [varID, data] = _ast->dataContainer().newVariable(type, name);
    VariableDecl& decl = _ctxt->getOrCreateNamedVariable(type, name);
    data->setDecl(&decl);
    return std::make_tuple(varID, data, &decl);
}

std::tuple<ASTNodeID, VariableData*, VariableDecl*> CypherAnalyzer::createDecl(VariableType type) {
    const auto [varID, data] = _ast->dataContainer().newVariable(type);
    VariableDecl& decl = _ctxt->createUnnamedVariable(type);
    data->setDecl(&decl);
    return std::make_tuple(varID, data, &decl);
}
