#pragma once

#include <stddef.h>
#include <string>

namespace db {

class DBIndex {
public:
    using ID = size_t;

    explicit DBIndex(ID networkID, ID objectID)
        : _networkID(networkID), _objectID(objectID)
    {
    }

    ID getNetworkID() const { return _networkID; }
    ID getObjectID() const { return _objectID; }

    bool operator==(const DBIndex& other) const {
        if (_networkID != other._networkID) {
            return false;
        }

        return _objectID == other._objectID;
    }

    bool operator!=(const DBIndex& other) const {
        return !(*this == other);
    }

    bool operator<(const DBIndex& other) const {
        if (_networkID < other._networkID) {
            return true;
        }

        if (_networkID > other._networkID) {
            return false;
        }

        return _objectID < other._objectID;
    }

    std::string toString() const;

private:
    ID _networkID;
    ID _objectID;
};

}
