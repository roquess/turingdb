#pragma once

#include "Utils.h"

namespace net {

struct ServerContext;

class TCPConnectionManager {
public:
    explicit TCPConnectionManager(ServerContext& ctxt);

    void process(utils::EpollEvent& ev);

private:
    ServerContext& _ctxt;
};

}
