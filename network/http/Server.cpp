#include "Server.h"

#include "TCPListener.h"
#include "TCPConnectionManager.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <spdlog/spdlog.h>
#include <sys/epoll.h>
#include <thread>
#include <vector>

using namespace net;

struct ThreadArguments {
    utils::ServerSocket _serverSocket {};
    utils::EpollInstance _epollInstance {};
    FlowStatus _status = FlowStatus::OK;
};

inline void runThread(ThreadArguments&& args) {
    constexpr size_t eventCount = 4;
    std::vector<utils::EpollEvent> events(eventCount);

    utils::ServerSocket server = args._serverSocket;
    utils::EpollInstance instance = args._epollInstance;

    TCPListener listener(server, instance);
    TCPConnectionManager connectionManager(instance);

    for (;;) {
        const int nfds = epoll_wait(instance, events.data(), eventCount, -1);
        if (nfds <= 0) {
            args._status = FlowStatus::WAIT_ERROR;
            utils::reportError("EpollWait");
            // TODO handle error
        }

        for (int i = 0; i < nfds; i++) {
            utils::EpollEvent& ev = events[i];

            if (ev.data.fd == server) {
                // Accept Connection
                listener.accept(ev, args._status);
                continue;
            }

            connectionManager.process(ev, args._status);
        }
    }
}

FlowStatus Server::initialize() {
    _serverSocket = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
    if (_serverSocket == -1) {
        utils::reportError("Create socket");
        return FlowStatus::CREATE_ERROR;
    }

    if (!utils::setReuseAddress(_serverSocket)) {
        utils::reportError("ReuseAddress");
        return FlowStatus::OPT_REUSEADDR_ERR;
    }

    if (!utils::bind(_serverSocket, _address, _port)) {
        utils::reportError("Bind");
        return FlowStatus::BIND_ERROR;
    }

    if (!utils::listen(_serverSocket)) {
        utils::reportError("Listen");
        return FlowStatus::LISTEN_ERROR;
    }

    _epollInstance = ::epoll_create1(0);

    utils::EpollEvent event {
        .events = EPOLLIN | EPOLLET | EPOLLONESHOT,
        .data = {.fd = _serverSocket},
    };

    if (!utils::epollAdd(_epollInstance, event)) {
        utils::reportError("EpollAdd root");
        return FlowStatus::CTL_ERROR;
    }

    return FlowStatus::OK;
}

FlowStatus Server::start() {
    sockaddr_in actual_server_addr {};
    socklen_t actual_server_addr_len = sizeof(actual_server_addr);

    if (getsockname(_serverSocket, (struct sockaddr*)&actual_server_addr, &actual_server_addr_len) == -1) {
        utils::reportError("GetServerAddress");
        return FlowStatus::CREATE_ERROR;
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

    return FlowStatus::OK;
}

FlowStatus Server::terminate() {
    ::close(_serverSocket);
    return FlowStatus::OK;
}
