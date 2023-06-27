#pragma once

#include "FileUtils.h"

#include <vector>

namespace db {

class DB;
class StringIndexLoader;

class EntityLoader{
public:
    EntityLoader(db::DB* db, const FileUtils::Path& dbPath);

    bool load(const StringIndexLoader& strLoader);

private:
    FileUtils::Path _indexPath;
    db::DB* _db{nullptr};
};

}
