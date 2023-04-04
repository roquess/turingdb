#ifndef _DB_COMPONENT_COLUMN_
#define _DB_COMPONENT_COLUMN_

#include <vector>

namespace db {

class Component;

class ComponentColumn {
public:
    ComponentColumn();
    ~ComponentColumn();

private:
    std::vector<Component*> _components;
};

}

#endif
