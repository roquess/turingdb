#pragma once

#include <string_view>

class CSVLexer {
public:
    struct Token {
        enum class Type {
            ERROR = 0,
            END,

            DELIMITER,
            LINE_BREAK,
            QUOTE,

            STRING,
            INT,
            DOUBLE,
        } type;

        std::string_view data;
    };

    CSVLexer(std::string_view buffer, const char delimiter);
    void next();
    void moveRight() { _current++; }

    const Token& getToken() const { return _token; }

private:
    std::string_view _buffer;
    std::string_view::const_iterator _current;
    std::string_view::const_iterator _end;
    const char _delimiter = ',';
    Token _token;

    void lexString();
};
