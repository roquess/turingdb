#include "ToolInit.h"

#include "BannerDisplay.h"
#include "BioLog.h"

using namespace Log;

ToolInit::ToolInit(const std::string& toolName)
    : _toolName(toolName)
{
}

ToolInit::~ToolInit() {
}

void ToolInit::init() {
    // Get tool directory path
    if (_toolDir.empty()) {
        _toolDir = std::filesystem::current_path();
    } else {
        _toolDir = std::filesystem::absolute(_toolDir);
    }

    // Check that tool directory exists
    if (!std::filesystem::exists(_toolDir)) {
        std::filesystem::create_directory(_toolDir);
    }

    // Get outputs and reports path
    _outputsDir = _toolDir/"outputs";
    _reportsDir = _toolDir/"reports";

    // Create outputs and reports directories
    std::filesystem::remove_all(_outputsDir);
    std::filesystem::remove_all(_reportsDir);
    std::filesystem::create_directory(_outputsDir);
    std::filesystem::create_directory(_reportsDir);

    // Init logging
    const auto logFilePath = _reportsDir/(_toolName + ".log");
    BioLog::init(logFilePath.string());

    BioLog::echo(BannerDisplay::getBannerString());
}
