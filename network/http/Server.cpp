#include "Server.h"
#include "spdlog/spdlog.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <thread>
#include <vector>

using namespace net;

constexpr std::string_view response = "HTTP/1.1 200 OK\r\n"
                                      "context-type:text/plain\r\n"
                                      "Connection: Keep-Alive\r\n"
                                      "content-length:2\r\n\r\n{}";

struct ThreadArguments {
    utils::ServerSocket _serverSocket {};
    utils::EpollInstance _epollInstance {};
    Server::Status _status = Server::Status::OK;
};

inline void runThread(ThreadArguments&& args) {
    constexpr size_t eventCount = 4;
    std::vector<utils::EpollEvent> events(eventCount);

    utils::ServerSocket server = args._serverSocket;
    utils::EpollInstance instance = args._epollInstance;
    utils::EpollEvent event = {
        .events = EPOLLIN | EPOLLET | EPOLLONESHOT,
        .data = {.fd = args._serverSocket},
    };

    for (;;) {
        const int nfds = epoll_wait(instance, events.data(), eventCount, -1);
        if (nfds <= 0) {
            args._status = Server::Status::WAIT_ERROR;
            utils::reportError("EpollWait");
            // TODO handle error
        }

        for (int i = 0; i < nfds; i++) {
            utils::EpollEvent& ev = events[i];
            utils::Socket s = ev.data.fd;
            uint32_t eventType = ev.events;

            if (s == server) {
                // Accept Connection
                while ((s = ::accept(server, nullptr, nullptr)) > 0) {
                    if (!utils::setNonBlock(s)) {
                        args._status = Server::Status::OPT_NONBLOCK_ERR;
                        utils::reportError("SetNonBlock");
                        // TODO handle error
                    }

                    if (!utils::setKeepAlive(s)) {
                        utils::reportError("SetKeepAlive");
                        args._status = Server::Status::OPT_KEEPALIVE_ERR;
                        // TODO handle error
                    }

                    if (!utils::setNoDelay(s)) {
                        utils::reportError("SetNoDelay");
                        args._status = Server::Status::OPT_NODELAY_ERR;
                        // TODO handle error
                    }

                    event.data.fd = s;
                    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT | EPOLLRDHUP | EPOLLHUP;
                    if (!utils::epollAdd(instance, event)) {
                        utils::reportError("EpollAdd new connection");
                        args._status = Server::Status::CTL_ERROR;
                        // TODO handle error
                    }

                    event.data.fd = server;
                    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
                    if (!utils::epollMod(instance, event)) {
                        utils::reportError("EpollMod server accept");
                        args._status = Server::Status::CTL_ERROR;
                        // TODO handle error
                    }
                }

            } else {
                // Process Connection
                if (eventType & (EPOLLRDHUP | EPOLLHUP)) {
                    ::shutdown(s, SHUT_RDWR);
                    ::close(s);
                    continue;
                }

                if (eventType & EPOLLIN) {
                    char buffer[1024];
                    ssize_t nread = recv(s, buffer, sizeof(buffer), 0);
                    if (nread <= 0) {
                        ::shutdown(s, SHUT_RDWR);
                        ::close(s);
                        continue;
                    }

                    ::send(s, response.data(), response.size(), 0);

                    event.data.fd = s;
                    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
                    if (!utils::epollMod(instance, event)) {
                        utils::reportError("EpollMod existing connection");
                        args._status = Server::Status::CTL_ERROR;
                        // TODO handle error
                    }
                }
            }
        }
    }
}

Server::Status Server::initialize() {
    _serverSocket = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
    if (_serverSocket == -1) {
        utils::reportError("Create socket");
        return Status::CREATE_ERROR;
    }

    if (!utils::setReuseAddress(_serverSocket)) {
        utils::reportError("ReuseAddress");
        return Status::OPT_REUSEADDR_ERR;
    }

    if (!utils::bind(_serverSocket, _address, _port)) {
        utils::reportError("Bind");
        return Status::BIND_ERROR;
    }

    if (!utils::listen(_serverSocket)) {
        utils::reportError("Listen");
        return Status::LISTEN_ERROR;
    }

    _epollInstance = ::epoll_create1(0);

    utils::EpollEvent event {
        .events = EPOLLIN | EPOLLET | EPOLLONESHOT,
        .data = {.fd = _serverSocket},
    };

    if (!utils::epollAdd(_epollInstance, event)) {
        utils::reportError("EpollAdd root");
        return Status::CTL_ERROR;
    }

    return Status::OK;
}

Server::Status Server::start() {
    sockaddr_in actual_server_addr {};
    socklen_t actual_server_addr_len = sizeof(actual_server_addr);

    if (getsockname(_serverSocket, (struct sockaddr*)&actual_server_addr, &actual_server_addr_len) == -1) {
        utils::reportError("GetServerAddress");
        return Server::Status::CREATE_ERROR;
    }

    char server_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(actual_server_addr.sin_addr), server_ip, INET_ADDRSTRLEN); // Convert IP to string
    int server_port = ntohs(actual_server_addr.sin_port);                           // Convert port from network to host byte order

    spdlog::info("Server listening on P: {}, Port: {}\n", server_ip, server_port);

    std::vector<std::thread> threads;

    threads.reserve(_workerCount);

    for (size_t i = 0; i < _workerCount; i++) {
        threads.emplace_back([this] {
            runThread({
                ._serverSocket = _serverSocket,
                ._epollInstance = _epollInstance,
            });
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return Status::OK;
}

Server::Status Server::terminate() {
    ::close(_serverSocket);
    return Status::OK;
}
