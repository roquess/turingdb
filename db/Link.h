// Copyright 2023 Turing Biosystems Ltd.

#ifndef _DB_LINK_
#define _DB_LINK_

namespace db {

class Object;
class LinkType;
class DB;

class Link {
public:
    friend DB;

    LinkType* getType() const { return _type; }

    Object* getSource() const { return _source; }
    Object* getTarget() const { return _target; }

    static Link* create(DB* db,
                        LinkType* type,
                        Object* source,
                        Object* target);

private:
    LinkType* _type {nullptr};
    Object* _source {nullptr};
    Object* _target {nullptr};

    Link(LinkType* type, Object* source, Object* target);
    ~Link();
};

}

#endif
