#pragma once

#include <filesystem>
#include <string>

namespace db {
class DB;

class DBDumper {
public:
    using Path = std::filesystem::path;

    DBDumper(const DB* db, const Path& outDir);
    ~DBDumper();

    void setDBDirectoryName(const std::string& dirName);
    const std::string& getDirName() const { return _dbDirName; }

    bool dump();

    static std::string getDefaultDBDirectoryName();

private:
    const Path _outDir;
    std::string _dbDirName;
    const DB* _db{nullptr};
};

}
