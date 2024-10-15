#pragma once

#include "Utils.h"
#include "FlowStatus.h"

namespace net {

class Server {
public:
    FlowStatus initialize();
    FlowStatus start();
    FlowStatus terminate();

    void setAddress(const char* address) { _address = address; };
    void setPort(uint32_t port) { _port = port; };
    void setWorkerCount(uint32_t count) { _workerCount = count; };

private:
    const char* _address = "127.0.0.1";
    uint32_t _port = 6666;
    uint32_t _workerCount = 8;

    utils::ServerSocket _serverSocket {};
    utils::EpollInstance _epollInstance {};
};

}
