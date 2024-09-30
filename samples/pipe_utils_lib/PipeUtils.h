#pragma once

#include <string>
#include <memory>

namespace db {
    class SystemManager;
    class MemoryManager;
    class PlannerContext;
    class QueryPlanner;
}

class JobSystem;

class PipeSample {
public:
    PipeSample(const std::string& sampleName);
    ~PipeSample();

    std::string getTuringHome() const;

    bool loadJsonDB(const std::string& jsonDir);
    bool compileQuery(const std::string& queryStr);
    bool execute();

private:
    std::string _sampleName;
    std::unique_ptr<JobSystem> _jobSystem;
    std::unique_ptr<db::SystemManager> _system;
    std::unique_ptr<db::MemoryManager> _mem;
    std::unique_ptr<db::PlannerContext> _planCtxt;
    std::unique_ptr<db::QueryPlanner> _queryPlanner;
};