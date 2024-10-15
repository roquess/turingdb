#pragma once

#include "FlowStatus.h"
#include "TCPConnection.h"
#include "Utils.h"

#include <memory>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_map>

namespace net {

class TCPConnectionManager {
public:
    inline static constexpr std::string_view response = "HTTP/1.1 200 OK\r\n"
                                                        "context-type:text/plain\r\n"
                                                        "Connection: Keep-Alive\r\n"
                                                        "content-length:2\r\n\r\n{}";


    explicit TCPConnectionManager(utils::EpollInstance instance)
        : _instance(instance) {
    }

    void process(utils::EpollEvent& ev, FlowStatus& status) {
        utils::Socket s = ev.data.fd;
        uint32_t eventType = ev.events;
        char buffer[1024];

        // Process Connection
        if (eventType & (EPOLLRDHUP | EPOLLHUP)) {
            ::shutdown(s, SHUT_RDWR);
            ::close(s);
            _connections.erase(s);
            return;
        }

        if (eventType & EPOLLIN) {
            auto connIt = _connections.find(s);
            if (connIt == _connections.end()) {
                connIt = _connections.emplace(s, std::make_unique<TCPConnection>(s)).first;
            }

            ssize_t nread = recv(s, buffer, sizeof(buffer), 0);
            if (nread <= 0) {
                ::shutdown(s, SHUT_RDWR);
                ::close(s);
                return;
            }

            ::send(s, response.data(), response.size(), 0);

            ev.data.fd = s;
            ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
            if (!utils::epollMod(_instance, ev)) {
                utils::reportError("EpollMod existing connection");
                status = FlowStatus::CTL_ERROR;
                // TODO handle error
            }
        }
    }

private:
    utils::EpollInstance _instance {};
    std::unordered_map<utils::DataSocket, std::unique_ptr<TCPConnection>> _connections;
};

}
