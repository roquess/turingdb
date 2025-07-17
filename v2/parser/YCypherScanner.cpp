#include "YCypherScanner.h"

#include <sstream>

#include "BioAssert.h"
#include "ParserException.h"

void db::YCypherScanner::generateError(const std::string& msg, std::string& errorOutput, const location& loc) {
    bioassert(_query != nullptr);

    const size_t firstLine = loc.begin.line;
    const size_t lastLine = loc.end.line;

    if (firstLine != lastLine) {
        // Multi-line error
        std::istringstream ss(*_query, std::ios_base::in);
        size_t lineCount = lastLine - firstLine + 1;
        std::string line;

        for (size_t i = 1; i < firstLine; i++) { // Location is 1-indexed
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        for (size_t i = 0; i < lineCount; i++) {
            const size_t lineNo = firstLine + i;
            std::string prefix = fmt::format("  {:>4} | ", lineNo);

            if (i == 0) {
                const std::string blank(prefix.size() + loc.begin.column - 1, ' ');
                errorOutput += fmt::format("{}⌄ \n", blank, firstLine, line);
            }

            line.clear();
            std::getline(ss, line);
            errorOutput += fmt::format("{}{}\n", prefix, line);
        }

        errorOutput += "\n\t" + msg + "\n";
        return;
    }

    const size_t errLineNo = firstLine;

    std::istringstream ss(*_query, std::ios_base::in);

    for (size_t i = 1; i < errLineNo; i++) { // Location is 1-indexed
        ss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::string errLine;
    std::getline(ss, errLine);

    const size_t errLen = loc.end.column - loc.begin.column;
    const std::string errorBars(errLen, '^');

    std::string prefixLine = fmt::format("Line {}: ", errLineNo);
    const size_t blankLen = prefixLine.size() + loc.begin.column - 1;
    const std::string blank(blankLen, ' ');

    errorOutput = fmt::format("{}{}\n", prefixLine, errLine);
    errorOutput += fmt::format("{}{}\n", blank, errorBars);
    errorOutput += "\t" + msg + "\n\n";
}

void db::YCypherScanner::syntaxError(const std::string& msg) {
    std::string errorMsg;
    generateError(msg, errorMsg, _location);

    throw ParserException(std::move(errorMsg));
}

void db::YCypherScanner::notImplemented(std::string_view rawMsg) {
    if (!_throwNotImplemented) {
        return;
    }

    std::string msg = fmt::format("Feature not implemented: {}", rawMsg);
    std::string errorMsg;
    generateError(msg, errorMsg, _location);
    throw ParserException(std::move(errorMsg));
}
