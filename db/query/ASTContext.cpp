#include "ASTContext.h"

#include "QueryCommand.h"

using namespace db::query;

ASTContext::ASTContext()
{
}

ASTContext::~ASTContext() {
    for (QueryCommand* cmd : _cmds) {
        delete cmd;
    }
}

void ASTContext::addCmd(QueryCommand* cmd) {
    _cmds.push_back(cmd);
}
