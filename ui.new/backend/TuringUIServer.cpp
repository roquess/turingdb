#include "TuringUIServer.h"

#include "ServerThreadEngine.h"
#include "SiteArchive.h"

#include "BioLog.h"
#include "MsgCommon.h"
#include "MsgUIServer.h"

using namespace Log;

namespace ui {


TuringUIServer::TuringUIServer(const FileUtils::Path& outDir)
    : _outDir(outDir),
      _engine(new ServerThreadEngine(outDir))
{
    const auto sitePath = _outDir / SiteArchive::getSiteDirectoryName();
}

TuringUIServer::~TuringUIServer() {
}

void TuringUIServer::start() {
    // Decompress site archive
    if (!SiteArchive::decompress(_outDir)) {
        _returnCode = -1;
        return;
    }

    BioLog::log(msg::INFO_RUNNING_UI_SERVER() << 5000);
    _engine->run();
}

void TuringUIServer::startDev() {
#ifdef TURING_DEV
    BioLog::log(msg::INFO_RUNNING_UI_SERVER() << 3000);
    _engine->runDev();
#else
    BioLog::log(msg::ERROR_CANNOT_START_DEV_UI_SERVER());
#endif
}

void TuringUIServer::wait() {
    _engine->wait();
    _returnCode = _engine->getReturnCode();
}

int TuringUIServer::getReturnCode() const {
    return _returnCode;
}

}
