#include "CSVLexer.h"

CSVLexer::CSVLexer(std::string_view buffer, const char delimiter)
    : _buffer(buffer),
      _current(buffer.cbegin()),
      _end(buffer.cend()),
      _delimiter(delimiter)
{
}

void CSVLexer::next() {
    if (_current == _end) {
        _token.type = Token::Type::END;
        _token.data = {_current, 0};
        return;
    }

    if (*_current == '\n') {
        _token.type = Token::Type::LINE_BREAK;
        _token.data = {_current, 1};
        return;
    }

    if (*_current == _delimiter) {
        _token.type = Token::Type::DELIMITER;
        _token.data = {_current, 1};
        return;
    }

    // Skipping white spaces
    while (*_current == ' ') {
        _current++;
    }

    lexString();
}

void CSVLexer::lexString() {
    size_t step = 0;
    const char* start = _current;

    while (_current != _end) {
        if (*_current == _delimiter || *_current == '\n') {
            _current--;
            _token.data = {start, step};
            _token.type = Token::Type::STRING;
            return;
        }

        _current++;
        step++;
    }
}
