# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.24)
project(ZipViews)

option(ZIP_VIEW_BUILD_TEST "Build test" ON)

add_library(zip-view INTERFACE)
target_include_directories(zip-view INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(zip-view INTERFACE cxx_std_20)

if(ZIP_VIEW_BUILD_TEST)
    add_executable(test-zip-view test_zip_view.cpp)
    target_link_libraries(test-zip-view zip-view)

    add_test(test-zip-view test-zip-view)
endif()
