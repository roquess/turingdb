#pragma once

#include <map>
#include <string>
#include <shared_mutex>
#include <vector>

namespace db {

class DB;

class DBManager {
public:
    DBManager(const std::string& dbDirPath);
    ~DBManager();

    void getDatabases(std::vector<std::string>& databases) const;

    bool loadDB(const std::string& dbName);

private:
    mutable std::shared_mutex _lock;
    const std::string _dbDirPath;
    std::map<std::string, DB*> _databases;
};

}
