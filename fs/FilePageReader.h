#pragma once

#include <optional>

#include "AlignedBuffer.h"
#include "Path.h"
#include "FileResult.h"

namespace fs {

class FilePageReader {
public:
    static constexpr size_t PAGE_SIZE = 1024ul * 1024 * 8; // 8 MiB
    using InternalBuffer = AlignedBuffer<PAGE_SIZE>;
    using InternalBufferIterator = AlignedBufferIterator<PAGE_SIZE>;

    FilePageReader() = default;

    [[nodiscard]] static FileResult<FilePageReader> open(Path&& path);
    [[nodiscard]] static FileResult<FilePageReader> openNoDirect(Path&& path);

    FileResult<void> nextPage();

    const InternalBuffer& getBuffer() const { return _buffer; }
    bool errorOccured() const { return _error.has_value(); }
    bool reachedEnd() const { return _reachedEnd; }
    const std::optional<FileError>& error() const { return _error; }

    [[nodiscard]] InternalBufferIterator begin() const { return _buffer.begin(); }
    [[nodiscard]] InternalBufferIterator end() const { return _buffer.end(); }

private:
    Path _path;
    int _fd {};
    InternalBuffer _buffer;
    std::optional<FileError> _error;
    bool _reachedEnd = false;

    FilePageReader(Path&& path, int fd)
        : _path(std::move(path)),
          _fd(fd)
    {
    }
};

}
