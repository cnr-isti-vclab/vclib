/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_RENDER_PRIMITIVES_LINES_H
#define VCL_RENDER_PRIMITIVES_LINES_H

#include <bgfx/bgfx.h>

#include <utility>

namespace vcl::detail {

inline uint64_t linesDrawState()
{
    return 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
           BGFX_STATE_DEPTH_TEST_LESS | UINT64_C(0) | BGFX_STATE_BLEND_ALPHA;
}

template<typename T>
inline std::pair<T*, bgfx::ReleaseFn> linesGetAllocatedBufferAndReleaseFn(
    uint size)
{
    T* buffer = new T[size];

    return std::make_pair(buffer, [](void* ptr, void*) {
        delete[] static_cast<T*>(ptr);
    });
}
} // namespace vcl::detail

#endif // VCL_RENDER_PRIMITIVES_LINES_H
