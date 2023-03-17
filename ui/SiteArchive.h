#ifndef _UI_SITE_ARCHIVE_
#define _UI_SITE_ARCHIVE_

#include <filesystem>

namespace ui {

class SiteArchive {
public:
    static void decompress(const std::filesystem::path& outDir);

    static void clean(const std::filesystem::path& outDir);

private:
    SiteArchive();
    ~SiteArchive();
};

}

#endif
