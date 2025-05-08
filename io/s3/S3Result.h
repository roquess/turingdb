#pragma once

#include "BasicResult.h"
#include "EnumToString.h"

#include <spdlog/fmt/bundled/format.h>

namespace S3 {

class Path;

enum class ErrorType : uint8_t {
    UNKNOWN = 0,
    CANNOT_UPLOAD_FILE,
    CANNOT_DOWNLOAD_FILE,
    CANNOT_UPLOAD_DIRECTORY,
    CANNOT_DOWNLOAD_DIRECTORY,
    CANNOT_OPEN_FILE,
    INVALID_DIRECTORY_NAME,
    INVALID_KEY_NAME,
    INVALID_BUCKET_NAME,
    _SIZE,
};

using ErrorTypeDescription = EnumToString<ErrorType>::Create<
    EnumStringPair<ErrorType::UNKNOWN, "Unknown">,
    EnumStringPair<ErrorType::CANNOT_UPLOAD_FILE, "Could not upload file">,
    EnumStringPair<ErrorType::CANNOT_DOWNLOAD_FILE, "Could not download file">,
    EnumStringPair<ErrorType::CANNOT_UPLOAD_DIRECTORY, "Could not upload directory">,
    EnumStringPair<ErrorType::CANNOT_DOWNLOAD_DIRECTORY, "Could not download directory">,
    EnumStringPair<ErrorType::CANNOT_OPEN_FILE, "Could not open file for read/write">,
    EnumStringPair<ErrorType::INVALID_DIRECTORY_NAME, "Could not find objects with the appropriate directory prefix">,
    EnumStringPair<ErrorType::INVALID_KEY_NAME, "Could not find an object mapped to the provided key">,
    EnumStringPair<ErrorType::INVALID_BUCKET_NAME, "Could not find a bucket with the provided name">>;

class Error {
public:
    explicit Error(ErrorType type)
        : _type(type)
    {
    }

    [[nodiscard]] ErrorType getType() const { return _type; }
    [[nodiscard]] std::string fmtMessage() const;

    template <typename... T>
    static BadResult<Error> result(ErrorType type) {
        return BadResult<Error>(Error(type));
    }

private:
    ErrorType _type {};
};

template <typename T>
using Result = BasicResult<T, class Error>;

}

