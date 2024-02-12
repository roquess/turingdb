#pragma once

#include "ServerCommand.h"
#include "Command.h"

namespace ui {

class BioserverCommand : public ServerCommand {
public:
    BioserverCommand(const std::vector<std::string>& dbNames);
    ~BioserverCommand() override;

    void run(ProcessGroup& group) override;

private:
    Command _cmd;
};

}
