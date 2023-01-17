# femtolog
# Copyright (c) 2021-2022, SanCloud Ltd
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.10)
project(femtolog VERSION 0.2.1)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_PREDEFINED DOXYGEN)
    set(DOXYGEN_SHOW_FILES NO)
    set(DOXYGEN_EXPAND_AS_DEFINED YES)
    set(DOXYGEN_MACRO_EXPANSION YES)
    doxygen_add_docs(
        docs
        src/femtolog.h
    )
endif(DOXYGEN_FOUND)

add_definitions(-DFEMTOLOG_VERSION="${femtolog_VERSION}")

add_library(femtolog STATIC src/femtolog.c)
add_executable(femtolog-example src/femtolog-example.c)
target_link_libraries(femtolog-example femtolog)

if(FEMTOLOG_INSTALL)
    include(GNUInstallDirs)
    install(TARGETS femtolog LIBRARY)
    install(TARGETS femtolog-example RUNTIME)
    install(FILES src/femtolog.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()
