#ifndef _FORMATS_ROW_READER_
#define _FORMATS_ROW_READER_

#include <vector>
#include <string>
#include <iostream>

#include "common/StringBuffer.h"

namespace formats {

class Field {
public:
    Field(const char* ptr, size_t size)
        : _ptr(ptr), _size(size)
    {
    }

    std::string getString() const {
        return std::string(_ptr, _size);
    }

private:
    const char* _ptr;
    size_t _size;
};

class Row {
public:
    using Fields = std::vector<Field>;

    bool isValid() const {
        return _isValid;
    }

    void invalidate() {
        _isValid = false;
        _fields.clear();
    }

    const Fields& fields() const { return _fields; }

    void addField(Field field) {
        _fields.push_back(field);
    }

private:
    bool _isValid {false};
    Fields _fields;
};

template <typename RowReaderTrait>
class RowIterator {
public:
    RowIterator(const char* ptr) 
        : _ptr(ptr)
    {
    }

    bool operator==(const RowIterator<RowReaderTrait>& other) const {
        return other._ptr == _ptr;
    }

    bool operator!=(const RowIterator<RowReaderTrait>& other) const {
        return !(*this == other);
    }

    const Row& operator*() {
        if (!_init) {
            parseRow();
            _init = true;
        }
        return _row;
    }

    RowIterator& operator++() {
        parseRow();
        return *this;
    }

private:
    const char* _ptr;
    bool _init {false};
    Row _row;

    void parseRow() {
        // Set row as not valid
        _row.invalidate();

        // Skip white space in front
        if (RowReaderTrait::SkipWhitespace) {
            skipWhitespace();
        }

        skipComments();

        char c = *_ptr;
        while (c != '\0' && !RowReaderTrait::isEndOfLine(c)) {
            // Read field
            const char* const fieldStart = _ptr;
            while (c != '\0' && !RowReaderTrait::isFieldSeparator(c) && !RowReaderTrait::isEndOfLine(c)) {
                _ptr++;
                c = *_ptr;
            }

            _row.addField(Field(fieldStart, _ptr-fieldStart));

            if (RowReaderTrait::isFieldSeparator(c)) {
                _ptr++;
                c = *_ptr;
            }
        }
    }

    void skipWhitespace() {
        char c = *_ptr;
        while (c != '\0' && RowReaderTrait::isWhitespace(c)) {
            _ptr++;
            c = *_ptr;
        }
    }

    void skipLine() {
        char c = *_ptr;
        while (c != '\0' && c != '\n') {
            _ptr++;
            c = *_ptr;
        }
        _ptr++;
    }
    
    void skipComments() {
        char c = *_ptr;
        while (c != '\0' && c == RowReaderTrait::CommentStart) {
            skipLine();
            if (RowReaderTrait::SkipWhitespace) {
                skipWhitespace();
            }
            c = *_ptr;
        }
    }
};

template <typename RowReaderTrait>
class RowReader {
public:
    using iterator = RowIterator<RowReaderTrait>;

    RowReader(const StringBuffer* buffer)
        : _beginIt(buffer->getData()),
        _endIt(buffer->getData()+buffer->getSize()-1)
    {
    }

    iterator begin() const {
        return _beginIt;
    }

    iterator end() const {
        return _endIt;
    }

private:
    iterator _beginIt;
    iterator _endIt;
};

struct TSVRowReaderTrait {
    static constexpr bool SkipWhitespace = true;
    static constexpr char CommentStart = '#';

    static constexpr bool isEndOfLine(char c) {
        return c == '\n';
    }

    static constexpr bool isFieldSeparator(char c) {
        return c == '\t';
    }

    static constexpr bool isWhitespace(char c) {
        return c == '\n' || c == ' ';
    }
};

using TSVRowReader = RowReader<TSVRowReaderTrait>;

}

#endif
