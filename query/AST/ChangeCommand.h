#pragma once

#include "QueryCommand.h"
#include "ChangeOpType.h"


namespace db {

class ChangeCommand : public QueryCommand {
public:
    static ChangeCommand* create(ASTContext* ctxt, ChangeOpType type);

    Kind getKind() const override { return Kind::CHANGE_COMMAND; }
    ChangeOpType getChangeOpType() const { return _type; }

private:
    explicit ChangeCommand(ChangeOpType type);
    ~ChangeCommand() override = default;

    ChangeOpType _type {};
};

}
