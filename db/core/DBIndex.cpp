#include "DBIndex.h"

using namespace db;

std::string DBIndex::toString() const {
    std::string str = "(";
    str += std::to_string(_networkID);
    str += ", ";
    str += std::to_string(_objectID);
    str += ")";
    return str;
}
