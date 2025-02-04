#include "JobSystem.h"

#include <cassert>

#include "JobGroup.h"

using namespace js;

JobSystem::JobSystem()
    : _mainThreadID(std::this_thread::get_id())
{
}

JobSystem::JobSystem(size_t nThreads)
    : _mainThreadID(std::this_thread::get_id()),
      _nThreads(nThreads)
{
}

JobSystem::~JobSystem() {
    if (!_terminated) {
        terminate();
    }
}

void JobSystem::initialize() {
    if (_nThreads == 0) {
        size_t numCores = std::thread::hardware_concurrency();
        _nThreads = std::max(1ul, numCores);
    }

    for (size_t i = 0; i < _nThreads; i++) {
        _workers.emplace_back([&] {
            while (true) {
                std::optional<Job> j = _jobs.waitJob([&] {
                    return _stopRequested.load();
                });

                if (!j && _stopRequested.load()) {
                    // No more job available and stop requested
                    return;
                }

                auto& job = j.value();

                job._operation(job._promise.get());
                job._promise->finish();
                _jobs.incrementFinished();
            }
        });
    }
}

void JobSystem::wait() {
    _jobs.wait();
}

void JobSystem::terminate() {
    assert(!_terminated);
    _stopRequested.store(true);
    _jobs.wait();
    _terminated = true;
}

JobGroup JobSystem::newGroup() {
    return JobGroup(this);
}
