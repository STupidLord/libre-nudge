# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2026 STupidLord

cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

if(WIN32)
    project(
        libre-nudge
        LANGUAGES CXX RC
    )
else()
    project(
        libre-nudge
        LANGUAGES CXX
    )
endif()

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_property(GLOBAL PROPERTY CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(/permissive-)
endif()

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
