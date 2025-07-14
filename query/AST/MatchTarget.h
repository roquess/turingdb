#pragma once

#include "PathPattern.h"

namespace db {

class ASTContext;
class PathPattern;

class MatchTarget {
public:
    friend ASTContext;

    static MatchTarget* create(ASTContext* ctxt, PathPattern* pattern = nullptr);

    PathPattern* getPattern() const { return _pattern; }

private:
    PathPattern* _pattern {nullptr};

    explicit MatchTarget(PathPattern* pattern = nullptr);
    ~MatchTarget();
};

}
