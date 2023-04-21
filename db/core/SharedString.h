// Copyright 2023 Turing Biosystems Ltd.

#pragma once

#include <string>

namespace db {

class SharedString {
public:
    using ID = std::size_t;

    SharedString(ID strId, const std::string& str);
    ~SharedString();

    ID getID() const { return _id; }

    bool isEqual(const SharedString* other) const;

    const std::string& getString() const { return _str; }

private:
    const ID _id {0};
    const std::string _str;
};

}
