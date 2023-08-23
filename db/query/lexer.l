%{

#include "YScanner.h"
#include "parser.hpp"

using namespace db::query;

%}

%option noyywrap
%option c++
%option yyclass="YScanner"
%option prefix="YScanner_"

%%

[ \t\r\n]+  { }

"list"    { return YParser::make_LIST(); }

"databases" { return YParser::make_DATABASES(); }



.           { return YParser::make_UNKNOWN_TOKEN(); }

<<EOF>>     {  yyterminate(); }

%%
