#pragma once

namespace db {
class DBManager;
}

namespace db::query {

class InterpreterContext {
public:
    explicit InterpreterContext(DBManager* dbMan)
        : _dbMan(dbMan)
    {
    }

    DBManager* getDBManager() const { return _dbMan; }

private:
    DBManager* _dbMan {nullptr};
};

}
