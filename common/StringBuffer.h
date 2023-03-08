#ifndef _COMMON_STRING_BUFFER_
#define _COMMON_STRING_BUFFER_

#include <stdint.h>
#include <string>

class StringBuffer {
public:
    enum BufferKind {
        MALLOC_BUFFER,
        MMAP_BUFFER
    };

    virtual ~StringBuffer() = default;

    virtual const BufferKind getKind() const = 0;

    char* getData() const { return _data; }

    size_t getSize() const { return _size; }

    static StringBuffer* readFromFile(const std::string& filename);

protected:
    char* const _data;
    const size_t _size;

    StringBuffer(char* data, size_t size);
};

#endif
