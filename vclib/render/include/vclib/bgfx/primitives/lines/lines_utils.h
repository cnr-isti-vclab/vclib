// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_PRIMITIVES_LINES_LINES_UTILS_H
#define VCL_BGFX_PRIMITIVES_LINES_LINES_UTILS_H

#include <bgfx/bgfx.h>

namespace vcl::detail {

inline uint64_t linesDrawState()
{
    return 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
           BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_BLEND_ALPHA;
}

} // namespace vcl::detail

#endif // VCL_BGFX_PRIMITIVES_LINES_LINES_UTILS_H
