#include "OpenDBStep.h"

using namespace db;

OpenDBStep::OpenDBStep(const std::string& path)
    : _dbPath(path)
{
}

OpenDBStep::~OpenDBStep() {
}

std::string OpenDBStep::getName() const {
    return "OpenDBStep";
}
