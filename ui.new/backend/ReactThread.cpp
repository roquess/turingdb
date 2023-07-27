#include "ReactThread.h"
#include "FileUtils.h"

namespace ui {

void ReactThread::devTask() {
#ifdef TURING_DEV
    FileUtils::Path previousPath = std::filesystem::current_path();
    FileUtils::Path turinguiPath {TURINGUI_BACKEND_SRC_DIR};
    turinguiPath = turinguiPath / "frontend";
    std::string cmd = "npm --prefix " + turinguiPath.string();
    cmd += " start > " + (previousPath / "react.log").string();

    [[maybe_unused]] int code = system(cmd.c_str());
#endif
}

}
