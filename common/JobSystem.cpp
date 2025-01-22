#include "JobSystem.h"
#include "BioAssert.h"

#include <spdlog/spdlog.h>

Job& JobQueue::push(Job job) {
    return _jobs.emplace(std::move(job));
}

std::optional<Job> JobQueue::pop() {
    if (_jobs.empty()) {
        return std::nullopt;
    }

    Job job = std::move(_jobs.front());
    _jobs.pop();
    return job;
}

size_t JobQueue::size() const {
    return _jobs.size();
}

bool JobQueue::empty() const {
    return _jobs.empty();
}

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
                std::unique_lock queueLock {_queueMutex};

                // Wait until new job is pushed or stop is requested
                _wakeCondition.wait(queueLock, [&] { return !_jobs.empty() || _stopRequested.load(); });

                // The job queue is locked
                auto j = _jobs.pop();

                if (!j && _stopRequested.load()) {
                    return;
                }

                // Unlock job queue before execution
                queueLock.unlock();
                auto& job = j.value();

                job._operation(job._promise.get());
                job._promise->finish();
                _finishedCount.fetch_add(1);
            }
        });
    }

    spdlog::info("Job system initialized with {} threads", _nThreads);
}

void JobSystem::wait() {
    while (_finishedCount.load() < _submitedCount.load()) {
        _wakeCondition.notify_one();
        std::this_thread::yield();
    }
}

void JobSystem::terminate() {
    msgbioassert(!_terminated, "Attempting to terminate the job system twice");

    {
        std::unique_lock<std::mutex> lock(_queueMutex);
        spdlog::info("Job system terminating, {} jobs remaining", _jobs.size());
    }

    _stopRequested.store(true);
    _wakeCondition.notify_all();
    wait();
    _terminated = true;
    spdlog::info("Job system terminated");
}

JobGroup JobSystem::newGroup() {
    return JobGroup(this);
}
