#include "Comparator.h"
#include "DB.h"
#include "DBEntityType.h"
#include "Edge.h"
#include "EdgeMap.h"
#include "EdgeType.h"
#include "Network.h"
#include "Node.h"
#include "NodeType.h"
#include "PropertyType.h"
#include "Value.h"

namespace db {

template <typename T, typename U>
    requires std::same_as<T, std::map<StringRef, U*>>
static inline bool compareStringMappedContainers(const T* c1, const T* c2) {
    if (c1->size() != c2->size()) {
        return false;
    }

    auto it1 = c1->cbegin();
    auto it2 = c2->cbegin();

    while (it1 != c1->cend()) {
        if (!StringRef::same(it1->first, it2->first)) {
            return false;
        }

        if (!Comparator<U>::same(it1->second, it2->second)) {
            return false;
        }

        ++it1;
        ++it2;
    }

    return true;
}

template <typename T, typename U>
    requires std::same_as<T, std::vector<U*>>
          || std::same_as<T, std::set<U*, DBObject::Sorter>>
static inline bool compareVectorContainers(const T* c1, const T* c2) {
    if (c1->size() != c2->size()) {
        return false;
    }

    auto it1 = c1->cbegin();
    auto it2 = c2->cbegin();

    while (it1 != c1->cend()) {
        if (!Comparator<U>::same(*it1, *it2)) {
            return false;
        }

        ++it1;
        ++it2;
    }

    return true;
}

template <>
bool Comparator<DBEntityType::PropertyTypes>::same(const DBEntityType::PropertyTypes* propTypes1,
                                                   const DBEntityType::PropertyTypes* propTypes2) {
    return compareStringMappedContainers<DBEntityType::PropertyTypes, PropertyType>(
        propTypes1, propTypes2);
}

template <>
bool Comparator<DB::Networks>::same(const DB::Networks* nets1,
                                    const DB::Networks* nets2) {
    return compareStringMappedContainers<DB::Networks, Network>(nets1, nets2);
}

template <>
bool Comparator<DB::NodeTypes>::same(const DB::NodeTypes* nt1,
                                     const DB::NodeTypes* nt2) {
    return compareStringMappedContainers<DB::NodeTypes, NodeType>(nt1, nt2);
}

template <>
bool Comparator<DB::EdgeTypes>::same(const DB::EdgeTypes* et1,
                                     const DB::EdgeTypes* et2) {
    return compareStringMappedContainers<DB::EdgeTypes, EdgeType>(et1, et2);
}

template <>
bool Comparator<Network::Nodes>::same(const Network::Nodes* nodes1,
                                      const Network::Nodes* nodes2) {
    return compareVectorContainers<Network::Nodes, Node>(nodes1, nodes2);
}

template <>
bool Comparator<Network::Edges>::same(const Network::Edges* edges1, const Network::Edges* edges2) {
    return compareVectorContainers<Network::Edges, Edge>(edges1, edges2);
}

template <>
bool Comparator<EdgeType::NodeTypes>::same(const EdgeType::NodeTypes* nt1,
                                           const EdgeType::NodeTypes* nt2) {
    return compareVectorContainers<EdgeType::NodeTypes, NodeType>(nt1, nt2);
}

template <>
bool Comparator<NodeType::EdgeTypes>::same(const NodeType::EdgeTypes* et1,
                                           const NodeType::EdgeTypes* et2) {
    return compareVectorContainers<NodeType::EdgeTypes, EdgeType>(et1, et2);
}

template <>
bool Comparator<DBEntity::Properties>::same(const DBEntity::Properties* props1,
                                            const DBEntity::Properties* props2) {
    if (props1->size() != props2->size()) {
        return false;
    }

    auto it1 = props1->cbegin();
    auto it2 = props2->cbegin();

    while (it1 != props1->cend()) {
        if (!Comparator<PropertyType>::same(it1->first, it2->first)) {
            return false;
        }

        if (!Comparator<Value>::same(&it1->second, &it2->second)) {
            return false;
        }

        ++it1;
        ++it2;
    }

    return true;
}

template <>
bool Comparator<EdgeMap>::same(const EdgeMap* edges1, const EdgeMap* edges2) {
    auto inIterator1 = edges1->_edges.cbegin();
    auto inIterator2 = edges2->_edges.cbegin();

    while (inIterator1 != edges1->_edges.cend()) {
        if (!Comparator<Network::Edges>::same(&inIterator1->second, &inIterator2->second)) {
            return false;
        }

        ++inIterator1;
        ++inIterator2;
    }

    return true;
}

}
