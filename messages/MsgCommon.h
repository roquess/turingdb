#ifndef _BIO_MSG_COMMON_
#define _BIO_MSG_COMMON_

#include "Message.h"

namespace msg {

class FATAL_FAILED_TO_CREATE_DIRECTORY : public Log::Message {
public:
    FATAL_FAILED_TO_CREATE_DIRECTORY()
        : Message(1001, Message::FATAL, "Failed to create directory '$0'")
    {}
};

class ERROR_DIRECTORY_NOT_EXISTS : public Log::Message {
public:
    ERROR_DIRECTORY_NOT_EXISTS()
        : Message(1002, Message::ERROR, "The directory '$0' does not exist")
    {}
};

class ERROR_FAILED_TO_COPY : public Log::Message {
public:
    ERROR_FAILED_TO_COPY()
        : Message(1003, Message::ERROR, "Failed to copy '$0' to '$1'")
    {}
};

class ERROR_FAILED_TO_OPEN_FOR_READ : public Log::Message {
public:
    ERROR_FAILED_TO_OPEN_FOR_READ()
        : Message(1004, Message::ERROR, "Failed to open file '$0' for read.")
    {}
};

class INFO_READING_FILE : public Log::Message {
public:
    INFO_READING_FILE()
        : Message(1005, Message::INFO, "Reading file '$0'")
    {}
};

class ERROR_FAILED_TO_REMOVE_DIRECTORY : public Log::Message {
public:
    ERROR_FAILED_TO_REMOVE_DIRECTORY()
        : Message(1006, Message::ERROR, "Failed to remove the directory '$0'.")
    {}
};

class ERROR_FAILED_TO_OPEN_FOR_WRITE : public Log::Message {
public:
    ERROR_FAILED_TO_OPEN_FOR_WRITE()
        : Message(1007, Message::ERROR, "Failed to open file '$0' for write.")
    {}
};

class FATAL_NO_TCL_INTERP : public Log::Message {
public:
    FATAL_NO_TCL_INTERP()
        : Message(1008, Message::FATAL, "Failed to create Tcl interpreter.")
    {}
};


}

#endif
