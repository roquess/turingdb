#pragma once

#include <filesystem>
#include <fstream>

namespace db {
class Network;
}

class GMLDumper {
public:
    using Path = std::filesystem::path;

    GMLDumper(const db::Network* net, const Path& gmlFilePath);
    ~GMLDumper();

    bool dump();

private:
    const db::Network* _net {nullptr};
    const Path _gmlFilePath;
    std::ofstream _gml;
};
