#include "PropertyContainer.h"

#include <gtest/gtest.h>

using namespace db;
using namespace std;
using namespace chrono;

using PropertyValue = std::variant<std::string, double, bool>;
using Property = std::pair<PropertyTypeID, PropertyValue>;

static const unordered_map<PropertyTypeID, size_t> propertyTypes = {
    {0, MandatoryStorageBuilder::StringIndex},
    {1, MandatoryStorageBuilder::DoubleIndex},
    {2, MandatoryStorageBuilder::BoolIndex  },
};

struct TestNode {
    EntityID _id;
    LabelSet _labelset;
};

TEST(PropertyContainerTest, Create) {
    using Nodes = std::map<EntityID, TestNode>;
    using PropValues = std::unordered_map<PropertyTypeID, std::map<EntityID, PropertyValue>>;

    Nodes nodes;
    PropValues properties;
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

    constexpr auto setStringProperties = [](PropertyContainer::Builder& builder,
                                            PropertyTypeID ptID,
                                            PropValues& properties,
                                            Nodes& nodes) {
        for (const auto& [eID, prop] : properties[ptID]) {
            const PropType::String::ValueType& p = std::get<std::string>(prop);
            builder.setNextProp<PropType::String>(ptID, nodes.at(eID)._labelset, p);
        }
    };

    constexpr auto setBoolProperties = [](PropertyContainer::Builder& builder,
                                          PropertyTypeID ptID,
                                          PropValues& properties,
                                          Nodes& nodes) {
        for (const auto& [eID, prop] : properties[ptID]) {
            bool p = std::get<bool>(prop);
            builder.setNextProp<PropType::Bool>(ptID, nodes.at(eID)._labelset, p);
        }
    };

    EntityID currentNodeID = 0;

    // Proteins
    {
        auto& n = nodes[currentNodeID];
        n._id = currentNodeID++;
        n._labelset = {protein};
        properties[isProtein].emplace(n._id, true);
        properties[isReaction].emplace(n._id, false);
        properties[name].emplace(n._id, "Apolipoprotein E");
        properties[dbID].emplace(n._id, "P02649");
    }

    {
        auto& n = nodes[currentNodeID];
        n._id = currentNodeID++;
        n._labelset = {protein};
        properties[isProtein].emplace(n._id, true);
        properties[isReaction].emplace(n._id, false);
        properties[name].emplace(n._id, "Insulin receptor");
        properties[dbID].emplace(n._id, "P06213");
    }

    {
        auto& n = nodes[currentNodeID];
        n._id = currentNodeID++;
        n._labelset = {protein};
        properties[isProtein].emplace(n._id, true);
        properties[isReaction].emplace(n._id, false);
        properties[name].emplace(n._id, "Espin");
        properties[dbID].emplace(n._id, "B1AK53");
    }

    // Reactions
    {
        auto& n = nodes[currentNodeID];
        n._id = currentNodeID++;
        n._labelset = {reaction};
        properties[isProtein].emplace(n._id, false);
        properties[isReaction].emplace(n._id, true);
        properties[name].emplace(n._id, "Formation of serum amyloid P decamer");
        properties[dbID].emplace(n._id, "1930689");
    }

    {
        auto& n = nodes[currentNodeID];
        n._id = currentNodeID++;
        n._labelset = {reaction};
        properties[isProtein].emplace(n._id, false);
        properties[isReaction].emplace(n._id, true);
        properties[name].emplace(n._id, "NAT8,8B acetylate BACE1");
        properties[dbID].emplace(n._id, "1930512");
    }

    builder.addPropertyType<PropType::Bool>(isProtein, PropertyImportance::Mandatory);
    builder.addPropertyType<PropType::Bool>(isReaction, PropertyImportance::Mandatory);
    builder.addPropertyType<PropType::String>(name, PropertyImportance::Mandatory);
    builder.addPropertyType<PropType::String>(dbID, PropertyImportance::Mandatory);
    builder.startBuilding(nodes.size());

    for (const auto& [id, node] : nodes) {
        nodeCounts[node._labelset]++;
    }

    for (const auto& [labelset, count] : nodeCounts) {
        builder.addNodeLabelSet(labelset, count);
    }

    setBoolProperties(builder, isProtein, properties, nodes);
    setBoolProperties(builder, isReaction, properties, nodes);
    setStringProperties(builder, name, properties, nodes);
    setStringProperties(builder, dbID, properties, nodes);

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
