set(ui_server_sources
            SiteArchive.cpp
            ServerCommand.cpp
            BioserverCommand.cpp
            FlaskCommand.cpp
            ReactDevCommand.cpp
            ReactPrototypeDevCommand.cpp
            ServerCommandEngine.cpp
            TuringUIServer.cpp)

add_library(turing_ui_server_s
    STATIC  ${ui_server_sources})

file(GLOB_RECURSE site_files
            site/package.json
            site/package-lock.json)

add_custom_command(
    OUTPUT  site.tar
            sitePrototype.tar
            SiteArchiveData.h
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../generate_archive.sh
              ${CMAKE_CURRENT_SOURCE_DIR}/../frontend
              ${CMAKE_CURRENT_BINARY_DIR}
              ${CMAKE_CURRENT_SOURCE_DIR}/../frontend_prototype
              ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${site_files}
            ${CMAKE_CURRENT_SOURCE_DIR}/../generate_archive.sh)

add_custom_target(turing_ui_site
    DEPENDS site.tar
            sitePrototype.tar
            SiteArchiveData.h)

add_dependencies(turing_ui_server_s
            turing_ui_site)

target_include_directories(turing_ui_server_s
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
            ${BIO_SOURCE_DIR}/messages
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(turing_ui_server_s
    PRIVATE turing_log_s
            common_s
            incbin)

if(${TURING_DEV})

target_compile_definitions(turing_ui_server_s
    PRIVATE TURINGUI_BACKEND_SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../")

endif()

install(
    DIRECTORY   turingui
    DESTINATION lib/python
    PATTERN     *.swp
    EXCLUDE)
