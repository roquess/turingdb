#include "TailwindThread.h"
#include "FileUtils.h"

namespace ui {

void TailwindThread::devTask() {
#ifdef TURING_DEV
    // Spawns the flask server with static folder "./" (not used in dev mode)
    // "cd frontend && npx tailwindcss -i ./src/input.css -o ./src/style.css --watch",
    FileUtils::Path turinguiPath {TURINGUI_BACKEND_SRC_DIR};
    turinguiPath = turinguiPath / "frontend";
    std::string cmd = "npm exec --prefix " + turinguiPath.string();
    cmd += " tailwindcss -i ./src/input.css -o ./src/style.css --watch";
    cmd += " > " + (turinguiPath / "tailwindcss.log").string();

    [[maybe_unused]] int code = system(cmd.c_str());
#endif
}

}
