#include "QueryCommand.h"

#include "attribution/DeclContext.h"

using namespace db;

QueryCommand::QueryCommand(DeclContainer* declContainer)
    : _rootCtxt(std::make_unique<DeclContext>(declContainer))
{
}

QueryCommand::~QueryCommand() = default;

