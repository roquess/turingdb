#pragma once

#include <optional>

#include "AlignedBuffer.h"
#include "Path.h"
#include "FileResult.h"

namespace fs {

class FilePageWriter {
public:
    static constexpr size_t PAGE_SIZE = 1024ul * 1024 * 8; // 8 MiB
    using InternalBuffer = AlignedBuffer<PAGE_SIZE>;
    using InternalBufferIterator = AlignedBufferIterator<PAGE_SIZE>;

    enum class FinishStrategy : uint8_t {
        DontCare = 0,
        FillZeros,
        Truncate
    };

    FilePageWriter() = default;

    ~FilePageWriter() {
        finish();
    }

    FilePageWriter(const FilePageWriter&) = delete;
    FilePageWriter(FilePageWriter&&) noexcept = default;
    FilePageWriter& operator=(const FilePageWriter&) = delete;
    FilePageWriter& operator=(FilePageWriter&&) noexcept = default;

    [[nodiscard]] static FileResult<FilePageWriter> open(Path&& path);
    [[nodiscard]] static FileResult<FilePageWriter> openNoDirect(Path&& path);

    void write(const Byte* data, size_t size);
    void sync();
    void finish();

    void write(fs::TrivialPrimitive auto v) {
        write(reinterpret_cast<const fs::Byte*>(&v), sizeof(decltype(v)));
    }

    template <TrivialPrimitive T, size_t SpanSizeT>
    void write(std::span<T, SpanSizeT> s) {
        write(reinterpret_cast<const fs::Byte*>(s.data()), s.size() * sizeof(T));
    }

    template <CharPrimitive T>
    void write(std::basic_string_view<T> str) {
        write(reinterpret_cast<const fs::Byte*>(str.data()), str.size() * sizeof(T));
    }

    template <CharPrimitive T>
    void write(const std::basic_string<T>& str) {
        write(std::basic_string_view<T> {str});
    }

    void setFinishStrategy(FinishStrategy strategy) {
        _finishStrategy = strategy;
    }

    size_t getBytesWritten() const { return _written; };
    bool errorOccured() const { return _error.has_value(); }
    bool reachedEnd() const { return _reachedEnd; }
    const std::optional<FileError>& error() const { return _error; }

private:
    Path _path;
    std::optional<FileError> _error;
    InternalBuffer _buffer;
    int _fd {};
    FinishStrategy _finishStrategy = FinishStrategy::DontCare;
    size_t _written {};
    bool _reachedEnd = false;

    FilePageWriter(Path&& path, int fd)
        : _path(std::move(path)),
          _fd(fd) {
    }

    void flush();
};

}
