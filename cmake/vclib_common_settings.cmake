# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

### Build settings
set(CMAKE_CXX_STANDARD 20)

# In case of building shared libraries, on windows we need to export all symbols
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)

if(WIN32)
    add_compile_definitions(NOMINMAX)
    if(MSVC)
        # remove possible loss of data warnings in msvc, leave the others
        add_compile_options(/w44244 /w44267 /w44305)
    endif()
endif()

if(VCLIB_INSTALL_MODULE_CORE)
    set(VCLIB_CORE_EXCLUDE_FROM_ALL_OPTION)
else()
    set(VCLIB_CORE_EXCLUDE_FROM_ALL_OPTION EXCLUDE_FROM_ALL)
endif()

if(VCLIB_INSTALL_MODULE_EXTERNAL)
    set(VCLIB_EXTERNAL_EXCLUDE_FROM_ALL_OPTION)
else()
    set(VCLIB_EXTERNAL_EXCLUDE_FROM_ALL_OPTION EXCLUDE_FROM_ALL)
endif()

if(VCLIB_INSTALL_MODULE_RENDER)
    set(VCLIB_RENDER_EXCLUDE_FROM_ALL_OPTION)
else()
    set(VCLIB_RENDER_EXCLUDE_FROM_ALL_OPTION EXCLUDE_FROM_ALL)
endif()

if(VCLIB_INSTALL_MODULE_BINDINGS)
    set(VCLIB_BINDINGS_EXCLUDE_FROM_ALL_OPTION)
else()
    set(VCLIB_BINDINGS_EXCLUDE_FROM_ALL_OPTION EXCLUDE_FROM_ALL)
endif()
