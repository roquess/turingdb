#pragma once

#include <exception>
#include <string>

namespace net {

class HTTPException : public std::exception {
public:
    explicit HTTPException(std::string msg)
        : _msg(std::move(msg))
    {
    }

    const char* what() const noexcept override {
        return _msg.c_str();
    }

private:
    std::string _msg;
};

}
