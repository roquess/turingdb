#pragma once

#include "ASTContext.h"

#include "StringSpan.h"

namespace db {

class QueryCommand;

class QueryParser {
public:
    QueryParser();

    QueryCommand* parse(StringSpan query);

private:
    ASTContext _astCtxt;
};

}
