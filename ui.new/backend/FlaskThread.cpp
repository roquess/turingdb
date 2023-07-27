#include "FlaskThread.h"
#include "FileUtils.h"

namespace ui {

void FlaskThread::task() {
    // TODO change here
    FileUtils::Path staticFolder = std::filesystem::current_path();
    staticFolder = staticFolder / "site";
    std::string cmd = "python3 -m turingui " + staticFolder.string() + "> /dev/null";
    [[maybe_unused]] int code = system(cmd.c_str());
}

void FlaskThread::devTask() {
#ifdef TURING_DEV
    // Spawns the flask server with static folder "./" (not used in dev mode)
    FileUtils::Path turinguiPath {TURINGUI_BACKEND_SRC_DIR};
    turinguiPath = turinguiPath / "backend"/ "turingui" / "__main__.py";

    std::string cmd = "python3 ";
    cmd += turinguiPath;
    cmd += " ./ --dev > /dev/null";
    [[maybe_unused]] int code = system(cmd.c_str());
#endif
}

}
