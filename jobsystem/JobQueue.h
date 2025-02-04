#pragma once

#include <optional>
#include <queue>

#include "Job.h"

namespace js {

class JobQueue {
public:
    Job& push(Job job) {
        std::scoped_lock lock {_mutex};
        _submitted++;
        _wakeCondition.notify_one();
        return _jobs.emplace(std::move(job));
    }

    std::optional<Job> pop() {
        std::scoped_lock lock {_mutex};
        if (_jobs.empty()) {
            return std::nullopt;
        }

        Job job = std::move(_jobs.front());
        _jobs.pop();
        return job;
    }

    size_t size() const {
        std::scoped_lock lock {_mutex};
        return _jobs.size();
    }

    bool empty() const {
        std::scoped_lock lock {_mutex};
        return _jobs.empty();
    }

    std::optional<Job> waitJob(std::invocable auto extraCondition) {
        std::unique_lock lock {_mutex};

        _wakeCondition.wait(lock, [&] {
            return !_jobs.empty() || extraCondition();
        });

        if (_jobs.empty()) {
            return std::nullopt;
        }

        Job job = std::move(_jobs.front());
        _jobs.pop();

        return job;
    }

    void incrementFinished() {
        std::scoped_lock lock {_mutex};
        _finished++;
    }

    void wait() {
        std::unique_lock lock {_mutex};

        while (_finished != _submitted) {
            lock.unlock();
            _wakeCondition.notify_one();
            std::this_thread::yield();
            lock.lock();
        }
        _wakeCondition.notify_all();
    }

private:
    std::condition_variable _wakeCondition;
    mutable std::mutex _mutex;
    std::queue<Job> _jobs;
    size_t _finished {0};
    size_t _submitted {0};
};

}
