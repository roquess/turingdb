#include "FilePageReader.h"

#include <fcntl.h>

using namespace fs;

FileResult<FilePageReader> FilePageReader::open(Path path) {
    const int access = O_RDONLY | O_DIRECT;
    const int permissions = S_IRUSR | S_IWUSR;

    const int fd = ::open(path.c_str(), access, permissions);

    FilePageReader reader = {std::move(path), -1};
    if (fd == -1) {
        return FileError::result(path.c_str(),
                                 "Could not open file",
                                 ::strerror(errno));
    }

    reader._fd = fd;

    return reader;
}

FileResult<FilePageReader> FilePageReader::openNoDirect(Path path) {
    const int access = O_RDONLY;
    const int permissions = S_IRUSR | S_IWUSR;

    const int fd = ::open(path.c_str(), access, permissions);

    FilePageReader reader = {std::move(path), -1};
    if (fd == -1) {
        return FileError::result(path.c_str(),
                                 "Could not open file",
                                 ::strerror(errno));
    }

    reader._fd = fd;

    return reader;
}

FileResult<void> FilePageReader::nextPage() {
    size_t bytesRead = 0;

    while (bytesRead != PAGE_SIZE) {
        ssize_t nbytes = ::read(_fd, _buffer.data(), PAGE_SIZE);

        if (nbytes == -1) {
            if (errno == EAGAIN) {
                continue;
            }

            return FileError::result(_path.c_str(),
                                     "Could not read page",
                                     ::strerror(errno));
        }

        if (nbytes == 0) {
            _reachedEnd = true;
            break; // Finished reading file
        }

        bytesRead += nbytes;
    }

    _buffer.resize(bytesRead);
    return {};
}
