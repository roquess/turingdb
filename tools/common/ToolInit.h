#ifndef _BIO_TOOL_INIT_
#define _BIO_TOOL_INIT_

#include <string>
#include <filesystem>

#include "ArgParser.h"

class ToolInit {
public:
    using Path = std::filesystem::path;
    
    ToolInit(const std::string& toolName);
    ~ToolInit();

    void setToolDir(const Path& path) { _toolDir = path; }

    void init(int argc, const char** argv);

    const Path& getToolDir() const { return _toolDir; }
    const Path& getOutputsDir() const { return _outputsDir; }
    const Path& getReportsDir() const { return _reportsDir; }

private:
    const std::string _toolName;
    Path _toolDir;
    Path _outputsDir;
    Path _reportsDir;
    ArgParser _argParser;
};

#endif
