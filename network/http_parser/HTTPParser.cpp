#include "HTTPParser.h"
#include "HTTPException.h"

#include <cctype>
#include <cstdlib>

using namespace net;

namespace {

bool isBlank(char c) {
    return (c == ' ') || (c == '\n') || (c == '\r') || (c == '\t');
}

bool isURIValid(char c) {
    return (c == '/') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9')
        || (c == '_' || c == '=' || c == '&' || c == ';' || c == '?' || c == '-');
}

}

HTTPParser::HTTPParser(Buffer* inputBuffer)
    : _reader(inputBuffer->getReader()),
      _currentPtr(_reader.getData())
{
}

void HTTPParser::reset() {
    _currentPtr = _reader.getData();
    _payloadSize = 0;
    _payloadBegin = nullptr;
    _parsedHeader = false;
    _info.reset();
}

bool HTTPParser::analyze() {
    if (getSize() == 0) {
        return true;
    }
    if (!_parsedHeader) {
        parseMethod();
        parseURI();
        parseContentLengthAndJump();
        _parsedHeader = true;
    }


    if (!_payloadBegin) {
        _payloadBegin = _currentPtr;
    }

    _info._payload = std::string_view {_payloadBegin, getSize()};
    const bool finished = _info._payload.size() == _payloadSize;


    if (!finished && _reader.getSize() == Buffer::BufferSize) {
        throw HTTPException("Request is too big, does not fit in buffer"); // TODO Better errors
    }

    return finished;
}

void HTTPParser::parseMethod() {
    if (getSize() < 4) {
        throw HTTPException("Invalid HTTP request. Header does not contain the method"); // TODO Better errors
    }

    const char c = *_currentPtr;
    if (c == 'P' || c == 'p') {
        parsePOST();
        return;
    }

    if (c == 'G' || c == 'g') {
        parseGET();
        return;
    }

    throw HTTPException("Invalid HTTP request. Invalid method"); // TODO Better errors
}

void HTTPParser::parsePOST() {
    if (getSize() < 4) {
        throw HTTPException("Invalid HTTP request"); // TODO Better errors
    }

    const bool isPOST = (_currentPtr[1] == 'O' || _currentPtr[1] == 'o')
                     && (_currentPtr[2] == 'S' || _currentPtr[2] == 's')
                     && (_currentPtr[3] == 'T' || _currentPtr[3] == 't');

    if (!isPOST) {
        throw HTTPException("Invalid method"); // TODO Better errors
    }

    _info._method = HTTP::Method::POST;
    _currentPtr += 4;
}

void HTTPParser::parseGET() {
    if (getSize() < 4) {
        throw HTTPException("Invalid HTTP request"); // TODO Better errors
    }

    const bool isGET = (_currentPtr[1] == 'E' || _currentPtr[1] == 'e')
                    && (_currentPtr[2] == 'T' || _currentPtr[2] == 't')
                    && _currentPtr[3] == ' ';
    if (!isGET) {
        throw HTTPException("Invalid method"); // TODO Better errors
    }

    _info._method = HTTP::Method::GET;
    _currentPtr += 4;
}

void HTTPParser::parseURI() {
    auto& uri = _info._uri;

    bool foundBegin = false;
    const char* endPtr = getEndPtr();
    for (; _currentPtr < endPtr; _currentPtr++) {
        if (*_currentPtr != ' ') {
            foundBegin = true;
            break;
        }
    }
    if (!foundBegin) {
        throw HTTPException("Invalid HTTP request"); // TODO Better errors
    }

    const char* beginPtr = _currentPtr;

    bool foundEnd = false;
    for (; _currentPtr < endPtr; _currentPtr++) {
        const char c = *_currentPtr;
        if (isBlank(c)) {
            foundEnd = true;
            break;
        }

        if (!isURIValid(c)) {
            throw HTTPException("Invalid URI"); // TODO Better errors
        }
    }

    if (!foundEnd) {
        throw HTTPException("HTTP Header must be sent in one chunk"); // TODO Better errors
    }

    uri = std::string_view(beginPtr, _currentPtr);

    if (uri.empty()) {
        throw HTTPException("Invalid HTTP request"); // TODO Better errors
    }

    // Extract the path part of the URI
    // up to the ? character if any
    const char* pathBegin = uri.data();
    const char* pathPtr = pathBegin;
    const char* const uriEnd = pathPtr + uri.size();
    for (; pathPtr < uriEnd; pathPtr++) {
        if (*pathPtr == '?') {
            break;
        }
    }

    _info._path = std::string_view(pathBegin, pathPtr - pathBegin);

    // We can stop here if we are already at the end of the URI
    if (pathPtr >= uriEnd) {
        return;
    }

    // URI variables
    pathPtr++;
    auto& parameters = _info._params;
    std::string_view key;
    std::string_view value;
    const char* wordStart = pathPtr;
    for (; pathPtr < uriEnd; pathPtr++) {
        const char c = *pathPtr;
        if (c == '=') {
            key = std::string_view(wordStart, pathPtr - wordStart);
            value = std::string_view();
            wordStart = pathPtr + 1;
        } else if (c == '&') {
            value = std::string_view(wordStart, pathPtr - wordStart);
            if (!key.empty() && !value.empty()) {
                if (key == "db") {
                    parameters[(size_t)HTTP::Param::db] = value;
                }
            }

            key = std::string_view();
            value = std::string_view();
            wordStart = pathPtr + 1;
        }
    }

    if (wordStart < uriEnd && !key.empty()) {
        value = std::string_view(wordStart, uriEnd - wordStart);
        if (key == "db") {
            parameters[(size_t)HTTP::Param::db] = value;
        }
    }
}

void HTTPParser::parseContentLengthAndJump() {
    const char* endPtr = getEndPtr();
    std::string_view key = "content-length:";
    std::string_view window;

    for (; _currentPtr != endPtr; _currentPtr++) {
        if (getSize() < key.size()) {
            return jumpToPayload();
        }

        window = {_currentPtr, _currentPtr + key.size()};

        if (std::equal(key.begin(), key.end(),
                       window.begin(), window.end(),
                       [](char a, char b) {
                           return a == tolower(b);
                       })) {
            break;
        }

        const bool isEmptyLine = (_currentPtr[0] == '\r'
                                  && _currentPtr[1] == '\n'
                                  && _currentPtr[2] == '\r'
                                  && _currentPtr[3] == '\n');
        if (isEmptyLine) {
            continue;
        }
    }

    _currentPtr += key.size();
    const char* _lengthBegin = _currentPtr;

    for (; _currentPtr != endPtr; _currentPtr++) {
        if (!isdigit(*_currentPtr)) {
            _lengthBegin = _currentPtr;
            break;
        }
    }

    _payloadSize = std::strtoull(_lengthBegin, &_currentPtr, 10);
    jumpToPayload();
}

void HTTPParser::jumpToPayload() {
    const char* endPtr = getEndPtr();

    while (endPtr - _currentPtr >= 4) {
        const bool isEmptyLine = (_currentPtr[0] == '\r'
                                  && _currentPtr[1] == '\n'
                                  && _currentPtr[2] == '\r'
                                  && _currentPtr[3] == '\n');
        if (isEmptyLine) {
            _currentPtr += 4;
            return;
        }

        _currentPtr++;
    }

    throw HTTPException("Whole header must be sent in one chunk"); // TODO Better errors
}
