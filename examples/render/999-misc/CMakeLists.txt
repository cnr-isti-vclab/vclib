# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

if (VCLIB_RENDER_BACKEND STREQUAL "bgfx")
    if (TARGET vclib-3rd-glfw)
        add_subdirectory(bgfx)
        add_subdirectory(bgfx-glfw)
    endif()

    if (TARGET vclib-3rd-qt)
        add_subdirectory(bgfx-qt)
    endif()
endif()

if (TARGET vclib-3rd-qt)
    if (TARGET vclib-3rd-imgui)
        add_subdirectory(broken-bgfx-imgui-qt)
    endif()
endif()
