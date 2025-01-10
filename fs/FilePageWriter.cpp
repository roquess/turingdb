#include "FilePageWriter.h"

#include <fcntl.h>

using namespace fs;

FileResult<FilePageWriter> FilePageWriter::open(Path&& path) {
    const int access = O_WRONLY | O_CREAT | O_TRUNC | O_DIRECT;
    const int permissions = S_IRUSR | S_IWUSR;

    const int fd = ::open(path.c_str(), access, permissions);

    if (fd == -1) {
        return FileError::result(path.c_str(), "Could not open file", ::strerror(errno));
    }

    return FilePageWriter {std::move(path), fd};
}

FileResult<FilePageWriter> FilePageWriter::openNoDirect(Path&& path) {
    const int access = O_WRONLY | O_TRUNC | O_CREAT;
    const int permissions = S_IRUSR | S_IWUSR;

    const int fd = ::open(path.c_str(), access, permissions);

    if (fd == -1) {
        return FileError::result(path.c_str(), "Could not open file", ::strerror(errno));
    }

    return FilePageWriter {std::move(path), fd};
}

void FilePageWriter::write(const Byte* data, size_t size) {
    size_t toWrite = size;
    const Byte* curr = data;

    while (toWrite != 0) {
        if (_buffer.avail() == 0) {
            flush();
        }

        const size_t currentSize = std::min(_buffer.avail(), toWrite);
        _buffer.write(curr, currentSize);
        toWrite -= currentSize;
        curr += currentSize;
        _written += currentSize;
    }
}

void FilePageWriter::sync() {
    if (auto res = ::fsync(_fd); res != 0) {
        _error = FileError(_path.c_str(), "Could not sync file", ::strerror(errno));
    }
}

void FilePageWriter::finish() {
    if (_buffer.size() == 0) {
        return;
    }

    if (_buffer.size() != InternalBuffer::Capacity) {
        switch (_finishStrategy) {
            case FinishStrategy::Truncate: {
                flush();
                if (_error) {
                    return;
                }

                if (auto res = ::ftruncate(_fd, _written); res != 0) {
                    _error = FileError(_path.c_str(), "Could not truncate file", ::strerror(errno));
                }

                sync();
                return;
            }
            case FinishStrategy::DontCare: {
                break;
            }
            case FinishStrategy::FillZeros: {
                // Set remaining bytes to 0
                std::memset(_buffer.data() + _buffer.size(), 0, _buffer.avail());
                break;
            }
        }
    }

    flush();
    sync();
}

void FilePageWriter::flush() {
    size_t bytesWrote = 0;

    while (bytesWrote != PAGE_SIZE) {
        ssize_t nbytes = ::write(_fd, _buffer.data(), PAGE_SIZE);

        if (nbytes == -1) {
            if (errno == EAGAIN) {
                continue;
            }

            _error = FileError(_path.c_str(), "Could not write page", ::strerror(errno));
            _buffer.resize(0);
            return;
        }

        bytesWrote += nbytes;
    }

    _buffer.resize(0);
}
