#pragma once

#include <string_view>

namespace db {

class ASTContext;
class QueryCommand;

class QueryParser {
public:
    QueryParser(ASTContext* ctxt);

    QueryCommand* parse(std::string_view query);

private:
    ASTContext* _astCtxt {nullptr};
};

}
