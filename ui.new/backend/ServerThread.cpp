#include "ServerThread.h"

namespace ui {

void ServerThread::run() {
    _thread = std::thread {&ServerThread::task, this};
}

void ServerThread::runDev() {
    _thread = std::thread {&ServerThread::devTask, this};
}

void ServerThread::join() {
    _thread.join();
}

}
