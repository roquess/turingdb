#pragma once

#include "Utils.h"

namespace net {

class TCPConnection {
public:
    explicit TCPConnection(utils::DataSocket socket)
        : _socket(socket) {
    }

private:
    utils::DataSocket _socket {};
};

}
