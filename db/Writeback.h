#pragma once

#include "StringRef.h"

namespace db {

class DB;
class Network;

class Writeback {
public:
    Writeback(DB* db);
    ~Writeback();

    Network* createNetwork(StringRef name);

private:
    DB* _db {nullptr};
};

}
