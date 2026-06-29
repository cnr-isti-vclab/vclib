// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_CONFIG_H
#define VCL_RENDER_CONFIG_H

// default render backend is bgfx
#if !defined(VCLIB_RENDER_BACKEND_BGFX) && \
    !defined(VCLIB_RENDER_BACKEND_OPENGL2)
#define VCLIB_RENDER_BACKEND_BGFX
#endif

#endif // VCL_RENDER_CONFIG_H
