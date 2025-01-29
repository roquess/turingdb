#pragma once

#include <string>

namespace db {

class ASTContext;

class NameConstraint {
public:
    friend ASTContext;

    static NameConstraint* create(ASTContext* ctxt, const std::string& name);

    const std::string& getName() const { return _name; }

private:
    const std::string _name;

    NameConstraint(const std::string& name);
    ~NameConstraint();
};

}
