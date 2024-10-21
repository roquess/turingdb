#pragma once

#include <string_view>
#include <array>

namespace net::HTTP {

enum class Param {
    db = 0,
    _SIZE,
};

using Uri = std::string_view;
using Path = std::string_view;
using Payload = std::string_view;
using Params = std::array<std::string_view, (size_t)Param::_SIZE>;

enum class Method {
    UNKNOWN,
    GET,
    POST
};

struct Info {
    Method _method;
    Uri _uri;
    Path _path;
    Payload _payload;
    Params _params;

    void reset() {
        _method = HTTP::Method::UNKNOWN;
        _uri = "";
        _path = "";
        _payload = "";

        for (auto& p : _params) {
            p = "";
        }
    }
};

}
