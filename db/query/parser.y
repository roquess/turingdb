%skeleton "lalr1.cc"
%require "3.0"
%defines
%define api.parser.class { YParser }

%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define api.namespace { db::query }

%code requires
{

#include <string>

namespace db::query {
class YScanner;
class ASTContext;
class QueryCommand;
}

}

%code top
{

#include "YScanner.h"
#include "parser.hpp"

#include "ASTContext.h"
#include "QueryCommand.h"

#include "BioLog.h"

using namespace db::query;
using namespace Log;

static db::query::YParser::symbol_type yylex(db::query::YScanner& scanner) {
    return scanner.get_next_token();
}

}

%lex-param { db::query::YScanner& scanner }
%parse-param { db::query::YScanner& scanner }
%parse-param { db::query::ASTContext* ctxt }

%define api.token.prefix {TOKEN_}

%token UNKNOWN_TOKEN

%token LIST
%token DATABASES

%token END

%type<db::query::QueryCommand*> list_cmd

%start query_unit

%%

query_unit: list_cmd { ctxt->setRoot($1); }
          | error    { ctxt->setError(true); }
          ;

list_cmd: LIST DATABASES {
                            auto cmd = ListCommand::create(ctxt); 
                            cmd->setSubType(ListCommand::LCOM_DATABASES);
                            $$ = cmd;
                         }
        ;

%%

void db::query::YParser::error(const std::string &message) {
}
