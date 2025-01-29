#pragma once

#include <sstream>

namespace db {

class GraphReader;

class GraphReport {
public:
    static void getReport(const GraphReader& reader, std::stringstream& report);
};

}
