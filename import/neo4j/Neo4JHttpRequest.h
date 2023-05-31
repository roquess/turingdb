#ifndef _BIO_NEO4J_HTTP_REQUEST_
#define _BIO_NEO4J_HTTP_REQUEST_

#include <string>

class Neo4JHttpRequest {
public:
    Neo4JHttpRequest(const std::string& outFile);
    ~Neo4JHttpRequest();

    void setHost(const std::string& host) { _host = host; }
    void setPort(unsigned port) { _port = port; }
    void setUser(const std::string& user) { _user = user; }
    void setPassword(const std::string& password) { _password = password; }
    void setStatement(const std::string& stmt) { _stmt = stmt; }

    std::string getCurlString() const;

    bool exec();

private:
    std::string _outFile;
    std::string _host{"localhost"};
    unsigned _port {7474};
    std::string _user{"neo4j"};
    std::string _password{"turing"};
    std::string _stmt;
};

#endif
