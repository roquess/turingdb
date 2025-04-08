#pragma once

#include <vector>

#include "EntityID.h"
#include "labels/LabelSetHandle.h"
#include "types/PropertyType.h"

namespace db {

class CommitMetadata;
class MetadataBuilder;

class MetadataRebaser {
public:
    MetadataRebaser() = default;
    ~MetadataRebaser() = default;

    MetadataRebaser(const MetadataRebaser&) = delete;
    MetadataRebaser(MetadataRebaser&&) = delete;
    MetadataRebaser& operator=(const MetadataRebaser&) = delete;
    MetadataRebaser& operator=(MetadataRebaser&&) = delete;

    bool rebase(const CommitMetadata& theirs, MetadataBuilder& ours);

    LabelID getLabelMapping(LabelID id) const { return _labelMapping[id.getValue()]; }
    LabelSetHandle getLabelSetMapping(LabelSetID id) const { return _labelsetMapping[id.getValue()]; }
    EdgeTypeID getEdgeTypeMapping(EdgeTypeID id) const { return _edgeTypeMapping[id.getValue()]; }
    PropertyType getPropertyTypeMapping(PropertyTypeID id) const { return _propTypeMapping[id.getValue()]; }

private:
    std::vector<LabelID> _labelMapping;
    std::vector<LabelSetHandle> _labelsetMapping;
    std::vector<EdgeTypeID> _edgeTypeMapping;
    std::vector<PropertyType> _propTypeMapping;
};

}
