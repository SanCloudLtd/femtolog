# femtolog
# Copyright (c) 2021-2022, SanCloud Ltd
# All Rights Reserved

cmake_minimum_required(VERSION 3.10)
project(femtolog VERSION 0.1.0)

add_definitions(-DFEMTOLOG_VERSION="${femtolog_VERSION}")

add_library(femtolog STATIC src/femtolog.c)

include(GNUInstallDirs)
install(TARGETS femtolog LIBRARY)
install(FILES src/femtolog.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
