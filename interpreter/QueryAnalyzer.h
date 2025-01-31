#pragma once

#include <stdint.h>
#include <string>

namespace db {

class ASTContext;
class QueryCommand;
class SelectCommand;
class CreateGraphCommand;
class DeclContext;
class EntityPattern;
class LoadGraphCommand;

class QueryAnalyzer {
public:
    QueryAnalyzer(ASTContext* ctxt, QueryCommand* cmd);
    ~QueryAnalyzer();

    bool analyze();

private:
    ASTContext* _ctxt {nullptr};
    QueryCommand* _cmd {nullptr};
    uint64_t _nextNewVarID {0};

    bool analyzeSelect(SelectCommand* cmd);
    bool analyzeCreateGraph(CreateGraphCommand* cmd);
    bool analyzeLoadGraph(LoadGraphCommand* cmd);
    bool analyzeEntityPattern(DeclContext* declContext,
                              EntityPattern* entity);
    std::string createVarName();
};

}
