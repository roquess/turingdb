#pragma once

#include "ServerThread.h"

namespace ui {

class BioserverThread : public ServerThread {
public:
    BioserverThread() = default;
    ~BioserverThread() override = default;

private:
    void task() override;
};

}
