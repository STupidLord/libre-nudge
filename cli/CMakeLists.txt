# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2026 STupidLord

cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

add_executable(cli)

set_property(TARGET cli PROPERTY OUTPUT_NAME "libre-nudge")

set_property(TARGET cli PROPERTY CXX_STANDARD 23)
set_property(TARGET cli PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET cli PROPERTY CXX_EXTENSIONS OFF)

set_property(TARGET cli PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libre-nudge/$<CONFIG>/bin)
set_property(TARGET cli PROPERTY LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libre-nudge/$<CONFIG>/bin)
set_property(TARGET cli PROPERTY ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/cli)
set_property(TARGET cli PROPERTY PDB_OUTPUT_DIRECTORY     ${CMAKE_BINARY_DIR}/cli)

set_target_properties(cli
    PROPERTIES
        VERSION 0.1.2
)

get_target_property(CLI_VERSION cli VERSION)
string(REPLACE "." ";" VERSION_LIST "${CLI_VERSION}")
list(GET VERSION_LIST 0 CLI_VERSION_MAJOR)
list(GET VERSION_LIST 1 CLI_VERSION_MINOR)
list(GET VERSION_LIST 2 CLI_VERSION_PATCH)
configure_file(
    "${CMAKE_SOURCE_DIR}/cli/cli_version.hpp.in"
    "${CMAKE_BINARY_DIR}/cli_version.hpp"
    @ONLY
)

target_link_libraries(
    cli
        PRIVATE core
        PRIVATE libre-nudge-config
)

target_include_directories(cli PRIVATE "include")

target_sources(cli
    PRIVATE
        "src/main.cpp"
        "src/common.cpp"
        "src/arg.cpp"
        "src/arg_flags.cpp"
        "src/ini.cpp"
    PRIVATE
        FILE_SET HEADERS
        FILES
            "include/common.hpp"
            "include/arg.hpp"
            "include/ini.hpp"
)

if(WIN32)
    target_sources(cli
        PRIVATE
            "resources.rc"
    )
endif()

add_custom_command(
    TARGET cli
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:cli>" "$<TARGET_FILE_DIR:cli>/nudge$<TARGET_FILE_SUFFIX:cli>"
    COMMENT "Creating alias 'nudge' for 'libre-nudge'"
)
