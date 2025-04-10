#pragma once

#include <cstddef>

#include "EntityID.h"

namespace db {

class DataPart;
class MetadataRebaser;

class DataPartRebaser {
public:
    DataPartRebaser() = default;
    ~DataPartRebaser() = default;

    DataPartRebaser(const DataPartRebaser&) = delete;
    DataPartRebaser(DataPartRebaser&&) = delete;
    DataPartRebaser& operator=(const DataPartRebaser&) = delete;
    DataPartRebaser& operator=(DataPartRebaser&&) = delete;

    bool rebase(const MetadataRebaser& metadata,
                const DataPart& prevPart,
                DataPart& part);
private:
    EntityID _prevFirstNodeID {0};
    EntityID _prevFirstEdgeID {0};
    size_t _nodeOffset {0};
    size_t _edgeOffset {0};

    EntityID rebaseNodeID(const EntityID& id) const {
        return id >= _prevFirstEdgeID ? id + _nodeOffset : id;
    }

    EntityID rebaseEdgeID(const EntityID& id) const {
        return id >= _prevFirstEdgeID ? id + _edgeOffset : id;
    }
};

}
