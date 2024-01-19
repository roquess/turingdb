set(pyclient_sources
    PyUtils.cpp
    PyExceptions.cpp
    PyTuring.cpp
    PyTuringDB.cpp)

Python3_add_library(turingdb2 SHARED ${pyclient_sources})
set_target_properties(turingdb2 PROPERTIES PREFIX "")
target_link_libraries(turingdb2 PRIVATE turing_client_native_fpic_s)

install(TARGETS turingdb2 LIBRARY DESTINATION lib/python)
