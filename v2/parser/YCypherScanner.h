#pragma once

#include "SourceLocation.h"
#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#define YYLLOC_DEFAULT(Current, Rhs, N) \
    do { \
        if (N) { \
            (Current).beginLine = YYRHSLOC(Rhs, 1).beginLine; \
            (Current).beginColumn = YYRHSLOC(Rhs, 1).beginColumn; \
            (Current).endLine = YYRHSLOC(Rhs, N).endLine; \
            (Current).endColumn = YYRHSLOC(Rhs, N).endColumn; \
        } else { \
            (Current).beginLine = (Current).endLine = YYRHSLOC(Rhs, 0).endLine; \
            (Current).beginColumn = (Current).endColumn = YYRHSLOC(Rhs, 0).endColumn; \
        } \
    } while (0)

#include "GeneratedCypherParser.h"

namespace db {

#undef YY_DECL

#define YY_DECL \
    db::YCypherParser::token_type YCypherScanner::lex(db::YCypherParser::semantic_type* yylval, SourceLocation* yylloc)

class YCypherScanner : public yyFlexLexer {
public:
    virtual YCypherParser::token_type lex(YCypherParser::semantic_type* yylval, SourceLocation* yylloc);

    SourceLocation getLocation() const { return _location; }

    void setQuery(std::string_view query) { _query = query; }

    void allowNotImplemented(bool allowNotImplemented) {
        _allowNotImplemented = allowNotImplemented;
    }

    void advanceLocation(uint64_t yyleng) {
        _offset = _nextOffset;
        _nextOffset += yyleng;
        _location.step();
        _location.columns(yyleng);
    }

    void locationNewLine() {
        _location.lines(1);
    }

    void generateError(const std::string& msg, std::string& errorOutput, bool printErrorBars = true);
    [[noreturn]] void syntaxError(const std::string& msg);
    void notImplemented(std::string_view rawMsg);

private:
    size_t _nextOffset = 0;
    size_t _offset = 0;
    SourceLocation _location;
    std::string_view _query;
    bool _allowNotImplemented = true;

    std::string_view getStringView(size_t offset, size_t length) const {
        return _query.substr(offset, length);
    }
};

}
