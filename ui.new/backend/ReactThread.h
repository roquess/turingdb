#pragma once

#include "ServerThread.h"

namespace ui {

class ReactThread : public ServerThread {
public:
    ReactThread() = default;
    ~ReactThread() override = default;

private:
    void devTask() override;
};

}
