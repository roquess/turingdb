%{

#include <charconv>

#include "YCypherScanner.h"
#include "GeneratedCypherParser.h"

using namespace db;

#define yyterminate() return db::YCypherParser::token::PROG_END
#define YY_USER_ACTION advanceLocation(yyleng);

using Token = db::YCypherParser::token;

%}

%option caseless noyywrap c++ yyclass="YCypherScanner"

%%

\n        { locationNewLine(); }
[ \t\r]+  { }

"//".*     { }
"/*".*"*/" { }

"--"  { return Token::TAIL_TAIL; }
"<--" { return Token::TIP_TAIL_TAIL; }
"-->" { return Token::TAIL_TAIL_TIP; }
"<-[" { return Token::TIP_TAIL_BRACKET; }
"]->" { return Token::BRACKET_TAIL_TIP; }
"-["  { return Token::TAIL_BRACKET; }
"]-"  { return Token::BRACKET_TAIL; }

"+=" { return Token::ADD_ASSIGN; }
"<>" { return Token::NOT_EQUAL; }
"!=" { return Token::NOT_EQUAL; }
".." { return Token::RANGE; }
"<=" { return Token::LE; }
">=" { return Token::GE; }

";"  { return Token::SEMI_COLON; }
"("  { return Token::OPAREN; }
")"  { return Token::CPAREN; }
"{"  { return Token::OBRACE; }
"}"  { return Token::CBRACE; }
"["  { return Token::OBRACK; }
"]"  { return Token::CBRACK; }
"="  { return Token::ASSIGN; }
"$"  { return Token::DOLLAR; }
","  { return Token::COMMA; }
":"  { return Token::COLON; }
"^"  { return Token::CARET; }
"|"  { return Token::PIPE; }
"+"  { return Token::PLUS; }
"*"  { return Token::MULT; }
"`"  { return Token::ESC; }
"-"  { return Token::SUB; }
"."  { return Token::DOT; }
"/"  { return Token::DIV; }
"%"  { return Token::MOD; }
">"  { return Token::GT; }
"<"  { return Token::LT; }


"DESCENDING" { return Token::DESCENDING; }
"CONSTRAINT" { return Token::CONSTRAINT; }
"MANDATORY" { return Token::MANDATORY; }
"ASCENDING" { return Token::ASCENDING; }
"OPTIONAL" { return Token::OPTIONAL; }
"CONTAINS" { return Token::CONTAINS; }
"DISTINCT" { return Token::DISTINCT; }
"EXTRACT" { return Token::EXTRACT; }
"REQUIRE" { return Token::REQUIRE; }
"STARTS" { return Token::STARTS; }
"UNIQUE" { return Token::UNIQUE; }
"FILTER" { return Token::FILTER; }
"SINGLE" { return Token::SINGLE; }
"SCALAR" { return Token::SCALAR; }
"UNWIND" { return Token::UNWIND; }
"REMOVE" { return Token::REMOVE; }
"RETURN" { return Token::RETURN; }
"CREATE" { return Token::CREATE; }
"DELETE" { return Token::DELETE; }
"DETACH" { return Token::DETACH; }
"EXISTS" { return Token::EXISTS; }
"LIMIT" { return Token::LIMIT; }
"YIELD" { return Token::YIELD; }
"MATCH" { return Token::MATCH; }
"MERGE" { return Token::MERGE; }
"ORDER" { return Token::ORDER; }
"WHERE" { return Token::WHERE; }
"UNION" { return Token::UNION; }
"FALSE" { return Token::FALSE; }
"COUNT" { return Token::COUNT; }
"DESC" { return Token::DESC; }
"CALL" { return Token::CALL; }
"NULL" { return Token::NULL_; }
"TRUE" { return Token::TRUE; }
"WHEN" { return Token::WHEN; }
"NONE" { return Token::NONE; }
"THEN" { return Token::THEN; }
"ELSE" { return Token::ELSE; }
"CASE" { return Token::CASE; }
"ENDS" { return Token::ENDS; }
"DROP" { return Token::DROP; }
"SKIP" { return Token::SKIP; }
"WITH" { return Token::WITH; }
"ANY" { return Token::ANY; }
"SET" { return Token::SET; }
"ALL" { return Token::ALL; }
"ASC" { return Token::ASC; }
"NOT" { return Token::NOT; }
"END" { return Token::END; }
"XOR" { return Token::XOR; }
"FOR" { return Token::FOR; }
"ADD" { return Token::ADD; }
"AND" { return Token::AND; }
"OR" { return Token::OR; }
"||" { return Token::OR; }
"IN" { return Token::IN; }
"IS" { return Token::IS; }
"BY" { return Token::BY; }
"DO" { return Token::DO; }
"OF" { return Token::OF; }
"ON" { return Token::ON; }
"AS" { return Token::AS; }


`[^`]*` {
    /* Escaped identifier. Example: `my-var` */
    yylval->emplace<std::string>(yytext + 1, yyleng - 2);
    return Token::ESC_LITERAL;
}


'([^'\\\r\n]|\\.|\\)' {
    /* Character literal */
    yylval->emplace<char>(yytext[1]);
    return Token::CHAR_LITERAL;
}


\"([^\"\\\r\n]|\\.)*\" {
    /* String literal */
    yylval->emplace<std::string>(yytext + 1, yyleng - 2);
    return Token::STRING_LITERAL;
}

\'([^\'\\\r\n]|\\.)*\' {
    /* Single-quoted string literal */
    yylval->emplace<std::string>(yytext + 1, yyleng - 2);
    return Token::STRING_LITERAL;
}

-?[0-9]*\.[0-9]+([eE][+-]?[0-9]+)?[fd]? {
    /* Float pattern */
    float v = 0.0f;
    std::from_chars(yytext, yytext + yyleng, v);
    yylval->emplace<float>(v);

    return Token::FLOAT;
}


-?[0-9]+[eE][+-]?[0-9]+[fd]? {
    /* Float pattern */
    float v = 0.0f;
    std::from_chars(yytext, yytext + yyleng, v);
    yylval->emplace<float>(v);

    return Token::FLOAT;
}


-?[0-9]+[fd] {
    float v = 0.0f;
    std::from_chars(yytext, yytext + yyleng, v);
    yylval->emplace<float>(v);

    return Token::FLOAT;
}


-?0[xX][0-9a-fA-F]+ {
    int64_t v = 0;
    std::from_chars(yytext, yytext + yyleng, v);
    yylval->emplace<int64_t>(v);

    return Token::DIGIT;
}


-?0[0-7]+ {
    int64_t v = 0;
    std::from_chars(yytext, yytext + yyleng, v);
    yylval->emplace<int64_t>(v);

    return Token::DIGIT;
}


-?[0-9]+ {
    int64_t v = 0;
    std::from_chars(yytext, yytext + yyleng, v);
    yylval->emplace<int64_t>(v);

    return Token::DIGIT;
}


[a-zA-Z_][a-zA-Z0-9_]* {
    /* Identifier */
    yylval->emplace<std::string>(yytext, yyleng);
    return Token::ID;
}


. { return YCypherParser::token::UNKNOWN; }


%%
