#include "PropertyContainer.h"

#include <gtest/gtest.h>

using namespace db;
using namespace std;
using namespace chrono;

using PropertyValue = std::variant<std::string, double, bool>;
using Property = std::pair<PropertyTypeID, PropertyValue>;

struct TestNode {
    EntityID _id;
    LabelSet _labelset;
};

TEST(PropertyContainerTest, Create) {
    using Nodes = std::map<EntityID, TestNode>;
    using StringProps = std::unordered_map<PropertyTypeID, std::map<EntityID, std::string>>;
    using BoolProps = std::unordered_map<PropertyTypeID, std::map<EntityID, bool>>;

    Nodes nodes;
    StringProps stringProps;
    BoolProps boolProps;

    std::unordered_map<LabelSet, size_t> nodeCounts;
    PropertyContainer::Builder builder;

    LabelID currentLabelID = 0;
    PropertyTypeID currentPropTypeID = 0;

    // Labelsets
    LabelID protein = currentLabelID++;
    LabelID reaction = currentLabelID++;

    // Boolean properties
    PropertyTypeID isProtein = currentPropTypeID++;
    PropertyTypeID isReaction = currentPropTypeID++;

    // String properties
    PropertyTypeID name = currentPropTypeID++;
    PropertyTypeID dbID = currentPropTypeID++;

    EntityID currentNodeID = 0;

    // Proteins
    {
        auto& n = nodes[currentNodeID];
        n._id = currentNodeID++;
        n._labelset = {protein};
        boolProps[isProtein].emplace(n._id, true);
        boolProps[isReaction].emplace(n._id, false);
        stringProps[name].emplace(n._id, "Apolipoprotein E");
        stringProps[dbID].emplace(n._id, "P02649");
    }

    {
        auto& n = nodes[currentNodeID];
        n._id = currentNodeID++;
        n._labelset = {protein};
        boolProps[isProtein].emplace(n._id, true);
        boolProps[isReaction].emplace(n._id, false);
        stringProps[name].emplace(n._id, "Insulin receptor");
        stringProps[dbID].emplace(n._id, "P06213");
    }

    {
        auto& n = nodes[currentNodeID];
        n._id = currentNodeID++;
        n._labelset = {protein};
        boolProps[isProtein].emplace(n._id, true);
        boolProps[isReaction].emplace(n._id, false);
        stringProps[name].emplace(n._id, "Espin");
        stringProps[dbID].emplace(n._id, "B1AK53");
    }

    // Reactions
    {
        auto& n = nodes[currentNodeID];
        n._id = currentNodeID++;
        n._labelset = {reaction};
        boolProps[isProtein].emplace(n._id, false);
        boolProps[isReaction].emplace(n._id, true);
        stringProps[name].emplace(n._id, "Formation of serum amyloid P decamer");
        stringProps[dbID].emplace(n._id, "1930689");
    }

    {
        auto& n = nodes[currentNodeID];
        n._id = currentNodeID++;
        n._labelset = {reaction};
        boolProps[isProtein].emplace(n._id, false);
        boolProps[isReaction].emplace(n._id, true);
        stringProps[name].emplace(n._id, "NAT8,8B acetylate BACE1");
        stringProps[dbID].emplace(n._id, "1930512");
    }

    builder.setNodeCount(nodes.size());
    builder.addPropertyType<PropType::Bool>(isProtein, boolProps.at(isProtein).size());
    builder.addPropertyType<PropType::Bool>(isReaction, boolProps.at(isReaction).size());
    builder.addPropertyType<PropType::String>(name, stringProps.at(name).size());
    builder.addPropertyType<PropType::String>(dbID, stringProps.at(dbID).size());
    builder.startBuilding();

    for (const auto& [id, node] : nodes) {
        nodeCounts[node._labelset]++;
    }

    for (const auto& [labelset, count] : nodeCounts) {
        builder.addNodeLabelSet(labelset, count);
    }

    for (const auto& [nodeID, n] : nodes) {
        for (const auto& [ptID, props] : boolProps) {
            auto it = props.find(nodeID);

            if (it == props.end()) {
                continue;
            }

            const bool prop = it->second;
            builder.setNextProp<PropType::Bool>(ptID, n._labelset, prop);
        }

        for (const auto& [ptID, props] : stringProps) {
            auto it = props.find(nodeID);

            if (it == props.end()) {
                continue;
            }

            const std::string& prop = it->second;
            builder.setNextProp<PropType::String>(ptID, n._labelset, prop);
        }
        builder.finishNode(n._labelset);
    }

    std::unique_ptr<PropertyContainer> container = builder.build();

    {
        auto strings = container->getLabelSetMandatoryProperties<PropType::String>(name, {protein});
        std::string output = "[";
        for (const auto& str : strings) {
            output += str + ";";
        }
        output = output.substr(0, output.size() - 1) + "]";
        std::cout << output << std::endl;
        ASSERT_STREQ("[Apolipoprotein E;Insulin receptor;Espin]",
                     output.c_str());
    }

    {
        auto strings = container->getLabelSetMandatoryProperties<PropType::String>(name, {reaction});
        std::string output = "[";
        for (const auto& str : strings) {
            output += str + ";";
        }
        output = output.substr(0, output.size() - 1) + "]";
        std::cout << output << std::endl;
        ASSERT_STREQ("[Formation of serum amyloid P decamer;NAT8,8B acetylate BACE1]",
                     output.c_str());
    }

    {
        auto strings = container->getAllMandatoryProperties<PropType::String>(dbID);
        std::string output = "[";
        for (const auto& str : strings) {
            output += str + ";";
        }
        output = output.substr(0, output.size() - 1) + "]";
        std::cout << output << std::endl;
        ASSERT_STREQ("[1930689;1930512;P02649;P06213;B1AK53]",
                     output.c_str());
    }
}
