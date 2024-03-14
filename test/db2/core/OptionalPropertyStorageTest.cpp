#include "optional/OptionalBools.h"
#include "optional/OptionalGenerics.h"
#include "optional/OptionalStrings.h"

#include <gtest/gtest.h>

using namespace db;

struct TestNode {
    EntityID _id = 0;
    LabelSet _labelset;
    std::optional<std::string> _stringProp;
    std::optional<bool> _boolProp;
    std::optional<int64_t> _intProp;
};

template <SupportedType T>
std::unique_ptr<OptionalGenerics<T>> buildStorage(const std::vector<TestNode>& nodes) {
    typename OptionalGenerics<T>::Builder builder;
    std::unordered_map<LabelSet, size_t> nodeCounts;

    builder.startBuilding(nodes.size());

    for (const auto& node : nodes) {
        nodeCounts[node._labelset]++;
    }

    for (const auto& [labelset, count] : nodeCounts) {
        builder.addNodeLabelSet(labelset, count);
    }

    for (const auto& node : nodes) {
        if constexpr (std::is_same_v<T, StringPropertyType>) {
            if (node._stringProp.has_value()) {
                builder.setNextProp(node._labelset, node._stringProp.value());
            }
        } else if constexpr (std::is_same_v<T, BoolPropertyType>) {
            if (node._boolProp.has_value()) {
                builder.setNextProp(node._labelset, node._boolProp.value());
            }
        } else if constexpr (std::is_same_v<T, Int64PropertyType>) {
            if (node._intProp.has_value()) {
                builder.setNextProp(node._labelset, node._intProp.value());
            }
        }
        builder.finishNode(node._labelset);
    }

    PropertyStorage* ptr = builder.build().release();
    return std::unique_ptr<OptionalGenerics<T>> {
        static_cast<OptionalGenerics<T>*>(ptr)};
}

TEST(OptionalPropertyStorageTest, Create) {
    std::vector<TestNode> nodes(1000);
    std::unordered_map<LabelSet, std::vector<std::optional<std::string>*>> stringDefs;
    std::unordered_map<LabelSet, std::vector<std::optional<bool>>> boolDefs;
    std::unordered_map<LabelSet, std::vector<std::optional<int64_t>>> integerDefs;

    for (size_t i = 0; i < nodes.size(); i++) {
        nodes[i] = {
            ._id = EntityID(i),
            ._labelset = {i % 3},
        };

        if (i % 6 == 0) {
            nodes[i]._stringProp = "ps" + std::to_string(i);
        }

        if (i % 7 == 0) {
            nodes[i]._boolProp = (bool)(i % 2);
        }

        if (i % 8 == 0) {
            nodes[i]._intProp = (int64_t)i;
        }

        stringDefs[nodes[i]._labelset].push_back(&nodes[i]._stringProp);
        boolDefs[nodes[i]._labelset].push_back(nodes[i]._boolProp);
        integerDefs[nodes[i]._labelset].push_back(nodes[i]._intProp);
    }

    auto strings = buildStorage<StringPropertyType>(nodes);

    for (const auto& [labelset, comparison] : stringDefs) {
        const auto s = strings->getSpanFromLabelSet(labelset);

        std::string output1;
        for (const auto& prop : s) {
            if (prop.has_value()) {
                output1 += prop.value() + " ";
            }
        }

        std::string output2;
        for (const auto& prop : comparison) {
            if (prop->has_value()) {
                output2 += prop->value() + " ";
            }
        }

        ASSERT_STREQ(output1.c_str(), output2.c_str());
    }

    auto bools = buildStorage<BoolPropertyType>(nodes);
    for (const auto& [labelset, comparison] : boolDefs) {
        const auto b = bools->getSpanFromLabelSet(labelset);

        std::string output1;
        for (const auto& prop : b) {
            if (prop.has_value()) {
                output1 += std::to_string(prop.value());
            }
        }

        std::string output2;
        for (const auto& prop : comparison) {
            if (prop.has_value()) {
                output2 += std::to_string(prop.value());
            }
        }

        ASSERT_STREQ(output1.c_str(), output2.c_str());
    }

    auto integers = buildStorage<Int64PropertyType>(nodes);
    for (const auto& [labelset, comparison] : integerDefs) {
        const auto b = integers->getSpanFromLabelSet(labelset);

        std::string output1;
        for (const auto& prop : b) {
            if (prop.has_value()) {
                output1 += std::to_string(prop.value());
            }
        }

        std::string output2;
        for (const auto& prop : comparison) {
            if (prop.has_value()) {
                output2 += std::to_string(prop.value());
            }
        }

        ASSERT_STREQ(output1.c_str(), output2.c_str());
    }
}
