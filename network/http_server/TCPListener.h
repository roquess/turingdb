#pragma once

#include "Utils.h"

#include <string_view>

namespace net {

struct ServerContext;

class TCPListener {
public:
    inline static constexpr std::string_view busyResponse = "HTTP/1.1 503 Service Unavailable\r\n"
                                                            "context-type:text/plain\r\n"
                                                            "Connection: close\r\n\r\n";

    explicit TCPListener(ServerContext& context);

    void accept(utils::EpollEvent& ev);

private:
    ServerContext& _ctxt;
};

}
