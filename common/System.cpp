#include "System.h"

#include <stdlib.h>

int System::runCommand(const std::string& cmd) {
    return system(cmd.c_str());
}
