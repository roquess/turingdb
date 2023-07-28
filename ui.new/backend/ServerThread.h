#pragma once

#include <thread>

namespace ui {

class ServerThread {
public:
    virtual ~ServerThread();

    void run();
    void runDev();
    void join();

private:
    std::thread _thread;

    virtual void task() {};
    virtual void devTask() {};
};

}
