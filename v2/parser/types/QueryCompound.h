#pragma once

namespace db {

class QueryCompound {
public:
    QueryCompound() = default;
    virtual ~QueryCompound() = 0;

    QueryCompound(const QueryCompound&) = delete;
    QueryCompound(QueryCompound&&) = delete;
    QueryCompound& operator=(const QueryCompound&) = delete;
    QueryCompound& operator=(QueryCompound&&) = delete;
};

inline QueryCompound::~QueryCompound() = default;

}
