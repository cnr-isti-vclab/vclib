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

#include <vclib/space/core/bit_set.h>
#include <vclib/types.h>

#include <array>

namespace vcl {

namespace detail {

template<typename Enum>
auto constexpr makeExclusiveReangesArray(auto... args)
{
    std::array<Enum, sizeof...(args)> array;

    std::size_t i = 0;
    ((array[i++] = args), ...);

    return array;
};

} // namespace detail

/**
 * @brief The MeshRenderInfo class is a collection of rendering settings for a
 * Mesh.
 *
 * It provides a set of enums that can be used for various rendering purposes
 * (like rendering settings, render buffer lists, ...).
 *
 * It also allows to store settings that can be used for draw capability
 * or draw mode of a mesh.
 */
class MeshRenderInfo
{
public:
    enum class Primitive {
        POINTS = 0,
        SURFACE,
        WIREFRAME,
        EDGES,

        COUNT
    };

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

    enum class Surface {
        VISIBLE          = VCL_MRS_DRAW_SURF,
        SHADING_NONE     = VCL_MRS_SURF_SHADING_NONE,
        SHADING_FLAT     = VCL_MRS_SURF_SHADING_FLAT,
        SHADING_SMOOTH   = VCL_MRS_SURF_SHADING_SMOOTH,
        COLOR_VERTEX     = VCL_MRS_SURF_COLOR_VERTEX,
        COLOR_FACE       = VCL_MRS_SURF_COLOR_FACE,
        COLOR_VERTEX_TEX = VCL_MRS_SURF_TEX_VERTEX,
        COLOR_WEDGE_TEX  = VCL_MRS_SURF_TEX_WEDGE,
        COLOR_MESH       = VCL_MRS_SURF_COLOR_MESH,
        COLOR_USER       = VCL_MRS_SURF_COLOR_USER,

        COUNT
    };

    enum class Wireframe {
        VISIBLE      = VCL_MRS_DRAW_WIREFRAME,
        SHADING_NONE = VCL_MRS_WIREFRAME_SHADING_NONE,
        SHADING_VERT = VCL_MRS_WIREFRAME_SHADING_VERT,
        COLOR_VERTEX = VCL_MRS_WIREFRAME_COLOR_VERT,
        COLOR_MESH   = VCL_MRS_WIREFRAME_COLOR_MESH,
        COLOR_USER   = VCL_MRS_WIREFRAME_COLOR_USER,

        COUNT
    };

    enum class Edges {
        VISIBLE        = VCL_MRS_DRAW_EDGES,
        SHADING_NONE   = VCL_MRS_EDGES_SHADING_NONE,
        SHADING_FLAT   = VCL_MRS_EDGES_SHADING_FLAT,
        SHADING_SMOOTH = VCL_MRS_EDGES_SHADING_SMOOTH,
        COLOR_VERTEX   = VCL_MRS_EDGES_COLOR_VERTEX,
        COLOR_EDGE     = VCL_MRS_EDGES_COLOR_EDGE,
        COLOR_MESH     = VCL_MRS_EDGES_COLOR_MESH,
        COLOR_USER     = VCL_MRS_EDGES_COLOR_USER,

        COUNT
    };

    bool visible;

    // settings for each primitive
    BitSet16 points;
    BitSet16 surface;
    BitSet16 wireframe;
    BitSet16 edges;

    void reset()
    {
        visible = false;
        points.reset();
        surface.reset();
        wireframe.reset();
        edges.reset();
    }

    bool operator==(const MeshRenderInfo& o) const
    {
        return visible == o.visible && points == o.points &&
               surface == o.surface && wireframe == o.wireframe &&
               edges == o.edges;
    }

    MeshRenderInfo& operator&=(const MeshRenderInfo& o)
    {
        visible &= o.visible;
        points &= o.points;
        surface &= o.surface;
        wireframe &= o.wireframe;
        edges &= o.edges;
        return *this;
    }

    MeshRenderInfo& operator|=(const MeshRenderInfo& o)
    {
        visible |= o.visible;
        points |= o.points;
        surface |= o.surface;
        wireframe |= o.wireframe;
        edges |= o.edges;
        return *this;
    }

    MeshRenderInfo& operator^=(const MeshRenderInfo& o)
    {
        visible ^= o.visible;
        points ^= o.points;
        surface ^= o.surface;
        wireframe ^= o.wireframe;
        edges ^= o.edges;
        return *this;
    }

    template<Primitive PRIMITIVE>
    constexpr static auto exclusiveRange(auto value)
    {
        const auto& ranges = exclusiveRanges<PRIMITIVE>();
        return getExclusiveRange(value, ranges);
    }

    constexpr static auto pointsExclusiveRange(auto value)
    {
        return exclusiveRange<Primitive::POINTS>(value);
    }

    constexpr static auto surfaceExclusiveRange(auto value)
    {
        return exclusiveRange<Primitive::SURFACE>(value);
    }

    constexpr static auto wireframeExclusiveRange(auto value)
    {
        return exclusiveRange<Primitive::WIREFRAME>(value);
    }

    constexpr static auto edgesExclusiveRange(auto value)
    {
        return exclusiveRange<Primitive::EDGES>(value);
    }

private:
    inline static constexpr const auto POINTS_EXCLUSIVE_RANGES =
        detail::makeExclusiveReangesArray<Points>(
            Points::SHAPE_PIXEL,
            Points::SHAPE_CIRCLE,
            Points::SHADING_NONE,
            Points::SHADING_VERT,
            Points::COLOR_VERTEX,
            Points::COLOR_USER);

    inline static constexpr const auto SURFACE_EXCLUSIVE_RANGES =
        detail::makeExclusiveReangesArray<Surface>(
            Surface::SHADING_NONE,
            Surface::SHADING_SMOOTH,
            Surface::COLOR_VERTEX,
            Surface::COLOR_USER);

    inline static constexpr const auto WIREFRAME_EXCLUSIVE_RANGES =
        detail::makeExclusiveReangesArray<Wireframe>(
            Wireframe::SHADING_NONE,
            Wireframe::SHADING_VERT,
            Wireframe::COLOR_VERTEX,
            Wireframe::COLOR_USER);

    inline static constexpr const auto EDGES_EXCLUSIVE_RANGES =
        detail::makeExclusiveReangesArray<Edges>(
            Edges::SHADING_NONE,
            Edges::SHADING_SMOOTH,
            Edges::COLOR_VERTEX,
            Edges::COLOR_USER);

    template<Primitive PRIMITIVE>
    inline static constexpr auto& exclusiveRanges()
    {
        if constexpr (PRIMITIVE == Primitive::POINTS) {
            return POINTS_EXCLUSIVE_RANGES;
        }
        else if constexpr (PRIMITIVE == Primitive::SURFACE) {
            return SURFACE_EXCLUSIVE_RANGES;
        }
        else if constexpr (PRIMITIVE == Primitive::WIREFRAME) {
            return WIREFRAME_EXCLUSIVE_RANGES;
        }
        else if constexpr (PRIMITIVE == Primitive::EDGES) {
            return EDGES_EXCLUSIVE_RANGES;
        }
        else {
            static_assert(PRIMITIVE, "Unknown Primitive");
        }
    }

    constexpr static auto getExclusiveRange(auto value, const auto& array)
    {
        for (uint i = 0; i < array.size(); i += 2) {
            if (toUnderlying(value) >= toUnderlying(array[i]) &&
                toUnderlying(value) <= toUnderlying(array[i + 1])) {
                return std::pair(
                    toUnderlying(array[i]), toUnderlying(array[i + 1]));
            }
        }
        return std::pair(toUnderlying(value), toUnderlying(value));
    }
};

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_INFO_H
