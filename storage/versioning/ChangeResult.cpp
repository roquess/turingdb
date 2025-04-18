#include "ChangeResult.h"

using namespace db;

std::string ChangeError::fmtMessage() const {
    if (_commitError) {
        return fmt::format("ChangeError: {} ({})",
                           ChangeErrorTypeDescription::value(_type),
                           CommitErrorTypeDescription::value(_commitError->getType()));
    }

    return std::string {ChangeErrorTypeDescription::value(_type)};
}

