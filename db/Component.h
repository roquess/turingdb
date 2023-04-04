#ifndef _DB_COMPONENT_
#define _DB_COMPONENT_

#include <vector>

#include "Value.h"

namespace db {

class Edge;

class Component {
public:
private:
    std::vector<Value> _properties;
    std::vector<std::vector<Edge*>> _inEdges;
    std::vector<std::vector<Edge*>> _outEdges;

    Component();
    ~Component();
};

}

#endif
