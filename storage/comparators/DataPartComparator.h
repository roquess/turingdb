#pragma once

namespace db {

class DataPart;

class DataPartComparator {
public:
    [[nodiscard]] static bool same(const DataPart& a, const DataPart& b);
};

}
