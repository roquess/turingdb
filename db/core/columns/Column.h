#pragma once

namespace db {

template <typename T>
class Column {
public:
    Column() = default;
    virtual ~Column() = default;

    Column(const Column&) = delete;
    Column(Column&&) = delete;
    Column& operator=(const Column&) = delete;
    Column& operator=(Column&&) = delete;

    virtual bool empty() const = 0;
    virtual size_t size() const = 0;

    virtual const T& get(size_t pos) const = 0;
};

}