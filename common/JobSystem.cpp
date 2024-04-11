#include "JobSystem.h"
#include "BioLog.h"

void JobQueue::push(Job job) {
    _jobs.insert(_jobs.begin(), std::move(job));
}

std::optional<Job> JobQueue::pop() {
    if (_jobs.empty()) {
        return std::nullopt;
    }

    Job job = std::move(_jobs.back());
    _jobs.erase(_jobs.end() - 1);
    return job;
}

JobSystem::~JobSystem() {
    terminate();
}

void JobSystem::initialize(size_t numThreads) {
    if (numThreads == 0) {
        size_t numCores = std::thread::hardware_concurrency();
        numThreads = std::max(2ul, numCores) - 1ul;
    }

    Log::BioLog::echo("JobSystem initialize with " + std::to_string(numThreads) + " threads.");

    for (size_t i = 0; i < numThreads; i++) {
        _workers.emplace_back([&] {
            JobOperation job;

            while (true) {
                _queueMutex.lock();
                auto job = _jobs.pop();
                _queueMutex.unlock();

                if (_stopRequested.load()) {
                    return;
                }

                if (job.has_value()) {
                    auto& jobValue = job.value();
                    jobValue._operation(jobValue._promise.get());
                    _finishedCount.fetch_add(1);
                } else {
                    // no job, put thread to sleep
                    std::unique_lock<std::mutex> lock(_wakeMutex);
                    _wakeCondition.wait(lock);
                }
            }
        });
    }
}

void JobSystem::wait() {
    while (_finishedCount.load() < _submitedCount.load()) {
        _wakeCondition.notify_one();
        std::this_thread::yield();
    }
}

void JobSystem::terminate() {
    _stopRequested.store(true);
    _wakeCondition.notify_all();
}
