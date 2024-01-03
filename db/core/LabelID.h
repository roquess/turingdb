#pragma once

#include <stdint.h>

namespace db {

class LabelID {
public:
    LabelID() = default;

    LabelID(uint64_t id)
        : _id(id)
    {
    }

    bool operator==(const LabelID& other) const { return _id == other._id; }
    bool operator!=(const LabelID& other) const { return !(_id == other._id); }
    bool operator<(const LabelID& other) const { return _id < other._id; }

    uint64_t getID() const { return _id; }

private:
    uint64_t _id {0};
};

}