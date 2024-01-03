#pragma once

#include <vector>

#include "Column.h"

namespace db {

template <typename T>
class ColumnVector : public Column {
public:
    ColumnVector();
    explicit ColumnVector(size_t size);
    ~ColumnVector();

    bool empty() const override { return _data.empty(); }
    size_t size() const override { return _data.size(); }

    inline const T& get(size_t pos) const override { return _data[pos]; }
};

private:
    std::vector<T> _data;
};

}