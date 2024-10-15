#pragma once

#include "Utils.h"

namespace net {

class Server {
public:
    enum class Status {
        OK = 0,
        CREATE_ERROR,
        BIND_ERROR,
        LISTEN_ERROR,
        ACCEPT_ERROR,
        WAIT_ERROR,
        CTL_ERROR,

        OPT_NODELAY_ERR,
        OPT_REUSEADDR_ERR,
        OPT_REUSEPORT_ERR,
        OPT_NONBLOCK_ERR,
        OPT_KEEPALIVE_ERR,
        OPT_KEEPIDLE_ERR,
        OPT_KEEPCNT_ERR,
        OPT_KEEPINTVL_ERR,
    };

    Status initialize();
    Status start();
    Status terminate();

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
