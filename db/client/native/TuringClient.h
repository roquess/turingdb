#pragma once

#include <string>

#include "TuringConfig.h"

namespace turing::db::client {

class TuringConfig;
class TuringClientImpl;

class TuringClient {
public:
    TuringClient();
    ~TuringClient();

    TuringConfig& getConfig() { return _config; }
    
    bool connect();
    bool executeQuery(const std::string& query);

private:
    TuringConfig _config;
    TuringClientImpl* _impl {nullptr};
};

}
