#pragma once

#include <string_view>

namespace db {

class QueryParams {
public:
    QueryParams(std::string_view queryStr,
                std::string_view graphName)
        : _queryStr(queryStr),
        _graphName(graphName)
    {
    }

    std::string_view getQuery() const { return _queryStr; }
    std::string_view getGraphName() const { return _graphName; }

private:
    std::string_view _queryStr;
    std::string_view _graphName;
};

}
