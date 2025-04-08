#include "MetadataRebaser.h"

#include "CommitMetadata.h"
#include "writers/MetadataBuilder.h"

using namespace db;

bool MetadataRebaser::rebase(const CommitMetadata& theirs, MetadataBuilder& ours) {
    const auto& theirLabels = theirs.labels();
    const auto& theirLabelSets = theirs.labelsets();
    const auto& theirEdgeTypes = theirs.edgeTypes();
    const auto& theirPropTypes = theirs.propTypes();

    const auto& ourLabels = ours._metadata->labels();
    const auto& ourLabelsets = ours._metadata->labelsets();
    const auto& ourEdgeTypes = ours._metadata->edgeTypes();
    const auto& ourPropTypes = ours._metadata->propTypes();

    LabelMap newLabels = theirLabels;
    LabelSetMap newLabelsets = theirLabelSets;
    EdgeTypeMap newEdgeTypes = theirEdgeTypes;
    PropertyTypeMap newPropTypes = theirPropTypes;

    _labelMapping.resize(ourLabels.getCount());
    _labelsetMapping.resize(ourLabelsets.getCount());
    _edgeTypeMapping.resize(ourEdgeTypes.getCount());
    _propTypeMapping.resize(ourPropTypes.getCount());

    // Labels
    for (const auto& [ourID, name] : ourLabels) {
        const auto newID = newLabels.getOrCreate(*name);
        _labelMapping[ourID.getValue()] = newID;
    }

    // Edge types
    for (const auto& [ourID, name] : ourEdgeTypes) {
        const auto newID = newEdgeTypes.getOrCreate(*name);
        _edgeTypeMapping[ourID.getValue()] = newID;
    }

    // Property types
    for (const auto& [ourPT, name] : ourPropTypes) {
        auto newPT = newPropTypes.getOrCreate(*name, ourPT._valueType);
        if (newPT._valueType != ourPT._valueType) {
            // Property type already exists, but with different value type
            // Creating a new property type with name = 'prevName (valueType)'
            const auto newName = fmt::format("{} ({})", *name, ValueTypeName::value(ourPT._valueType));
            newPT = newPropTypes.getOrCreate(newName, ourPT._valueType);
        }

        _propTypeMapping[ourPT._id.getValue()] = newPT;
    }

    // Labelsets
    LabelSet newLabelset;
    for (const auto& [prevID, prevValue] : ourLabelsets) {
        // Build labelset with patched IDs
        newLabelset = LabelSet {};

        for (LabelID a = 0; a < _labelMapping.size(); a++) {
            const LabelID b = _labelMapping[a.getValue()];
            if (prevValue->hasLabel(a)) {
                newLabelset.set(b);
            }
        }


        const auto newHandle = newLabelsets.getOrCreate(newLabelset);
        _labelsetMapping[prevID.getValue()] = newHandle;
    }


    ours._metadata->_labelMap = std::move(newLabels);
    ours._metadata->_labelsetMap = std::move(newLabelsets);
    ours._metadata->_edgeTypeMap = std::move(newEdgeTypes);
    ours._metadata->_propTypeMap = std::move(newPropTypes);

    return true;
}
