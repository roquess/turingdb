#pragma once

#include "ServerThread.h"

namespace ui {

class TailwindThread : public ServerThread {
public:
    TailwindThread() = default;
    ~TailwindThread() override = default;

private:
    void devTask() override;
};

}
