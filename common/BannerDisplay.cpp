#include "BannerDisplay.h"

#include <string>
#include <sstream>
#include <boost/date_time/local_time/local_time.hpp>

#include "BuildInfo.h"

using namespace boost::local_time;
using namespace boost::posix_time;
using namespace boost::gregorian;

std::string BannerDisplay::getBannerString() {
    const auto utcTime = boost::posix_time::second_clock::universal_time();
    const time_zone_ptr tz(new posix_time_zone("GMT"));
    const local_date_time dateTime(utcTime, tz);

    std::ostringstream banner;
    banner << "******* Turing Platform\n";
    banner << "  ***** Build " << HEAD_COMMIT_STRING << " on " << dateTime << "\n";
    banner << "    *** Copyright 2020-" << dateTime.date().year() << " Turing Biosystems. All Rights Reserved.\n";
    return banner.str();
}
