#include "QueryParser.h"

#include <sstream>

#include "YScanner.h"
#include "parser.hpp"
#include "ASTContext.h"

using namespace db::query;

QueryParser::QueryParser()
{
}

QueryCommand* QueryParser::parse(const std::string& queryStr) {
    ASTContext astCtxt;
    YScanner yscanner;
    YParser yparser(yscanner, &astCtxt);

    std::istringstream iss(queryStr);
    yscanner.switch_streams(&iss, NULL);

    yparser.parse();

    return astCtxt.getRoot();
}
