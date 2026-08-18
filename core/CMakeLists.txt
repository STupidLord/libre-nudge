# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2026 STupidLord

cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

add_library(core SHARED)

set_property(TARGET core PROPERTY OUTPUT_NAME "libre-nudge")

set_property(TARGET core PROPERTY CXX_STANDARD 23)
set_property(TARGET core PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET core PROPERTY CXX_EXTENSIONS OFF)

set_property(TARGET core PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libre-nudge/$<CONFIG>/bin)
set_property(TARGET core PROPERTY LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libre-nudge/$<CONFIG>/bin)
set_property(TARGET core PROPERTY ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/core)
set_property(TARGET core PROPERTY PDB_OUTPUT_DIRECTORY     ${CMAKE_BINARY_DIR}/core)

set_target_properties(core
    PROPERTIES
        VERSION 0.2.3
        SOVERSION 0
)

get_target_property(CORE_VERSION core VERSION)
string(REPLACE "." ";" VERSION_LIST "${CORE_VERSION}")
list(GET VERSION_LIST 0 CORE_VERSION_MAJOR)
list(GET VERSION_LIST 1 CORE_VERSION_MINOR)
list(GET VERSION_LIST 2 CORE_VERSION_PATCH)
configure_file(
    "${CMAKE_SOURCE_DIR}/core/core_version.hpp.in"
    "${CMAKE_BINARY_DIR}/core_version.hpp"
    @ONLY
)

target_include_directories(core PUBLIC "include")
target_include_directories(core PUBLIC "vendor")

target_sources(core
    PRIVATE
        "src/entry.cpp"
        "src/parse.cpp"
        "src/buildings.cpp"
        "src/definition.cpp"
        "src/provinces.cpp"
        "src/image.cpp"
    PRIVATE
        FILE_SET HEADERS
        FILES
            "include/parse.hpp"
            "include/buildings.hpp"
            "include/definition.hpp"
            "include/provinces.hpp"
            "include/image.hpp"
)

if(WIN32)
    target_sources(core
        PRIVATE
            FILE_SET HEADERS
            FILES
                "include/windowsdll.hpp"
    )
endif()

target_compile_definitions(core PRIVATE CORE_EXPORTS)
