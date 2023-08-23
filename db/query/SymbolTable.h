#pragma once

#include "Symbol.h"

namespace db::query {

class SymbolTable {
public:
    SymbolTable();
    ~SymbolTable();

    Symbol createSymbol(const std::string& name);

private:
    std::unordered_map<std::string, size_t> _nameMap;
};

}
