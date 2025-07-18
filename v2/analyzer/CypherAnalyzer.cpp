#include "CypherAnalyzer.h"

#include "AnalyzeException.h"
#include "CypherAST.h"

#include "types/SinglePartQuery.h"

#include "statements/Match.h"
#include "statements/Skip.h"
#include "statements/Limit.h"
#include "statements/Return.h"

using namespace db;

CypherAnalyzer::CypherAnalyzer() = default;

void CypherAnalyzer::analyze(const CypherAST& ast) {
    _ast = &ast;

    for (const auto& query : ast.queries()) {
        if (const auto* q = dynamic_cast<const SinglePartQuery*>(query.get())) {
            analyzeSinglePartQuery(*q);
            continue;
        }

        throw AnalyzeException("Unsupported query type");
    }
}

void CypherAnalyzer::analyzeSinglePartQuery(const SinglePartQuery& query) {
    for (const auto* statement : query.getStatements()) {
        if (const auto* s = dynamic_cast<const Match*>(statement)) {
            continue;
        }

        if (const auto* s = dynamic_cast<const Skip*>(statement)) {
            continue;
        }

        if (const auto* s = dynamic_cast<const Limit*>(statement)) {
            continue;
        }

        if (const auto* s = dynamic_cast<const Return*>(statement)) {
            continue;
        }
    }
}
