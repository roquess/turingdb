#pragma once

#include <iterator>
#include <ostream>
#include <vector>

namespace db {

class CypherAST;
class SinglePartQuery;
class Match;
class Limit;
class Skip;
class Return;
class Projection;
class Pattern;
class PatternElement;
class WhereClause;
class Expression;
class NodePattern;
class EdgePattern;
class Symbol;
class MapLiteral;
class BinaryExpression;
class UnaryExpression;
class AtomExpression;
class PathExpression;
class NodeLabelExpression;
class StringExpression;
class PropertyExpression;

class CypherASTDumper {
public:
    explicit CypherASTDumper(const CypherAST& ast);
    ~CypherASTDumper() = default;

    CypherASTDumper(const CypherASTDumper&) = delete;
    CypherASTDumper& operator=(const CypherASTDumper&) = delete;
    CypherASTDumper(CypherASTDumper&&) = delete;
    CypherASTDumper& operator=(CypherASTDumper&&) = delete;

    void dump(std::ostream& output);

private:
    const CypherAST& _ast;
    std::ostream_iterator<char> _o;

    void dump(const SinglePartQuery& query);
    void dump(const Match& match);
    void dump(const Limit& lim);
    void dump(const Skip& skip);
    void dump(const Return& ret);
    void dump(const Projection& projection);
    void dump(const Pattern& pattern);
    void dump(const PatternElement& part);
    void dump(const WhereClause& where);
    void dump(const NodePattern& node);
    void dump(const EdgePattern& edge);
    void dump(const Symbol& symbol);
    void dump(const MapLiteral& map);
    void dump(const std::vector<std::string_view>& types);
    void dump(const Expression& expr);
    void dump(const BinaryExpression& expr);
    void dump(const UnaryExpression& expr);
    void dump(const AtomExpression& expr);
    void dump(const PathExpression& expr);
    void dump(const NodeLabelExpression& expr);
    void dump(const StringExpression& expr);
    void dump(const PropertyExpression& expr);
};

}

