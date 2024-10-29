#include "gtest/gtest.h"

#include <thread>
#include <string>
#include <spdlog/spdlog.h>

#include "DBServerConfig.h"
#include "DBServer.h"

#include "HTTPClient.h"

using namespace db;
using namespace net::HTTP;

namespace {

bool checkEndpointOK(const DBServerConfig& serverConfig,
                     const std::string& endpoint) {
    HTTPClient client;
    HTTPClient::ResponseBuffer buffer;

    const std::string url = serverConfig.getURL()+endpoint;

    const auto res = client.fetch(url, "", buffer);

    return res == Status::OK;
}

}

class ServerTest : public ::testing::Test {
};

constexpr auto HttpServerStartSleepDelay = std::chrono::milliseconds(50);

TEST_F(ServerTest, queryEndpointOK) {
    DBServerConfig serverConfig;
    DBServer server(serverConfig);

    std::thread serverThread([&server]() {
        server.start();
    });

    std::thread clientThread([serverConfig]() {
        // Wait for the http server to accept connections on /query
        for (;;) {
            std::this_thread::sleep_for(HttpServerStartSleepDelay);
            if (checkEndpointOK(serverConfig, "/query")) {
                break;
            }
        }
    });

    // Wait for client thread to terminate
    clientThread.join();

    // Terminate server thread
    serverThread.detach();
}
