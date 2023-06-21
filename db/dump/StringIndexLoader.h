#pragma once

#include "StringRef.h"

#include <filesystem>
#include <vector>

namespace db {

class StringIndex;

class StringIndexLoader {
public:
    using Path = std::filesystem::path;

    StringIndexLoader(const Path& dbPath);

    bool load(StringIndex& index);

    StringRef operator[](size_t id) const {
        return _stringIdMapping[id];
    }

private:
    Path _indexPath;
    std::vector<StringRef> _stringIdMapping;
};

}
