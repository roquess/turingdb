#pragma once

#include "FileUtils.h"
#include "ServerType.h"

namespace ui {

class ServerCommandEngine;

class TuringUIServer {
public:
    TuringUIServer(const FileUtils::Path& outDir);
    ~TuringUIServer();

    bool start(const std::vector<std::string>& dbNames);
    bool startDev();
    bool startPrototype(const std::vector<std::string>& dbNames);
    bool startPrototypeDev();
    void terminate();
    ServerType waitServerDone();

    int getReturnCode(ServerType serverType) const;
    void getOutput(ServerType serverType, std::string& output) const;

private:
    FileUtils::Path _outDir;
    std::unique_ptr<ServerCommandEngine> _engine;

    void cleanSite();
};

}
