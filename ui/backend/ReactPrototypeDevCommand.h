#pragma once

#include "ServerCommand.h"
#include "Command.h"

namespace ui {

class ReactPrototypeDevCommand : public ServerCommand {
public:
    ReactPrototypeDevCommand();
    ~ReactPrototypeDevCommand() override;

    void runDev(ProcessGroup& group) override;

private:
    Command _cmd;
};

}
