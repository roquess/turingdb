#pragma once

#include "FlowStatus.h"
#include "Utils.h"

#include <atomic>
#include <functional>

namespace net {

class TCPConnectionStorage;
class TCPConnection;
using ServerProcessor = std::function<void(TCPConnection&)>;

struct ServerContext {
    utils::ServerSocket _socket {};
    utils::EpollInstance _instance {};
    utils::EpollSignal& _signalFd;
    TCPConnectionStorage& _connections;
    TCPConnection& _serverConnection;
    std::atomic<FlowStatus>& _status;
    std::atomic<bool>& _running;
    ServerProcessor _process;

    void encounteredError(FlowStatus err) {
        _status.store(err);
        _running.store(false);
    }
};

}
