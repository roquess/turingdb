#include "Writeback.h"

#include "DB.h"
#include "Network.h"

using namespace db;

Writeback::Writeback(DB* db)
    : _db(db)
{
}

Writeback::~Writeback() {
}

Network* Writeback::createNetwork(StringRef name) {
    if (_db->getNetwork(name)) {
        return nullptr;
    }

    Network* net = new Network(name);
    _db->addNetwork(net);
    return net;
}
