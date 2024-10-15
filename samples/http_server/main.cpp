#include "Server.h"

#include "spdlog/spdlog.h"

int main() {
    net::Server server;

    server.setAddress("127.0.0.1");
    server.setPort(6665);
    server.setWorkerCount(32);

    if (auto res = server.initialize(); res != net::Server::Status::OK) {
        spdlog::error("Could not initialize server: {}", (uint32_t)res);
        server.terminate();
        return 1;
    }

    if (auto res = server.start(); res != net::Server::Status::OK) {
        spdlog::error("Could not start server: {}", (uint32_t)res);
        server.terminate();
        return 1;
    };

    server.terminate();

    return 0;
}
