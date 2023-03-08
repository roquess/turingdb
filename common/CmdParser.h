#ifndef _BIO_COMMON_CMD_PARSER_
#define _BIO_COMMON_CMD_PARSER_

#include <string>
#include <vector>

class CmdParser {
public:
    CmdParser();

    bool isHelpRequested() const { return _helpRequested; }
    bool isVersionRequested() const { return _versionRequested; }

    bool parse(int argc, char** argv);

private:
    bool _helpRequested {false};
    bool _versionRequested {false};
    std::vector<std::string> _args;
};

#endif