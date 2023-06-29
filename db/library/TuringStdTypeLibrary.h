#pragma once

#include "TypeLibrary.h"

namespace db {

class TuringStdTypeLibrary : public TypeLibrary {
public:
    TuringStdTypeLibrary(DB* db);
    ~TuringStdTypeLibrary() override = default;
    virtual bool load() override;
};

}
