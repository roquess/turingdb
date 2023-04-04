// Copyright 2023 Turing Biosystems Ltd.

#ifndef _DB_EDGE_
#define _DB_EDGE_

namespace db {

class Component;
class DB;

class Edge {
public:
    Component* getSource() const { return _source; }
    Component* getTarget() const { return _target; }

private:
    Component* _source {nullptr};
    Component* _target {nullptr};

    Edge(Component* source, Component* target);
    ~Edge();
};

}

#endif
