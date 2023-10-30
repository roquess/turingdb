#pragma once

#include "FileUtils.h"
#include "BoostProcess.h"

class RegressJob {
public:
    using Path = FileUtils::Path;

    RegressJob(const Path& path);
    ~RegressJob();

    const Path& getPath() const { return _path; }

    bool start(ProcessGroup& group);
    bool isRunning();

    int getExitCode() const;

private:
    const Path _path;
    ProcessChild _process;
};
