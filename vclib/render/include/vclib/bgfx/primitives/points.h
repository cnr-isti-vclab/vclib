/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_BGFX_PRIMITIVES_POINTS_H
#define VCL_BGFX_PRIMITIVES_POINTS_H

#include <vclib/bgfx/buffers.h>

namespace vcl {

/**
 * @brief Renders a set of 3D points in world space as point splats (squares
 * or circles).
 *
 * Each point is positioned using its 3D coordinates and projected through the
 * standard camera pipeline.
 */
class Points
{
    inline static const VertexBuffer NULL_VERTEX_BUFFER;

public:
    /**
     * @brief Specifies how point colors are determined during rendering.
     */
    enum class ColorSetting {
        PER_VERTEX, ///< Each point uses color from per-vertex color buffer.
        GENERAL     ///< All points use a single general color.
    };

    enum class Shading {
        NONE,      ///< No shading applied to points.
        PER_VERTEX ///< Lighting computed using vertex normals (if provided).
    };

    /**
     * @brief Specifies the visual shape of each point splat.
     */
    enum class Shape {
        SQUARE, ///< Square splats (axis-aligned quads in screen space).
        CIRCLE  ///< Circular splats (disk-shaped in screen space).
    };

private:
    uint mVerPosCount = 0;
    uint mVerNorCount = 0;
    uint mVerColCount = 0;

    float        mWidth        = 1.0f;
    ColorSetting mColorSetting = ColorSetting::GENERAL;
    Shading      mShading      = Shading::NONE;
    Shape        mShape        = Shape::SQUARE;
    Color        mGeneralColor = Color::Black;
    float        mDepthOffset  = 0.0f;

    OwnedOrRefBuffer<VertexBuffer> mVertexPositions;
    OwnedOrRefBuffer<VertexBuffer> mVertexNormals;
    OwnedOrRefBuffer<VertexBuffer> mVertexColors;

    mutable bool                mIsUpdateProgramNeeded = true;
    mutable bgfx::ProgramHandle mProgram               = BGFX_INVALID_HANDLE;

public:
    /**
     * @brief Default constructor — creates an empty point set with no points.
     */
    Points() = default;

    /**
     * @brief Constructs a point set from ranges of 3D coordinates, per-point
     * normals and per-point colors.
     *
     * @param[in] verts: Range of elements convertible to Point3 (must
     * provide x(), y(), z()). Each element contributes one world-space point.
     * @param[in] vertNormals: Optional range of elements convertible to Point3.
     * @param[in] vertColors: Optional range of Color elements. If non-empty,
     * per-point colors are enabled; the size must match vertCoords. Default is
     * empty (falls back to general color).
     *
     * @note This constructor creates an owned buffer copy of the input data.
     * The original ranges are not referenced after construction.
     */
    template<Range RV, Range RN, Range RC>
    requires Point3Concept<std::ranges::range_value_t<RV>> &&
             Point3Concept<std::ranges::range_value_t<RN>> &&
             ColorConcept<std::ranges::range_value_t<RC>>
    Points(
        RV&& verts,
        RN&& vertNormals = std::vector<Point3d>(),
        RC&& vertColors  = std::vector<Color>())
    {
        setVertices(verts);
        if (!vertNormals.empty()) {
            setVertexNormals(vertNormals);
        }
        if (!vertColors.empty()) {
            setVertexColors(vertColors);
        }
    }

    Points(
        const uint          vertexCount,
        const VertexBuffer& verts,
        const VertexBuffer& normals    = NULL_VERTEX_BUFFER,
        const VertexBuffer& vertColors = NULL_VERTEX_BUFFER);

    /**
     * @brief Returns the width of point splats.
     *
     * @return The width of the point splats in pixels.
     */
    float width() const { return mWidth; }

    /**
     * @brief Returns the depth offset applied to the points.
     * @return The depth offset applied to the points.
     */
    float depthOffset() const { return mDepthOffset; }

    /**
     * @brief Returns whether the point set has valid vertex positions.
     *
     * @return True if vertex positions are valid; false otherwise.
     */
    bool hasPositions() const { return mVertexPositions.isValid(); }

    /**
     * @brief Returns whether the point set has valid vertex normals.
     *
     * @return True if vertex normals are valid; false otherwise.
     */
    bool hasNormals() const { return mVertexNormals.isValid(); }

    /**
     * @brief Returns whether the point set has valid vertex colors.
     *
     * @return True if vertex colors are valid; false otherwise.
     */
    bool hasColors() const { return mVertexColors.isValid(); }

    /**
     * @brief Returns the number of points in the set.
     *
     * @return The number of points (vertices) in the set.
     */
    uint vertexCount() const { return mVerPosCount; }

    /**
     * @brief Sets point positions from a range of 3D points.
     *
     * @tparam R: Range whose value type satisfies Point3Concept (must provide
     * x(), y(), z()).
     * @param[in] verts: Range of 3D points. Each element is read as a
     * world-space coordinate (x, y, z). The size determines the number of
     * rendered points.
     *
     * @note This creates an owned buffer copy. The original range is not
     * referenced.
     */
    template<Range R>
    requires Point3Concept<std::ranges::range_value_t<R>>
    void setVertices(R&& verts)
    {
        mVerPosCount = std::ranges::size(verts);

        // Compute padding to ensure the buffer size is a multiple of 4 floats
        // (16 bytes). This is required because the vertex shader reads the
        // buffer as vec4 elements.
        uint padding = (4 - (mVerPosCount % 4)) % 4;
        uint nv      = mVerPosCount + padding;

        VertexBuffer vertBuff;
        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<float>(nv * 3);

        for (size_t i = 0; const auto& v : verts) {
            buffer[i * 3 + 0] = v.x();
            buffer[i * 3 + 1] = v.y();
            buffer[i * 3 + 2] = v.z();
            ++i;
        }

        vertBuff.create(
            buffer,
            nv,
            bgfx::Attrib::Position,
            3,
            PrimitiveType::FLOAT,
            releaseFn);
        mVertexPositions.setOwned(std::move(vertBuff));
        mIsUpdateProgramNeeded = true;
    }

    /**
     * @brief Sets per-point normals from a range of 3D points.
     *
     * @tparam R: Range whose value type satisfies Point3Concept (must provide
     * x(), y(), z()).
     * @param[in] vertNormals: Range of 3D points representing normals. Each
     * element is read as a normal vector (x, y, z) for the corresponding point.
     * The size must match vertexCount().
     *
     * @note This creates an owned buffer copy. The original range is not
     * referenced.
     */
    template<Range R>
    requires Point3Concept<std::ranges::range_value_t<R>>
    void setVertexNormals(R&& vertNormals)
    {
        mVerNorCount = std::ranges::size(vertNormals);

        // Compute padding to ensure the buffer size is a multiple of 4 floats
        // (16 bytes). This is required because the vertex shader reads the
        // buffer as vec4 elements.
        uint padding = (4 - (mVerNorCount % 4)) % 4;
        uint nn      = mVerNorCount + padding;

        VertexBuffer vNormsBuff;

        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<float>(nn * 3);

        for (size_t i = 0; const auto& n : vertNormals) {
            buffer[i * 3 + 0] = n.x();
            buffer[i * 3 + 1] = n.y();
            buffer[i * 3 + 2] = n.z();
            ++i;
        }

        vNormsBuff.create(
            buffer,
            nn,
            bgfx::Attrib::Normal,
            3,
            PrimitiveType::FLOAT,
            releaseFn);
        mVertexNormals.setOwned(std::move(vNormsBuff));
        mIsUpdateProgramNeeded = true;
    }

    /**
     * @brief Sets per-point colors from a range of Color elements.
     *
     * @tparam R: Range whose value type satisfies ColorConcept.
     * @param[in] vertColors: Range of Color objects. Must have exactly
     * the vertexCount() size. Each color is converted to ABGR format (uint).
     */
    template<Range R>
    requires ColorConcept<std::ranges::range_value_t<R>>
    void setVertexColors(R&& vertColors)
    {
        mVerColCount = std::ranges::size(vertColors);

        // Compute padding to ensure the buffer size is a multiple of 16 bytes.
        uint padding = (4 - (mVerColCount % 4)) % 4;
        uint nc      = mVerColCount + padding;

        VertexBuffer vColsBuff;

        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<uint>(nc);

        for (uint i = 0; const auto& c : vertColors) {
            buffer[i] = c.abgr();
            ++i;
        }

        vColsBuff.create(
            buffer,
            nc,
            bgfx::Attrib::Color0,
            4,
            PrimitiveType::UCHAR,
            true,
            releaseFn);
        mVertexColors.setOwned(std::move(vColsBuff));
        mIsUpdateProgramNeeded = true;
    }

    void setVertices(const uint vertexCount, const VertexBuffer& verts);

    void setVertexNormals(uint vNorCount, const VertexBuffer& vertNormals);

    void setVertexColors(uint vColsCount, const VertexBuffer& vertColors);

    /**
     * @brief Sets the size of point splats.
     *
     * @param[in] size: The point splat size.
     */
    void setSize(float size) { mWidth = size; }

    /**
     * @brief Sets the color mode for point rendering.
     *
     * @param[in] colorToUse: Whether to use per-point colors or a general
     * uniform color.
     */
    void setColorSetting(ColorSetting colorToUse)
    {
        mColorSetting          = colorToUse;
        mIsUpdateProgramNeeded = true;
    }

    /**
     * @brief Sets the shading mode for point rendering.
     *
     * @param[in] shading: Whether to apply no shading or compute lighting per
     * vertex using normals (if provided).
     */
    void setShading(Shading shading)
    {
        mShading               = shading;
        mIsUpdateProgramNeeded = true;
    }

    /**
     * @brief Sets the visual shape of each point splat.
     *
     * @param[in] shape: The splat shape (SQUARE or CIRCLE).
     */
    void setShape(Shape shape)
    {
        mShape                 = shape;
        mIsUpdateProgramNeeded = true;
    }

    /**
     * @brief Sets the general (uniform) color used when color mode is GENERAL.
     *
     * @param[in] generalColor: The fallback color for all points.
     */
    void setGeneralColor(const Color& generalColor)
    {
        mGeneralColor = generalColor;
    }

    /**
     * @brief Sets the depth offset applied to the points.
     * @param[in] depthOffset: The depth offset value.
     */
    void setDepthOffset(float depthOffset) { mDepthOffset = depthOffset; }

    void draw(bgfx::ViewId viewId) const;

private:
    void                checkAndUpdateProgram() const;
    bgfx::ProgramHandle pointsProgramSelector() const;

    static constexpr uint POINTS_POSITIONS_STAGE = 0;
    static constexpr uint POINTS_NORMALS_STAGE   = 1;
    static constexpr uint POINTS_COLORS_STAGE    = 2;
};

} // namespace vcl

#endif // VCL_BGFX_PRIMITIVES_POINTS_H
