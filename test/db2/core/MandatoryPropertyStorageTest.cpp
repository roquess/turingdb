#include "mandatory/MandatoryBools.h"
#include "mandatory/MandatoryGenerics.h"
#include "mandatory/MandatoryStrings.h"

#include <gtest/gtest.h>
#include <iostream>

using namespace db;

struct TestNode {
    EntityID _id = 0;
    LabelSet _labelset;
    std::string _stringProp;
    bool _boolProp;
    int64_t _intProp;
};

template <typename T>
std::unique_ptr<MandatoryGenerics<T>> buildStorage(const std::vector<TestNode>& nodes) {
    typename MandatoryGenerics<T>::Builder builder;
    std::unordered_map<LabelSet, size_t> nodeCounts;

    builder.startBuilding(nodes.size());

    for (const auto& node : nodes) {
        nodeCounts[node._labelset]++;
    }

    for (const auto& [labelset, count] : nodeCounts) {
        builder.addNodeLabelSet(labelset, count);
    }

    for (const auto& node : nodes) {
        if constexpr (std::is_same_v<T, PropType::String>) {
            builder.setNextProp(node._labelset, node._stringProp);
        } else if constexpr (std::is_same_v<T, PropType::Bool>) {
            builder.setNextProp(node._labelset, node._boolProp);
        } else if constexpr (std::is_same_v<T, PropType::Int64>) {
            builder.setNextProp(node._labelset, node._intProp);
        }
        builder.finishNode(node._labelset);
    }

    PropertyStorage* ptr = builder.build().release();
    return std::unique_ptr<MandatoryGenerics<T>> {
        static_cast<MandatoryGenerics<T>*>(ptr)};
}

TEST(MandatoryPropertyStorageTest, Create) {
    std::vector<TestNode> nodes(1000);
    std::unordered_map<LabelSet, std::vector<std::string*>> stringDefs;
    std::unordered_map<LabelSet, std::vector<bool>> boolDefs;
    std::unordered_map<LabelSet, std::vector<int64_t>> integerDefs;

    for (size_t i = 0; i < nodes.size(); i++) {
        nodes[i] = {
            ._id = EntityID(i),
            ._labelset = {i % 3},
        };

        stringDefs[nodes[i]._labelset].push_back(&nodes[i]._stringProp);
        boolDefs[nodes[i]._labelset].push_back(nodes[i]._boolProp);
        integerDefs[nodes[i]._labelset].push_back(nodes[i]._intProp);
    }

    auto strings = buildStorage<PropType::String>(nodes);

    for (const auto& [labelset, comparison] : stringDefs) {
        const auto s = strings->getSpanFromLabelSet(labelset);

        std::string output1;
        for (const auto& prop : s) {
            output1 += prop + " ";
        }

        std::string output2;
        for (const auto& prop : comparison) {
            output2 += *prop + " ";
        }

        ASSERT_STREQ(output1.c_str(), output2.c_str());
    }

    auto bools = buildStorage<PropType::Bool>(nodes);
    for (const auto& [labelset, comparison] : boolDefs) {
        const auto b = bools->getSpanFromLabelSet(labelset);

        std::string output1;
        for (const auto& prop : b) {
            output1 += std::to_string(prop.v);
        }

        std::string output2;
        for (const auto& prop : comparison) {
            output2 += std::to_string(prop);
        }

        ASSERT_STREQ(output1.c_str(), output2.c_str());
    }

    auto integers = buildStorage<PropType::Int64>(nodes);
    for (const auto& [labelset, comparison] : integerDefs) {
        const auto b = integers->getSpanFromLabelSet(labelset);

        std::string output1;
        for (const auto& prop : b) {
            output1 += std::to_string(prop);
        }

        std::string output2;
        for (const auto& prop : comparison) {
            output2 += std::to_string(prop);
        }

        ASSERT_STREQ(output1.c_str(), output2.c_str());
    }
}
