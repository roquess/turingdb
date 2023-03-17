#ifndef _TURING_SYSTEM_
#define _TURING_SYSTEM_

#include <string>

class System {
public:
    static int runCommand(const std::string& cmd);

private:
    System() = default;
    ~System() = default;
};

#endif
