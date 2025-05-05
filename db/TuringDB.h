#pragma once

#include <string_view>

#include "SystemManager.h"
#include "QueryStatus.h"
#include "QueryCallback.h"
#include "versioning/CommitHash.h"

namespace db {

class LocalMemory;
class JobSystem;
class Block;

class TuringDB {
public:
    TuringDB();
    ~TuringDB();

    SystemManager& getSystemManager() {
        return _systemManager;
    }

    JobSystem& getJobSystem() {
        return *_jobSystem;
    }

    QueryStatus query(std::string_view query,
                      std::string_view graphName,
                      LocalMemory* mem,
                      QueryCallback callback,
                      CommitHash hash = CommitHash::head());

    QueryStatus query(std::string_view query,
                      std::string_view graphName,
                      LocalMemory* mem,
                      CommitHash hash = CommitHash::head());

private:
    SystemManager _systemManager;
    std::unique_ptr<JobSystem> _jobSystem;
};

}
