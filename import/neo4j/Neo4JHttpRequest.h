#ifndef _BIO_NEO4J_HTTP_REQUEST_
#define _BIO_NEO4J_HTTP_REQUEST_

#include <string>
#include <filesystem>

class Neo4JHttpRequest {
public:
    struct RequestProps {
        std::string host = "localhost";
        std::string user = "neo4j";
        std::string password = "turing";
        uint16_t port = 7474;
    };

    Neo4JHttpRequest(RequestProps&& props);
    ~Neo4JHttpRequest();

    void setStatement(const std::string& s) { _statement = s; }

    bool exec();
    bool writeToFile(const std::filesystem::path& path) const;

    const std::string& getData() const { return _data; }

private:
    std::string _url;
    std::string _username;
    std::string _password;
    std::string _statement;
    std::string _jsonRequest;
    std::string _data;
};

#endif
