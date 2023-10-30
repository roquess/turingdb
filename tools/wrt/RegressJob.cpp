#include "RegressJob.h"

#include "Command.h"

#include <boost/process.hpp>

#include "BioLog.h"
#include "MsgCommon.h"

using namespace Log;

RegressJob::RegressJob(const Path& path)
    : _path(path)
{
}

RegressJob::~RegressJob() {
}

bool RegressJob::start(ProcessGroup& group) {
    const auto runScriptPath = _path/"run.sh";
    if (!FileUtils::exists(runScriptPath)) {
        BioLog::log(msg::ERROR_FILE_NOT_EXISTS() << runScriptPath.string());
        return false;
    }

    Command cmd(runScriptPath.string());
    cmd.setWorkingDir(_path);
    cmd.setLogFile(_path/"run.log");
    cmd.setGenerateScript(true);

    BioLog::echo("Run: "+_path.string());
    _process = cmd.runAsync(group);
    return true;
}

bool RegressJob::isRunning() {
    if (!_process) {
        return false;
    }
    return _process->running();
}

int RegressJob::getExitCode() const {
    return _process->exit_code();
}
