#pragma once

namespace db {

class CypherAST;
class SinglePartQuery;
class Match;
class Skip;
class Limit;
class Return;

class CypherAnalyzer {
public:
    CypherAnalyzer();
    ~CypherAnalyzer() = default;

    CypherAnalyzer(const CypherAnalyzer&) = delete;
    CypherAnalyzer(CypherAnalyzer&&) = delete;
    CypherAnalyzer& operator=(const CypherAnalyzer&) = delete;
    CypherAnalyzer& operator=(CypherAnalyzer&&) = delete;

    void analyze(const CypherAST& ast);

private:
    const CypherAST* _ast {nullptr};

    // Query types
    void analyzeSinglePartQuery(const SinglePartQuery& query);

    // Statemens
    void analyzeMatch(const Match& matchSt);
    void analyzeSkip(const Skip& skipSt);
    void analyzeLimit(const Limit& limitSt);
    void analyzeReturn(const Return& returnSt);
};

}
