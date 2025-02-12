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

#ifndef VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_INFO_H
#define VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_INFO_H

#include "mesh_render_info_macros.h"

#include <vclib/types.h>

#include <array>

namespace vcl {

namespace detail {

template<typename Enum>
auto constexpr makeExclusiveReangesArray(auto... args) {
    std::array<Enum, sizeof...(args)> array;

    std::size_t i = 0;
    ((array[i++] = args), ...);

    return array;
};

} // namespace detail

struct MeshRenderInfo {
    enum class Points {
        VISIBLE      = VCL_MRS_DRAW_POINTS,
        SHAPE_PIXEL  = VCL_MRS_POINTS_PIXEL,
        SHAPE_CIRCLE = VCL_MRS_POINTS_CIRCLE,
        SHADING_NONE = VCL_MRS_POINTS_SHADING_NONE,
        SHADING_VERT = VCL_MRS_POINTS_SHADING_VERT,
        COLOR_VERTEX = VCL_MRS_POINTS_COLOR_VERTEX,
        COLOR_MESH   = VCL_MRS_POINTS_COLOR_MESH,
        COLOR_USER   = VCL_MRS_POINTS_COLOR_USER,

        COUNT
    };

    constexpr static auto pointsExclusiveRange(auto value)
    {
        return getExclusiveRange(value, POINTS_EXCLUSIVE_RANGES);
    }

    enum class Surface {
        VISIBLE = 0,
        SHADING_NONE,
        SHADING_FLAT,
        SHADING_SMOOTH,
        COLOR_FACE,
        COLOR_VERTEX,
        COLOR_MESH,
        COLOR_USER,
        COLOR_VERTEX_TEX,
        COLOR_WEDGE_TEX,

        COUNT
    };

    enum class Wireframe {
        VISIBLE = 0,
        SHADING_NONE,
        SHADING_VERT,
        COLOR_VERTEX,
        COLOR_MESH,
        COLOR_USER,

        COUNT
    };

    enum class Edges {
        VISIBLE = 0,
        SHADING_NONE,
        SHADING_FLAT,
        SHADING_SMOOTH,
        COLOR_VERTEX,
        COLOR_EDGE,
        COLOR_USER,
        COLOR_MESH,

        COUNT
    };

private:
    inline static constexpr auto const POINTS_EXCLUSIVE_RANGES =
        detail::makeExclusiveReangesArray<Points>(
            Points::SHAPE_PIXEL, Points::SHAPE_CIRCLE,
            Points::SHADING_NONE, Points::SHADING_VERT,
            Points::COLOR_VERTEX, Points::COLOR_USER);

    constexpr static auto getExclusiveRange(auto value, const auto& array)
    {
        for (uint i = 0; i < array.size(); i+=2) {
            if (toUnderlying(value) >= toUnderlying(array[i]) &&
                toUnderlying(value) <= toUnderlying(array[i+1])) {
                return std::pair(
                    toUnderlying(array[i]), toUnderlying(array[i + 1]));
            }
        }
        return std::pair(toUnderlying(value), toUnderlying(value));
    }
};

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_INFO_H
