#include "BioserverThread.h"

#include <spawn.h>

namespace ui {

void BioserverThread::task() {
    [[maybe_unused]] int code = system("bioserver > /dev/null");
}

}
