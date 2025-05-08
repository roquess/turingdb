#include "S3Result.h"

namespace S3 {

std::string Error::fmtMessage() const {
             return fmt::format("Turing S3Client error: {}",
                           ErrorTypeDescription::value(_type));
}

}

