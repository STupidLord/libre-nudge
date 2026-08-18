# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2026 STupidLord

cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

project(
    libre-nudge
    VERSION 0.1.1
    LANGUAGES CXX
)

# By default we use the version up there,
# but it makes it easier in CI/CD 
option(USE_GIT_VERSION "Use version provided by Git tag" OFF)

if(USE_GIT_VERSION)
    # Grabs most recent Git tag
    execute_process(
        COMMAND git describe --tags --abbrev=0
        OUTPUT_VARIABLE GIT_TAG_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    # Then strips v from it if it's there,
    # or defaults if not (or requested)
    if(NOT GIT_TAG_VERSION)
        set(GIT_TAG_VERSION ${PROJECT_VERSION})
    else()
        string(REGEX REPLACE "^v" "" GIT_TAG_VERSION "${GIT_TAG_VERSION}")
    endif()

    project(libre-nudge VERSION ${GIT_TAG_VERSION})
endif()


if(WIN32)
    enable_language(RC)
endif()

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_property(GLOBAL PROPERTY CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(/permissive-)
endif()

configure_file(
    "${CMAKE_SOURCE_DIR}/project_version.hpp.in"
    "${CMAKE_BINARY_DIR}/project_version.hpp"
    @ONLY
)

add_library(libre-nudge-config INTERFACE)

target_include_directories(
    libre-nudge-config
        INTERFACE
            "${CMAKE_BINARY_DIR}"
)

add_subdirectory(core)
add_subdirectory(cli)

install(TARGETS core cli
    RUNTIME DESTINATION .
    LIBRARY DESTINATION .
)

install(
    FILES "$<TARGET_FILE_DIR:cli>/nudge$<TARGET_FILE_SUFFIX:cli>"
    DESTINATION .
)
