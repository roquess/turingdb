#include "QueryCommand.h"

#include "ASTContext.h"

using namespace db::query;

void QueryCommand::registerCmd(ASTContext* ctxt) {
    ctxt->addCmd(this);
}

/* ListCommand */

ListCommand::ListCommand()
{
}

ListCommand::~ListCommand() {
}

ListCommand* ListCommand::create(ASTContext* ctxt) {
    ListCommand* listCmd = new ListCommand();
    listCmd->registerCmd(ctxt);
    return listCmd;
}
