#include "ToolInit.h"

#include "FileUtils.h"
#include "BannerDisplay.h"

#include "BioLog.h"
#include "MsgCommon.h"

using namespace Log;

ToolInit::ToolInit(const std::string& toolName)
    : _toolName(toolName),
    _toolDir(toolName+".out")
{
}

ToolInit::~ToolInit() {
}

void ToolInit::init() {
    BioLog::init();

    if (_toolDir.empty()) {
        _toolDir = std::filesystem::current_path();
    }

    if (files::exists(_toolDir)) {
        if (!files::isDirectory(_toolDir)) {
            BioLog::log(msg::ERROR_NOT_A_DIRECTORY() << _toolDir.string());
            exit(EXIT_FAILURE);
            return;
        }
    } else {
        const bool createRes = files::createDirectory(_toolDir);
        if (!createRes) {
            BioLog::log(msg::ERROR_FAILED_TO_CREATE_DIRECTORY() << _toolDir.string());
            exit(EXIT_FAILURE);
            return;
        }
    }

    _toolDir = std::filesystem::absolute(_toolDir);

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
    BioLog::openFile(logFilePath.string());

    BioLog::echo(BannerDisplay::getBannerString());
}
