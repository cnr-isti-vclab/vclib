# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

find_package(Boost QUIET)

if(VCLIB_ALLOW_SYSTEM_BOOST AND TARGET Boost::boost)
    message(STATUS "- boost - using system-provided library")
    set(VCLIB_USED_SYSTEM_BOOST ON CACHE INTERNAL "")

    add_library(vclib-3rd-boost INTERFACE)
    target_link_libraries(vclib-3rd-boost INTERFACE Boost::boost)

    list(APPEND VCLIB_EXTERNAL_OPTIONAL_SYSTEM_LIBRARIES vclib-3rd-boost)

    target_compile_definitions(vclib-3rd-boost INTERFACE VCLIB_WITH_BOOST)
elseif(VCLIB_ALLOW_DOWNLOAD_BOOST)
    message(STATUS "- boost - using downloaded source")

    FetchContent_Declare(
        boost
        URL https://archives.boost.io/release/1.91.0/source/boost_1_91_0.zip
        SOURCE_SUBDIR "include" # trick to avoid add_subdir
    )

    FetchContent_MakeAvailable(boost)

    add_library(vclib-3rd-boost INTERFACE)
    target_include_directories(vclib-3rd-boost INTERFACE "${boost_SOURCE_DIR}")

    list(APPEND VCLIB_EXTERNAL_3RDPARTY_LIBRARIES vclib-3rd-boost)

    target_compile_definitions(vclib-3rd-boost INTERFACE VCLIB_WITH_BOOST)

    # Install
    if(VCLIB_ALLOW_INSTALL_BOOST)
        install(
            DIRECTORY "${boost_SOURCE_DIR}/boost"
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )
    endif()
else()
    message(STATUS "- boost - not found, skipping")
endif()
