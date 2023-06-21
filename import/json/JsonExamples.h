#pragma once

#include <string>

namespace db {
class DB;
}

db::DB* getNeo4j4DB(const std::string& dbName);

db::DB* cyberSecurityDB();
db::DB* networkDB();
db::DB* poleDB();
db::DB* stackoverflowDB();
db::DB* recommendationsDB();

