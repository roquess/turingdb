#ifndef _ARG_PARSER_
#define _ARG_PARSER_

#include <string>
#include <map>
#include <vector>
#include <utility>

class ArgParser {
public:
    ArgParser(const std::string& toolName);
    ~ArgParser();

    void setArgsDesc(const std::string& desc);
    void addOption(const std::string& optionName,
                   const std::string& desc,
                   bool expectsArg);

    void parse(int argc, const char** argv);

private:
    struct Option {
        Option(const std::string& desc, bool expectsArg)
            : _desc(desc),
            _expectArg(expectsArg)
        {
        }

        Option() = default;

        std::string _desc;
        bool _expectArg {false};
    };

    std::string _toolName;
    std::string _argsDesc;
    std::map<std::string, Option> _optionMap;
    std::vector<std::pair<std::string, std::string>> _options;
    std::vector<std::string> _args;

    void printHelp() const;
    void handleUnknownOption(const std::string& name) const;
    void handleOptionArgExpected(const std::string& optionName) const;
};

#endif
