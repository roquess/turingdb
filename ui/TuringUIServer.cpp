#include "TuringUIServer.h"

#include <filesystem>

#include "SiteArchive.h"
#include "System.h"

using namespace ui;

TuringUIServer::TuringUIServer()
{
}

TuringUIServer::~TuringUIServer() {
}

void TuringUIServer::start() {
    // Decompress site archive
    const auto outDir = std::filesystem::path("/tmp/biocauldron");
    SiteArchive::decompress(outDir);
}
