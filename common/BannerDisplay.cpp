#include "BannerDisplay.h"

#include <string>
#include <sstream>

#include "BuildInfo.h"

std::string BannerDisplay::getBannerString() {
    std::ostringstream banner;
    banner << "******* Turing Platform\n";
    banner << "  ***** Build " << HEAD_COMMIT_STRING << "\n";
    banner << "    *** Copyright Turing Biosystems Ltd. All Rights Reserved.\n";
    return banner.str();
}
