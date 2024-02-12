#pragma once

#include <string>
#include <vector>

class DBServerConfig;

class DBServer {
public:
    DBServer(const DBServerConfig& serverConfig);
    ~DBServer();

    bool run();
    bool run(const std::vector<std::string>& dbNames);

private:
    const DBServerConfig& _config;
};
