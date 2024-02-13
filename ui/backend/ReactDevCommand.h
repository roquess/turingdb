#pragma once

#include "ServerCommand.h"
#include "Command.h"

namespace ui {

class ReactDevCommand : public ServerCommand {
public:
    ReactDevCommand();
    ~ReactDevCommand() override;

    void runDev(ProcessGroup& group) override;

private:
    Command _cmd;
};

}
