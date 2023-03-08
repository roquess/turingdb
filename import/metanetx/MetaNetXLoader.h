#ifndef _BIO_IMPORT_METANETX_LOADER_
#define _BIO_IMPORT_METANETX_LOADER_

#include <string>

namespace db {
    class DB;
}

class MetaNetXConfig {
public:
    MetaNetXConfig(const std::string& metanetxPath);

    const std::string& getMetaNetXPath() const { return _metanetxPath; }

    const std::string& getChemPropPath() const { return _chemPropPath; }
    void setChemPropPath(const std::string& chemPropPath) { 
        _chemPropPath = chemPropPath;
    }

private:
    const std::string _metanetxPath;
    std::string _chemPropPath;
};

class MetaNetXLoader {
public:
    MetaNetXLoader(const MetaNetXConfig& config, db::DB* db);

    bool load();

private:
    const MetaNetXConfig& _config;
    db::DB* _db;

    bool loadChemProp();
};

#endif
