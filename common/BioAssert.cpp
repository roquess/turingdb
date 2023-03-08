#include "BioAssert.h"

#include <stdio.h>
#include <stdlib.h>

void __bioAssertWithLocation(const char* file, unsigned line, const char* expr) {
    fprintf(stderr, "Internal Error: The assertion '%s' failed at %s:%u\n\n", expr, file, line);
    abort();
}

