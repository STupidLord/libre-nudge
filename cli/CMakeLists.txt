# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2026 STupidLord

cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

add_executable(cli)

set_property(TARGET cli PROPERTY OUTPUT_NAME "libre-nudge")

set_property(TARGET cli PROPERTY CXX_STANDARD 23)
set_property(TARGET cli PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET cli PROPERTY CXX_EXTENSIONS OFF)

set_property(TARGET cli PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libre-nudge)
set_property(TARGET cli PROPERTY LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libre-nudge)
set_property(TARGET cli PROPERTY ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/cli)
set_property(TARGET cli PROPERTY PDB_OUTPUT_DIRECTORY     ${CMAKE_BINARY_DIR}/cli)

set_target_properties(cli
    PROPERTIES
        VERSION 0.1.0
)

target_link_libraries(cli PRIVATE core)

target_include_directories(cli PRIVATE "include")

target_sources(cli
    PRIVATE
        "src/main.cpp"
        "src/common.cpp"
        "src/arg.cpp"
        "src/ini.cpp"
    PRIVATE
        FILE_SET HEADERS
        FILES
            "include/version.hpp"
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
