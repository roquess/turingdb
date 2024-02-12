#pragma once

#include "Message.h"

namespace msg {

MSG_INFO(INFO_RPC_SERVER_STARTED, 6001,
         "RPC Server started on $0")

MSG_ERROR(ERROR_RPC_SERVER_FAILED_TO_START, 6002,
          "RPC Server failed to start on $0")

MSG_ERROR(ERROR_RPC_DB_ALREADY_LOADED, 6003,
          "Could not load db \"$0\": alread loaded")

MSG_ERROR(ERROR_RPC_DB_DOES_NOT_EXIST, 6004,
          "Could not load db \"$0\": does not exist")

MSG_ERROR(ERROR_RPC_DURING_LOADING, 6005,
          "Could not load db \"$0\": error during import")
}
