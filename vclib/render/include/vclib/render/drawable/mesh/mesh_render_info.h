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
    /**
     * @brief List of primitives for which settings can be stored.
     */
    enum class Primitive {
        POINTS = 0,
        SURFACE,
        WIREFRAME,
        EDGES,

        COUNT
    };

private:
    bool mVisible;

    // settings for each primitive
    std::array<BitSet16, toUnderlying(Primitive::COUNT)> mSettings;

public:
    /**
     * @brief List of possible settings for the points primitive.
     */
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

    /**
     * @brief List of possible settings for the surface primitive.
     */
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

    /**
     * @brief List of possible settings for the wireframe primitive.
     */
    enum class Wireframe {
        VISIBLE      = VCL_MRS_DRAW_WIREFRAME,
        SHADING_NONE = VCL_MRS_WIREFRAME_SHADING_NONE,
        SHADING_VERT = VCL_MRS_WIREFRAME_SHADING_VERT,
        COLOR_VERTEX = VCL_MRS_WIREFRAME_COLOR_VERT,
        COLOR_MESH   = VCL_MRS_WIREFRAME_COLOR_MESH,
        COLOR_USER   = VCL_MRS_WIREFRAME_COLOR_USER,

        COUNT
    };

    /**
     * @brief List of possible settings for the edges primitive.
     */
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

    /**
     * @brief Returns the visibility status of the mesh.
     * @return the visibility status of the mesh.
     */
    bool visible() const { return mVisible; }

    /**
     * @brief Sets the visibility status of the mesh.
     * @return the visibility status of the mesh.
     */
    bool& visible() { return mVisible; }

    /**
     * @brief Returns the settings for a given primitive.
     * @tparam PRIMITIVE: the primitive for which to get the settings.
     * @return the settings for the given primitive.
     */
    template<Primitive PRIMITIVE>
    BitSet16 settings() const
    {
        return mSettings[toUnderlying(PRIMITIVE)];
    }

    /**
     * @brief Returns the settings for a given primitive.
     * @tparam PRIMITIVE: the primitive for which to get the settings.
     * @return the settings for the given primitive.
     */
    template<Primitive PRIMITIVE>
    BitSet16& settings()
    {
        return mSettings[toUnderlying(PRIMITIVE)];
    }

    /**
     * @brief Returns the settings for the points primitive.
     * @return the settings for the points primitive.
     */
    BitSet16 points() const { return settings<Primitive::POINTS>(); }

    /**
     * @brief Returns the settings for the points primitive.
     * @return the settings for the points primitive.
     */
    BitSet16& points() { return settings<Primitive::POINTS>(); }

    /**
     * @brief Returns the settings for the surface primitive.
     * @return the settings for the surface primitive.
     */
    BitSet16 surface() const { return settings<Primitive::SURFACE>(); }

    /**
     * @brief Returns the settings for the surface primitive.
     * @return the settings for the surface primitive.
     */
    BitSet16& surface() { return settings<Primitive::SURFACE>(); }

    /**
     * @brief Returns the settings for the wireframe primitive.
     * @return the settings for the wireframe primitive.
     */
    BitSet16 wireframe() const { return settings<Primitive::WIREFRAME>(); }

    /**
     * @brief Returns the settings for the wireframe primitive.
     * @return the settings for the wireframe primitive.
     */
    BitSet16& wireframe() { return settings<Primitive::WIREFRAME>(); }

    /**
     * @brief Returns the settings for the edges primitive.
     * @return the settings for the edges primitive.
     */
    BitSet16 edges() const { return settings<Primitive::EDGES>(); }

    /**
     * @brief Returns the settings for the edges primitive.
     * @return the settings for the edges primitive.
     */
    BitSet16& edges() { return settings<Primitive::EDGES>(); }

    /**
     * @brief Resets all the settings of the mesh.
     */
    void reset()
    {
        mVisible = false;
        for (auto& s : mSettings)
            s.reset();
    }

    bool operator==(const MeshRenderInfo& o) const
    {
        return mVisible == o.mVisible && mSettings == o.mSettings;
    }

    MeshRenderInfo& operator&=(const MeshRenderInfo& o)
    {
        mVisible &= o.mVisible;
        for (const auto& [s, os] : std::views::zip(mSettings, o.mSettings)) {
            s &= os;
        }
        return *this;
    }

    MeshRenderInfo& operator|=(const MeshRenderInfo& o)
    {
        mVisible |= o.mVisible;
        for (const auto& [s, os] : std::views::zip(mSettings, o.mSettings)) {
            s |= os;
        }
        return *this;
    }

    MeshRenderInfo& operator^=(const MeshRenderInfo& o)
    {
        mVisible ^= o.mVisible;
        for (const auto& [s, os] : std::views::zip(mSettings, o.mSettings)) {
            s ^= os;
        }
        return *this;
    }

    /**
     * @brief Given a primitive and a setting, returns pair representing the
     * range in the primitive enumeration of the mutual exclusive settings for
     * the given setting.
     *
     * If the given value does not belong to any range, the function returns a
     * pair having the same value as first and second element.
     *
     * E.g. for a Points primitive, if the setting given is COLOR_USER, the
     * function returns a pair having as first value the first setting of the
     * Points enumeration that starts with COLOR_ and as second value the last
     * setting of the Points enumeration that starts with COLOR_.
     *
     * @tparam PRIMITIVE: the primitive for which to get the exclusive range.
     *
     * @param[in] value: the option to query.
     * @return a pair representing the range of the mutual exclusive settings
     * for the given value.
     */
    template<Primitive PRIMITIVE>
    constexpr static auto exclusiveRange(auto value)
    {
        const auto& ranges = exclusiveRanges<PRIMITIVE>();
        return getExclusiveRange(value, ranges);
    }

    /**
     * @brief Returns pair representing the range in the Points enumeration of
     * the mutual exclusive settings for the given setting.
     *
     * If the given value does not belong to any range, the function returns a
     * pair having the same value as first and second element.
     *
     * E.g. if the setting given is COLOR_USER, the function returns a pair
     * having as first value the first setting of the Points enumeration that
     * starts with COLOR_ and as second value the last setting of the Points
     * enumeration that starts with COLOR_.
     *
     * @param[in] value: the option to query.
     * @return a pair representing the range of the mutual exclusive settings
     * for the given value.
     */
    constexpr static auto pointsExclusiveRange(auto value)
    {
        return exclusiveRange<Primitive::POINTS>(value);
    }

    /**
     * @brief Returns pair representing the range in the Surface enumeration of
     * the mutual exclusive settings for the given setting.
     *
     * If the given value does not belong to any range, the function returns a
     * pair having the same value as first and second element.
     *
     * E.g. if the setting given is COLOR_USER, the function returns a pair
     * having as first value the first setting of the Surface enumeration that
     * starts with COLOR_ and as second value the last setting of the Surface
     * enumeration that starts with COLOR_.
     *
     * @param[in] value: the option to query.
     * @return a pair representing the range of the mutual exclusive settings
     * for the given value.
     */
    constexpr static auto surfaceExclusiveRange(auto value)
    {
        return exclusiveRange<Primitive::SURFACE>(value);
    }

    /**
     * @brief Returns pair representing the range in the Wireframe enumeration of
     * the mutual exclusive settings for the given setting.
     *
     * If the given value does not belong to any range, the function returns a
     * pair having the same value as first and second element.
     *
     * E.g. if the setting given is COLOR_USER, the function returns a pair
     * having as first value the first setting of the Wireframe enumeration that
     * starts with COLOR_ and as second value the last setting of the Wireframe
     * enumeration that starts with COLOR_.
     *
     * @param[in] value: the option to query.
     * @return a pair representing the range of the mutual exclusive settings
     * for the given value.
     */
    constexpr static auto wireframeExclusiveRange(auto value)
    {
        return exclusiveRange<Primitive::WIREFRAME>(value);
    }

    /**
     * @brief Returns pair representing the range in the Edges enumeration of
     * the mutual exclusive settings for the given setting.
     *
     * If the given value does not belong to any range, the function returns a
     * pair having the same value as first and second element.
     *
     * E.g. if the setting given is COLOR_USER, the function returns a pair
     * having as first value the first setting of the Edges enumeration that
     * starts with COLOR_ and as second value the last setting of the Edges
     * enumeration that starts with COLOR_.
     *
     * @param[in] value: the option to query.
     * @return a pair representing the range of the mutual exclusive settings
     * for the given value.
     */
    constexpr static auto edgesExclusiveRange(auto value)
    {
        return exclusiveRange<Primitive::EDGES>(value);
    }

private:
    inline static constexpr const auto POINTS_EXCLUSIVE_RANGES =
        detail::makeExclusiveReangesArray<Points>(
            Points::SHAPE_PIXEL,  // first
            Points::SHAPE_CIRCLE, // last
            Points::SHADING_NONE, // first
            Points::SHADING_VERT, // last
            Points::COLOR_VERTEX, // first
            Points::COLOR_USER);  // last

    inline static constexpr const auto SURFACE_EXCLUSIVE_RANGES =
        detail::makeExclusiveReangesArray<Surface>(
            Surface::SHADING_NONE,   // first
            Surface::SHADING_SMOOTH, // last
            Surface::COLOR_VERTEX,   // first
            Surface::COLOR_USER);    // last

    inline static constexpr const auto WIREFRAME_EXCLUSIVE_RANGES =
        detail::makeExclusiveReangesArray<Wireframe>(
            Wireframe::SHADING_NONE, // first
            Wireframe::SHADING_VERT, // last
            Wireframe::COLOR_VERTEX, // first
            Wireframe::COLOR_USER);  // last

    inline static constexpr const auto EDGES_EXCLUSIVE_RANGES =
        detail::makeExclusiveReangesArray<Edges>(
            Edges::SHADING_NONE,   // first
            Edges::SHADING_SMOOTH, // last
            Edges::COLOR_VERTEX,   // first
            Edges::COLOR_USER);    // last

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
