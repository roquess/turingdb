#pragma once

#include "Buffer.h"
#include "HTTPParsingInfo.h"

namespace net {

class HTTPParser {
public:
    explicit HTTPParser(Buffer* inputBuffer);

    /* @brief Analyze the incoming data.
     *
     * The HTTP header must be received in one chunk.
     * Only the payload is allowed to be received in multiple chunks
     * */
    bool analyze();
    void reset();

    const HTTP::Info& getHttpInfo() const { return _info; }

private:
    Buffer::Reader _reader;
    char* _currentPtr {nullptr};
    char* _payloadBegin {nullptr};
    uint64_t _payloadSize {};
    bool _parsedHeader = false;
    HTTP::Info _info;

    size_t getSize() { return getEndPtr()-_currentPtr; }
    char* getEndPtr() { return _reader.getData() + _reader.getSize(); }

    void parseMethod();
    void parseGET();
    void parsePOST();
    void parseURI();
    void parseContentLengthAndJump();
    void jumpToPayload();
};

}
