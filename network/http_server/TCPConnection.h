#pragma once

#include "HTTPParser.h"
#include "NetWriter.h"
#include "Utils.h"

#include <cstddef>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace net {

class TCPConnectionStorage;

class TCPConnection {
public:
    TCPConnection() = default;

    void close();
    void dealloc();

    void setSocket(utils::DataSocket socket) {
        _socket = socket;
        _writer.setSocket(socket);
    }

    void setStorage(TCPConnectionStorage* storage) { _storage = storage; }
    void setStorageIndex(size_t index) { _storageIndex = index; }
    void setCloseRequired(bool v) { _closeRequired = v; }

    HTTPParser& getParser() { return _parser; }
    utils::DataSocket getSocket() const { return _socket; }
    bool isOpen() const;
    bool isCloseRequired() const { return _closeRequired; }
    size_t getStorageIndex() const { return _storageIndex; }
    Buffer& getInputBuffer() { return _inputBuffer; }
    NetWriter& getWriter() { return _writer; }

private:
    utils::DataSocket _socket {};
    TCPConnectionStorage* _storage {nullptr};
    size_t _storageIndex {};
    Buffer _inputBuffer;
    NetWriter _writer {_socket};
    HTTPParser _parser {&_inputBuffer};
    bool _closeRequired = false;
};

}
