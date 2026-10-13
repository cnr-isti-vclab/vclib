# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

if(
    NOT VCLIB_INSTALL_MODULE_CORE
    AND NOT VCLIB_INSTALL_MODULE_EXTERNAL
    AND NOT VCLIB_INSTALL_MODULE_RENDER
)
    return()
endif()

# General package metadata
set(CPACK_PACKAGE_NAME "vclib")
set(CPACK_PACKAGE_VENDOR "Visual Computing Lab, ISTI - CNR")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Visual Computing Library")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://alemuntoni.github.io/vclib")
set(CPACK_PACKAGE_CONTACT "Alessandro Muntoni <alessandro.muntoni@cnr.it>")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
    "${CMAKE_CURRENT_BINARY_DIR}/LICENSE.txt"
    COPYONLY
)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_BINARY_DIR}/LICENSE.txt")

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

if(WIN32)
    set(CPACK_GENERATOR "NSIS")

    # NSIS specific settings
    set(CPACK_PACKAGE_FILE_NAME "vclib-${PROJECT_VERSION}-windows-x86_64")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "VCLib")
    set(CPACK_NSIS_DISPLAY_NAME "VCLib")
    set(CPACK_NSIS_PACKAGE_NAME "VCLib")
    set(CPACK_NSIS_HELP_LINK "https://alemuntoni.github.io/vclib")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://alemuntoni.github.io/vclib")
    set(CPACK_NSIS_CONTACT "alessandro.muntoni@cnr.it")
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)

    # WIX specific settings
    set(CPACK_WIX_UPGRADE_GUID "B6539080-945B-4EB3-A6BE-0D4C44E7293A")
    set(CPACK_WIX_PRODUCT_DESC "Visual Computing Library")

elseif(APPLE)
    # macOS packaging will be handled via Homebrew Tap
    return()

else() # Linux
    set(CPACK_GENERATOR "DEB")
    set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")

    # Architecture mapping
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64|AMD64")
        set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64|ARM64")
        set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "arm64")
    endif()

    # Debian package settings
    set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
    set(CPACK_DEBIAN_PACKAGE_NAME "vclib")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER
        "Alessandro Muntoni <alessandro.muntoni@cnr.it>"
    )
    set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
    set(CPACK_DEBIAN_PACKAGE_DEPENDS
        "libeigen3-dev, libboost-dev, libcgal-dev, libglfw3-dev, libtbb-dev, libembree-dev, libgmp-dev, libmpfr-dev, qt6-base-dev, libgl1-mesa-dev, libwayland-dev, libxkbcommon-dev"
    )

    # RPM package settings
    set(CPACK_RPM_PACKAGE_NAME "vclib")
    set(CPACK_RPM_FILE_NAME RPM-DEFAULT)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64|AMD64")
        set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64|ARM64")
        set(CPACK_RPM_PACKAGE_ARCHITECTURE "aarch64")
    endif()
    set(CPACK_RPM_PACKAGE_REQUIRES
        "eigen3-devel, boost-devel, CGAL-devel, glfw-devel, tbb-devel, embree-devel, gmp-devel, mpfr-devel, qt6-qtbase-devel, freeglut-devel, mesa-libGL-devel, libXi-devel, libXinerama-devel, libXcursor-devel, libXrandr-devel, wayland-devel, wayland-protocols-devel, libxkbcommon-devel"
    )
endif()

include(CPack)
