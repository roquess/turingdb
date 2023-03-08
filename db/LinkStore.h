// Copyright 2023 Turing Biosystems Ltd.

#ifndef _DB_LINK_STORE_
#define _DB_LINK_STORE_

#include <vector>

namespace db {

class Link;

class LinkStore {
public:
    using Links = std::vector<Link*>;
    using LinkSlots = std::vector<Links>;

    LinkStore(std::size_t size);
    ~LinkStore();
    
    void addLink(Link* link, std::size_t index);

private:
    LinkSlots _linkSlots;
};

}

#endif
