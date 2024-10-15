#pragma once

#include "FlowStatus.h"
#include "Utils.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>

namespace net {

class TCPListener {
public:
    TCPListener(utils::ServerSocket socket, utils::EpollInstance instance)
        : _socket(socket),
          _instance(instance)
    {
    }

    void accept(utils::EpollEvent& ev, FlowStatus& status) {
        utils::DataSocket s {};

        while ((s = ::accept(_socket, nullptr, nullptr)) > 0) {
            if (!utils::setNonBlock(s)) {
                status = FlowStatus::OPT_NONBLOCK_ERR;
                utils::reportError("SetNonBlock");
                // TODO handle error
            }

            if (!utils::setKeepAlive(s)) {
                utils::reportError("SetKeepAlive");
                status = FlowStatus::OPT_KEEPALIVE_ERR;
                // TODO handle error
            }

            if (!utils::setNoDelay(s)) {
                utils::reportError("SetNoDelay");
                status = FlowStatus::OPT_NODELAY_ERR;
                // TODO handle error
            }

            ev.data.fd = s;
            ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT | EPOLLRDHUP | EPOLLHUP;
            if (!utils::epollAdd(_instance, ev)) {
                utils::reportError("EpollAdd new connection");
                status = FlowStatus::CTL_ERROR;
                // TODO handle error
            }

            ev.data.fd = _socket;
            ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
            if (!utils::epollMod(_instance, ev)) {
                utils::reportError("EpollMod server accept");
                status = FlowStatus::CTL_ERROR;
                // TODO handle error
            }
        }
    }

private:
    utils::ServerSocket _socket {};
    utils::EpollInstance _instance {};
};

}
