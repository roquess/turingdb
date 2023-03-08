#include "Neo4JHttpRequest.h"

#include <cstdlib>

#include "BioLog.h"
#include "MsgImport.h"

using namespace Log;

Neo4JHttpRequest::Neo4JHttpRequest(const std::string& outFile)
    : _outFile(outFile)
{
}

Neo4JHttpRequest::~Neo4JHttpRequest() {
}

std::string Neo4JHttpRequest::getCurlString() const {
    // Build url
    std::string url = "http://";
    url += _user;
    url += ":";
    url += _password;
    url += "@";
    url += _host;
    url += ":";
    url += std::to_string(_port);
    url += "/db/data/transaction/commit";

    // Build data string
    std::string data = "'{\"statements\": [{\"statement\": \"";
    data += _stmt;
    data += "\"}]}'";

    std::string curlString = "/usr/bin/curl -o ";
    curlString += _outFile;
    curlString += " -X POST";
    curlString += " -H 'Content-type: application/json' ";
    curlString += url;
    curlString += " -d ";
    curlString += data;

    return curlString;
}

bool Neo4JHttpRequest::exec() {

    BioLog::log(msg::INFO_SEND_NEO4J_HTTP_REQUEST());

    const int curlRes = std::system(getCurlString().c_str());
    if (curlRes != 0) {
        BioLog::log(msg::ERROR_NEO4J_HTTP_REQUEST() << _stmt);
        return false;
    }

    return true;
}
