// Copyright 2023 Turing Biosystems Ltd.

#pragma once

#include "StringRef.h"

namespace db {

class Writeback;

class Network {
public:
    friend Writeback;

    StringRef getName() const { return _name; }

private:
    StringRef _name;

    Network(StringRef name);
    ~Network();
};

}
