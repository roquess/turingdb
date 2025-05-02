#pragma once

#include <vector>

namespace db {

class ASTContext;
class PathPattern;

class CreateTarget {
public:
    CreateTarget() = default;

    explicit CreateTarget(PathPattern* pattern)
        : _pattern(pattern) {
    }

    const PathPattern* getPattern() const { return _pattern; }
    PathPattern* getPattern() { return _pattern; }

private:
    PathPattern* _pattern {nullptr};
};

class CreateTargets : public std::vector<CreateTarget*> {};

}
