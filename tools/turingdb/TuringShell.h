#pragma once

#include <unordered_map>
#include <string>
#include <string_view>
#include <functional>
#include <vector>

namespace db {

class TuringDB;
class LocalMemory;

class TuringShell {
public:
    struct Command {
        using Words = std::vector<std::string>;
        std::function<void(const Words&, TuringShell&)> _func;
    };

    TuringShell(TuringDB& turingDB, LocalMemory* mem);
    ~TuringShell();

    void setGraphName(const std::string& graphName) { _graphName = graphName; }
    void setQuiet(bool quiet) { _quiet = quiet; }
    void startLoop();

    void printHelp() const;

private:
    TuringDB& _turingDB;
    LocalMemory* _mem {nullptr};
    std::string _graphName {"default"};
    bool _quiet {false};
    std::unordered_map<std::string_view, Command> _localCommands;

    void processLine(std::string& line);
    std::string composePrompt();
};

}
