#pragma once

#include <cstdint>

namespace db {

struct SourceLocation {
    uint32_t beginLine {1};
    uint32_t beginColumn {1};
    uint32_t endLine {1};
    uint32_t endColumn {1};


    void step() {
        beginLine = endLine;
        beginColumn = endColumn;
    }

    void columns(uint32_t columns = 1) {
        endColumn += columns;
    }

    void lines(uint32_t lines = 1) {
        endLine += lines;
        endColumn = 1;
    }
};

}
