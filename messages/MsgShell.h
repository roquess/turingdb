#pragma once

#include "Message.h"

namespace msg {

MSG_ERROR(ERROR_SHELL_IMPOSSIBLE_TO_CONNECT, 7001,
          "Impossible to connect to Turing database server at $0:$1")

MSG_ERROR(ERROR_SHELL_ERROR_DURING_QUERY_EXECUTION, 7002,
          "The database encountered an error while executing the query")

}
