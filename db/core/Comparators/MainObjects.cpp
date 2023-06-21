#include "Comparator.h"
#include "DB.h"
#include "Network.h"
#include "Property.h"
#include "Value.h"

namespace db {

template <>
bool Comparator<DB>::same(const DB* db1, const DB* db2) {
    return Comparator<DB::NodeTypes>::same(&db1->_nodeTypes, &db2->_nodeTypes)
        && Comparator<DB::EdgeTypes>::same(&db1->_edgeTypes, &db2->_edgeTypes)
        && Comparator<DB::Networks>::same(&db1->_networks, &db2->_networks);
}

template <>
bool Comparator<Network>::same(const Network* net1, const Network* net2) {
    return Comparator<DBObject>::same(net1, net2)
        && StringRef::same(net1->_name, net2->_name)
        && Comparator<Network::Nodes>::same(&net1->_nodes, &net2->_nodes)
        && Comparator<Network::Edges>::same(&net1->_edges, &net2->_edges);
}

template <>
bool Comparator<Value>::same(const Value* vt1, const Value* vt2) {
    return Comparator<ValueType>::same(&vt1->_type, &vt2->_type)
        && vt1->_data == vt2->_data;
}

template <>
bool Comparator<Property>::same(const Property* p1, const Property* p2) {
    return Comparator<PropertyType>::same(p1->_type, p2->_type)
        && Comparator<Value>::same(&p1->_value, &p2->_value);
}

}
