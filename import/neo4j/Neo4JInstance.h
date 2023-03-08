#ifndef _BIO_NEO4J_INSTANCE_
#define _BIO_NEO4J_INSTANCE_

#include <filesystem>

class Neo4JInstance {
public:
    Neo4JInstance(const std::string& path);
    ~Neo4JInstance();

    bool setup();
    bool start();
    bool stop();
    void destroy();

    bool importDBDir(const std::string& dbPath);
    bool changePassword(const std::string& oldPassword, const std::string& newPassword);

private:
    const std::filesystem::path _neo4jDir;

    std::filesystem::path getNeo4jBinary() const;
};

#endif
