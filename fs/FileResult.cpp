#include "FileResult.h"

namespace fs {

std::string Error::fmtMessage() const {
    return _errno == -1
             ? fmt::format("Filesystem error: {}",
                           ErrorTypeDescription::value(_type))
             : fmt::format("Filesystem error: {} ({})",
                           ErrorTypeDescription::value(_type),
                           strerror(_errno));
}

std::string FileError::fmtMessage() const {
    return _errno == -1
             ? fmt::format("File '{}' error: {}",
                           _path.c_str(),
                           ErrorTypeDescription::value(_type))
             : fmt::format("File '{}' error: {} ({})",
                           _path.c_str(),
                           ErrorTypeDescription::value(_type),
                           strerror(_errno));
}

}
