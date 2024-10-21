#pragma once

#include <array>
#include <cstddef>
#include <cstring>
#include <string_view>

template <size_t ChunkSize = 1024 * 8, size_t ChunkCount = 10>

class FixedSizeBuffer {
public:
    static constexpr size_t BufferSize = ChunkCount * ChunkSize;

    FixedSizeBuffer() = default;
    ~FixedSizeBuffer() = default;

    FixedSizeBuffer(const FixedSizeBuffer&) = delete;
    FixedSizeBuffer(FixedSizeBuffer&&) = delete;
    FixedSizeBuffer& operator=(const FixedSizeBuffer&) = delete;
    FixedSizeBuffer& operator=(FixedSizeBuffer&&) = delete;

    class Writer {
    public:
        explicit Writer(FixedSizeBuffer* buffer)
            : _buffer(buffer)
        {
        }

        void reset() {
            _buffer->_data[0] = '\0';
            _buffer->_bytes = 0;
        }

        void writeChar(char c) {
            char* buffer = getBuffer();
            *buffer = c;
            setWrittenBytes(1);
        }

        void writeString(std::string_view str) {
            char* buffer = getBuffer();
            memcpy(buffer, str.data(), str.size());
            setWrittenBytes(str.size());
        }

        void setWrittenBytes(size_t bytesWritten) {
            _buffer->_bytes += bytesWritten;
        }

        size_t getBufferSize() const { return BufferSize - _buffer->_bytes; }
        size_t getChunkSize() const {
            const size_t avail = BufferSize - _buffer->_bytes;
            return std::min(ChunkSize, avail);
        }

        char* getBuffer() { return &_buffer->_data[_buffer->_bytes]; }

        bool isFull() const { return getBufferSize() == 0; }

    private:
        FixedSizeBuffer* _buffer {nullptr};
    };

    class Reader {
    public:
        explicit Reader(FixedSizeBuffer* buffer)
            : _buffer(buffer) 
        {
        }

        const char* getData() const { return _buffer->_data.data(); }
        char* getData() { return _buffer->_data.data(); }
        size_t getSize() const { return _buffer->_bytes; }

        void dump() const;

    private:
        FixedSizeBuffer* _buffer {nullptr};
    };

    Writer getWriter() { return Writer(this); }
    Reader getReader() { return Reader(this); }

private:
    static constexpr size_t SAFETY_MARGIN = 256;
    std::array<char, BufferSize + SAFETY_MARGIN> _data {};
    size_t _bytes {0};
};

using Buffer = FixedSizeBuffer<>;

