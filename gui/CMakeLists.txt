# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2026 STupidLord

cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

include(FetchContent)
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG 73db193f853e2ee079bf3ca8a64aa2eaf6459043
    SOURCE_SUBDIR cmake
)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG d9d6f0f1f967807ffade6598ea9a631ebaf37a56
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(glad)

glad_add_library(glad_generated STATIC API gl:core=4.6) 

set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui")

add_library(imgui STATIC)

target_sources(imgui
    PRIVATE
        "${IMGUI_DIR}/imgui.cpp"
        "${IMGUI_DIR}/imgui_demo.cpp"
        "${IMGUI_DIR}/imgui_draw.cpp"
        "${IMGUI_DIR}/imgui_tables.cpp"
        "${IMGUI_DIR}/imgui_widgets.cpp"

        "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
        "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
    PUBLIC
        "${IMGUI_DIR}/imconfig.h"
        "${IMGUI_DIR}/imgui.h"
        "${IMGUI_DIR}/imgui_internal.h"

        "${IMGUI_DIR}/imstb_rectpack.h"
        "${IMGUI_DIR}/imstb_textedit.h"
        "${IMGUI_DIR}/imstb_truetype.h"

        "${IMGUI_DIR}/backends/imgui_impl_glfw.h"
        "${IMGUI_DIR}/backends/imgui_impl_opengl3.h"
)

target_include_directories(imgui
    PUBLIC
        "${IMGUI_DIR}"
        "${IMGUI_DIR}/backends"
)

add_executable(gui)

set_property(TARGET gui PROPERTY OUTPUT_NAME "libre-nudge-gui")

set_property(TARGET gui PROPERTY CXX_STANDARD 23)
set_property(TARGET gui PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET gui PROPERTY CXX_EXTENSIONS OFF)

set_property(TARGET gui PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libre-nudge/$<CONFIG>/bin)
set_property(TARGET gui PROPERTY LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libre-nudge/$<CONFIG>/bin)
set_property(TARGET gui PROPERTY ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/gui)
set_property(TARGET gui PROPERTY PDB_OUTPUT_DIRECTORY     ${CMAKE_BINARY_DIR}/gui)

set_target_properties(gui
    PROPERTIES
        VERSION 0.1.0
)

get_target_property(GUI_VERSION gui VERSION)
string(REPLACE "." ";" VERSION_LIST "${GUI_VERSION}")
list(GET VERSION_LIST 0 GUI_VERSION_MAJOR)
list(GET VERSION_LIST 1 GUI_VERSION_MINOR)
list(GET VERSION_LIST 2 GUI_VERSION_PATCH)
configure_file(
    "${CMAKE_SOURCE_DIR}/gui/gui_version.hpp.in"
    "${CMAKE_BINARY_DIR}/gui_version.hpp"
    @ONLY
)

target_link_libraries(
    imgui
        PRIVATE
            glfw
)

target_link_libraries(
    gui
        PRIVATE
            core
            libre-nudge-config
            glad_generated
            glfw
            imgui
)

target_include_directories(gui PRIVATE "include")

target_sources(gui
    PRIVATE
        "src/main.cpp"
    # PRIVATE
    #     FILE_SET HEADERS
    #     FILES
)

if(WIN32)
    target_sources(gui
        PRIVATE
            "resources.rc"
    )
endif()
