#include "SiteArchive.h"

#include <string>
#include <fstream>

#include "SiteArchiveData.h"

#include "System.h"
#include "FileUtils.h"
#include "BioLog.h"

#include "MsgCommon.h"
#include "MsgUIServer.h"

using namespace Log;

static constexpr const char* SiteArchiveName = "site.tar";
static constexpr const char* SiteName = "site";

using namespace ui;

SiteArchive::SiteArchive()
{
}

SiteArchive::~SiteArchive() {
}

void SiteArchive::decompress(const std::filesystem::path& outDir) {
    if (files::exists(outDir)) {
        if (!files::isDirectory(outDir)) {
            BioLog::log(msg::ERROR_NOT_A_DIRECTORY() << outDir.string());
            return;
        }
    } else {
        const bool createResult = files::createDirectory(outDir);
        if (!createResult) {
            BioLog::log(msg::ERROR_FAILED_TO_CREATE_DIRECTORY() << outDir.string());
            return;
        }
    }

    // Write archive file in output directory
    const auto archiveFileName = outDir/SiteArchiveName;
    std::ofstream archiveFile(archiveFileName, std::ios_base::binary);
    archiveFile.write((const char*)globfs_site_data, globfs_site_size);

    // Decompress archive file
    std::string untarCmd = "cd ";
    untarCmd += outDir.string(); 
    untarCmd += " && tar -xf ";
    untarCmd += SiteArchiveName;

    BioLog::log(msg::INFO_DECOMPRESSING_SITE());
    const int decompressResult = System::runCommand(untarCmd);
    if (decompressResult != 0) {
        BioLog::log(msg::ERROR_FAILED_TO_DECOMPRESS_SITE() << outDir.string()); 
        clean(outDir);
        return;
    }

    // Prepare site
    const auto sitePath = outDir/SiteName;
    std::string buildCmd = "cd ";
    buildCmd += sitePath.string();
    buildCmd += " && ./build.sh > /dev/null 2>&1";

    BioLog::log(msg::INFO_BUILDING_SITE());
    const int buildResult = System::runCommand(buildCmd);
    if (buildResult != 0) {
        BioLog::log(msg::ERROR_FAILED_TO_BUILD_SITE() << outDir.string());
        clean(outDir);
        return;
    }

    // Run node server
    std::string nodeStartCmd = "cd ";
    nodeStartCmd += sitePath.string();
    nodeStartCmd += " && npm run start";

    BioLog::log(msg::INFO_RUN_NODE_SERVER());
    const int runResult = System::runCommand(nodeStartCmd);
    if (runResult != 0) {
        BioLog::log(msg::INFO_STOPPING_NODE_SERVER());
        clean(outDir);
        return;
    }

    clean(outDir);
}

void SiteArchive::clean(const std::filesystem::path& outDir) {
    const bool result = files::removeDirectory(outDir);
    if (!result) {
        BioLog::log(msg::ERROR_CLEANING_SITE_FAILED() << outDir.string());
        return;
    }
}
