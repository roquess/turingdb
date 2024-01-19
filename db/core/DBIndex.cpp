#include "DBIndex.h"

using namespace db;

std::string DBIndex::toString() const {
    return std::to_string(_objectID);
}
