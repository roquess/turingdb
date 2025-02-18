#pragma once

namespace db {

class TuringDB;

class TuringServer {
public:
    TuringServer(TuringDB& db);
    ~TuringServer();

    bool start();
    
private:
    TuringDB& _db;
};

}
