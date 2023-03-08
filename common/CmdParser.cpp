#include "CmdParser.h"

CmdParser::CmdParser()
{
}

bool CmdParser::parse(int argc, char** argv) {
    if (argc <= 0) {
        return false;
    }

    // Parsing options
    int pos = 0;
    std::string optionName;
    for (pos = 1; pos < argc; pos++) {
        optionName.assign(argv[pos]);
        if (optionName.empty()) {
            break;
        }

        if (optionName == "-h") {
            _helpRequested = true;
        } else if (optionName == "-v") {
            _versionRequested = true;
        } else if (optionName.front() == '-') {
            break;
        }
    }

    // Parsing arguments
    for (; pos < argc; pos++) {
        std::string argStr(argv[pos]);
        if (!argStr.empty()) {
            _args.emplace_back(std::move(argStr));
        }
    }

    return true;
}