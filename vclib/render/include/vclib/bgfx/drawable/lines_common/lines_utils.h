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

#ifndef VCL_BGFX_DRAWABLE_LINES_LINES_UTILS_H
#define VCL_BGFX_DRAWABLE_LINES_LINES_UTILS_H

#include <bit>
#include <cmath>
#include <cstdint>

namespace vcl {

struct LinesVertex
{
    float X, Y, Z;
    float color;
    float xN, yN, zN;

    LinesVertex() = default;

    LinesVertex(
        float    x,
        float    y,
        float    z,
        uint32_t color = COLOR(1, 1, 1, 1),
        float    xn    = 0,
        float    yn    = 0,
        float    zn    = 0) :
            X(x), Y(y), Z(z), color(std::bit_cast<float>(color)), xN(xn),
            yN(yn), zN(zn)
    {
    }

    uint32_t getRGBAColor() const { return std::bit_cast<uint32_t>(color); }

    uint32_t getABGRColor() const
    {
        uint32_t uint_color = getRGBAColor();

        return ((uint_color & 0xFF000000) >> 24) |
               ((uint_color & 0x00FF0000) >> 8) |
               ((uint_color & 0x0000FF00) << 8) |
               ((uint_color & 0x000000FF) << 24);
    }

    static inline uint32_t COLOR(float r, float g, float b, float a)
    {
        return static_cast<uint32_t>(
            (static_cast<uint8_t>(std::round((r) * 255)) << 24) |
            (static_cast<uint8_t>(std::round((g) * 255)) << 16) |
            (static_cast<uint8_t>(std::round((b) * 255)) << 8) |
            (static_cast<uint8_t>(std::round((a) * 255))));
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_LINES_LINES_UTILS_H
