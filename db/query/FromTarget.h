#pragma once

#include <vector>

namespace db {

class ASTContext;
class PathPattern;

class FromTarget {
public:
    friend ASTContext;
    using Patterns = std::vector<PathPattern*>;

    static FromTarget* create(ASTContext* ctxt);

    const Patterns& patterns() const { return _patterns; }

    void addPathPattern(PathPattern* pattern);

private:
    Patterns _patterns;

    FromTarget();
    ~FromTarget();
};

}
