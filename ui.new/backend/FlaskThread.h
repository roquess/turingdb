#pragma once

#include "ServerThread.h"

namespace ui {

class FlaskThread : public ServerThread {
public:
    FlaskThread() = default;
    ~FlaskThread() override = default;

private:
    void task() override;
    void devTask() override;
};

}
