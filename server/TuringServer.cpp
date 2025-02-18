#include "TuringServer.h"

#include "HTTPServer.h"
#include "HTTPParser.h"
#include "DBThreadContext.h"
#include "DBServerProcessor.h"
#include "DBURIParser.h"

using namespace db;

TuringServer::TuringServer(TuringDB& db)
    : _db(db)
{
}

TuringServer::~TuringServer() {
}

bool TuringServer::start() {

    net::HTTPServer::Functions functions {
        ._processor =
            [&](net::AbstractThreadContext* threadContext, net::TCPConnection& connection) {
                DBServerProcessor processor(_db, connection);
                processor.process(threadContext);
            },
        ._createThreadContext =
            [] {
                return std::unique_ptr<net::AbstractThreadContext>(new DBThreadContext());
            },
        ._createHttpParser =
            [](net::NetBuffer* inputBuffer) {
                return std::unique_ptr<net::AbstractHTTPParser>(new net::HTTPParser<DBURIParser>(inputBuffer));
            },
    };

    return true;
}
