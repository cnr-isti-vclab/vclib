# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

project(vclib-benchmarks)

add_library(vclib-benchmarks-common INTERFACE)

target_include_directories(
    vclib-benchmarks-common
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/common
)

target_link_libraries(vclib-benchmarks-common INTERFACE vclib-3rd-nlohmann_json)

add_subdirectory(core)
