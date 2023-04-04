#ifndef _DB_COMPONENT_STORAGE_
#define _DB_COMPONENT_STORAGE_

#include <vector>

namespace db {

class ComponentColumn;

class ComponentStorage {
public:
    ComponentStorage();
    ~ComponentStorage();

private:
    std::vector<ComponentColumn*> _columns;
};

}

#endif
