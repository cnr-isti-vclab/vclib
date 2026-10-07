# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

macro(vclib_begin_3rdparty_install_scope)
    set(OLD_CMAKE_INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}")
    set(OLD_CMAKE_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}")
    set(OLD_CMAKE_INSTALL_BINDIR "${CMAKE_INSTALL_BINDIR}")
    set(OLD_CMAKE_INSTALL_DATADIR "${CMAKE_INSTALL_DATADIR}")

    set(CMAKE_INSTALL_LIBDIR "${VCLIB_3RDPARTY_INSTALL_LIBDIR}")
    set(CMAKE_INSTALL_INCLUDEDIR "${VCLIB_3RDPARTY_INSTALL_INCLUDEDIR}")
    set(CMAKE_INSTALL_BINDIR "${VCLIB_3RDPARTY_INSTALL_BINDIR}")
    set(CMAKE_INSTALL_DATADIR "${VCLIB_3RDPARTY_INSTALL_DATADIR}")
endmacro()

macro(vclib_end_3rdparty_install_scope)
    set(CMAKE_INSTALL_LIBDIR "${OLD_CMAKE_INSTALL_LIBDIR}")
    set(CMAKE_INSTALL_INCLUDEDIR "${OLD_CMAKE_INSTALL_INCLUDEDIR}")
    set(CMAKE_INSTALL_BINDIR "${OLD_CMAKE_INSTALL_BINDIR}")
    set(CMAKE_INSTALL_DATADIR "${OLD_CMAKE_INSTALL_DATADIR}")
endmacro()
