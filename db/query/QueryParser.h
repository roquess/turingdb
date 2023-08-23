#pragma once

#include <string>

namespace db::query {

class QueryCommand;

class QueryParser {
public:
    QueryParser();

    QueryCommand* parse(const std::string& queryStr);
};

}
