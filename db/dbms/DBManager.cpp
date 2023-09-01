#include "DBManager.h"

#include <mutex>

#include "DB.h"
#include "DBLoader.h"
#include "FileUtils.h"

using namespace db;

DBManager::DBManager(const std::string& dbDirPath)
    : _dbDirPath(dbDirPath)
{
}

DBManager::~DBManager() {
}

void DBManager::getDatabases(std::vector<std::string>& databases) const {
    std::shared_lock rd(_lock);
    for (const auto& entry : _databases) {
        databases.emplace_back(entry.first);
    }
}

bool DBManager::loadDB(const std::string& dbName) {
    std::unique_lock wr(_lock);

    if (dbName.empty()) {
        return false;
    }

    // Search if the db is already loaded
    const auto it = _databases.find(dbName);
    if (it != _databases.end()) {
        return false;
    }

    // Load the database from disk
    DB* db = db::DB::create();
    DBLoader loader(db, FileUtils::Path(_dbDirPath)/dbName);
    if (!loader.load()) {
        return false;
    }

    _databases.emplace(dbName, db);

    return true;
}
