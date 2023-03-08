#ifndef _BIO_MSG_IMPORT_
#define _BIO_MSG_IMPORT_

#include "Message.h"

namespace msg {

class ERROR_IMPORT_FAILED_FIND_NEO4J_ARCHIVE : public Log::Message {
public:
    ERROR_IMPORT_FAILED_FIND_NEO4J_ARCHIVE()
        : Message(2001, Message::ERROR, "Failed to find Neo4j archive '$0'")
    {}
};

class INFO_DECOMPRESS_NEO4J_ARCHIVE : public Log::Message {
public:
    INFO_DECOMPRESS_NEO4J_ARCHIVE()
        : Message(2002, Message::INFO, "Decompressing Neo4j archive in $0")
    {}
};

class ERROR_IMPORT_FAILED_DECOMPRESS_NEO4J_ARCHIVE : public Log::Message {
public:
    ERROR_IMPORT_FAILED_DECOMPRESS_NEO4J_ARCHIVE()
        : Message(2003, Message::ERROR, "Failed to decompress Neo4j archive '$0'")
    {}
};

class INFO_CLEAN_NEO4J_SETUP : public Log::Message {
public:
    INFO_CLEAN_NEO4J_SETUP()
        : Message(2004, Message::INFO, "Clean Neo4j setup")
    {}
};

class INFO_STARTING_NEO4J : public Log::Message {
public:
    INFO_STARTING_NEO4J()
        : Message(2005, Message::INFO, "Starting Neo4j at $0")
    {}
};

class ERROR_FAILED_TO_START_NEO4J : public Log::Message {
public:
    ERROR_FAILED_TO_START_NEO4J()
        : Message(2006, Message::ERROR, "Failed to start Neo4j")
    {}
};

class INFO_STOPPING_NEO4J : public Log::Message {
public:
    INFO_STOPPING_NEO4J()
        : Message(2007, Message::INFO, "Stopping Neo4j")
    {}
};

class ERROR_FAILED_TO_STOP_NEO4J : public Log::Message {
public:
    ERROR_FAILED_TO_STOP_NEO4J()
        : Message(2008, Message::ERROR, "Failed to stop Neo4j")
    {}
};

class ERROR_EMPTY_DB_NAME : public Log::Message {
public:
    ERROR_EMPTY_DB_NAME()
        : Message(2009, Message::ERROR, "Empty database name in '$0'")
    {}
};

class INFO_COPY_DB : public Log::Message {
public:
    INFO_COPY_DB()
        : Message(2010, Message::INFO, "Copy Neo4J database from '$0' to '$1'")
    {}
};

class INFO_SEND_NEO4J_HTTP_REQUEST : public Log::Message {
public:
    INFO_SEND_NEO4J_HTTP_REQUEST()
        : Message(2011, Message::INFO, "Sending HTTP request to Neo4j")
    {}
};

class ERROR_NEO4J_HTTP_REQUEST : public Log::Message {
public:
    ERROR_NEO4J_HTTP_REQUEST()
        : Message(2012, Message::ERROR, "Error in request to Neo4j '$0'")
    {}
};

class INFO_NEO4J_WAIT_WARMUP : public Log::Message {
public:
    INFO_NEO4J_WAIT_WARMUP()
        : Message(2013, Message::INFO, "Waiting $0 seconds for Neo4j to warmup")
    {}
};

class ERROR_FAILED_TO_PARSE_JSON : public Log::Message {
public:
    ERROR_FAILED_TO_PARSE_JSON()
        : Message(2014, Message::ERROR, "Failed to parse JSON file '$0'")
    {}
};

class INFO_EXTRACTING_ENTITIES_FROM_NEO4J_DUMP : public Log::Message {
public:
    INFO_EXTRACTING_ENTITIES_FROM_NEO4J_DUMP()
        : Message(2015, Message::INFO, "Extracting nodes and relations from Neo4j dump")
    {}
};

}

#endif
