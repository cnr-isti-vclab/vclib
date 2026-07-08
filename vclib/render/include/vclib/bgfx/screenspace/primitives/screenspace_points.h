// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_SCREENSPACE_PRIMITIVES_SCREENSPACE_POINTS_H
#define VCL_BGFX_SCREENSPACE_PRIMITIVES_SCREENSPACE_POINTS_H

#include <vclib/bgfx/buffers.h>

namespace vcl {

/**
 * @brief Renders a set of 2D points as screen-space splats (squares or
 * circles).
 *
 * Each point is expanded into a quad procedurally in the vertex shader
 * using programmable vertex pulling.
 */
class ScreenSpacePoints
{
    inline static const VertexBuffer NULL_VERTEX_BUFFER;

public:
    /**
     * @brief Specifies how point colors are determined during rendering.
     */
    enum class ColorSetting {
        GENERAL,    ///< All points use a single general color.
        PER_VERTEX, ///< Each point uses color from per-vertex color buffer.
    };

    /**
     * @brief Specifies the visual shape of each point splat.
     */
    enum class Shape {
        SQUARE, ///< Square splats (axis-aligned quads).
        CIRCLE  ///< Circular splats (disk-shaped).
    };

private:
    uint mVerPosCount = 0;
    uint mVerColCount = 0;

    float        mWidth        = 1.0f;
    ColorSetting mColorSetting = ColorSetting::GENERAL;
    Shape        mShape        = Shape::SQUARE;
    Color        mGeneralColor = Color::Black;

    OwnedOrRefBuffer<VertexBuffer> mVertexPositions;
    OwnedOrRefBuffer<VertexBuffer> mVertexColors;

    mutable bool                mIsUpdateProgramNeeded = true;
    mutable bgfx::ProgramHandle mProgram               = BGFX_INVALID_HANDLE;

public:
    /**
     * @brief Default constructor — creates an empty point set with no points.
     */
    ScreenSpacePoints() = default;

    /**
     * @brief Constructs a point set from ranges of 2D coordinates and per-point
     * colors.
     *
     * @param[in] verts: Range of elements convertible to Point2 (must
     * provide x() and y()). Each element contributes one screen-space point at
     * (x, y).
     * @param[in] vertColors: Optional range of Color elements. If non-empty,
     * per-point colors are enabled; the size must match vertCoords. Default is
     * empty (falls back to general color).
     *
     * @note This constructor creates an owned buffer copy of the input data.
     * The original ranges are not referenced after construction.
     */
    template<Range RV, Range RC>
    requires Point2Concept<std::ranges::range_value_t<RV>> &&
             ColorConcept<std::ranges::range_value_t<RC>>
    ScreenSpacePoints(RV&& verts, RC&& vertColors = std::vector<Color>())
    {
        setVertices(verts);
        if (!vertColors.empty()) {
            setVertexColors(vertColors);
        }
    }

    ScreenSpacePoints(
        const uint          vertexCount,
        const VertexBuffer& verts,
        const VertexBuffer& vertColors = NULL_VERTEX_BUFFER);

    /**
     * @brief Returns the width of point splats.
     *
     * @return The width of the point splats in pixels.
     */
    float width() const { return mWidth; }

    /**
     * @brief Returns whether the point set has valid vertex positions.
     *
     * @return True if vertex positions are valid; false otherwise.
     */
    bool hasPositions() const { return mVertexPositions.isValid(); }

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
     * @brief Sets point positions from a range of 2D points.
     *
     * @tparam R: Range whose value type satisfies Point2Concept (must provide
     * x() and y()).
     * @param[in] verts: Range of 2D points. Each element is read as a
     * screen-space coordinate (x, y). The size determines the number of
     * rendered points.
     *
     * @note This creates an owned buffer copy. The original range is not
     * referenced.
     */
    template<Range R>
    requires Point2Concept<std::ranges::range_value_t<R>>
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
            Context::getAllocatedBufferAndReleaseFn<float>(nv * 2);

        for (size_t i = 0; const auto& v : verts) {
            buffer[i * 2 + 0] = v.x();
            buffer[i * 2 + 1] = v.y();
            ++i;
        }

        vertBuff.create(
            buffer,
            nv,
            bgfx::Attrib::Position,
            2,
            PrimitiveType::FLOAT,
            releaseFn);
        mVertexPositions.setOwned(std::move(vertBuff));
        mIsUpdateProgramNeeded = true;
    }

    /**
     * @brief Sets per-point colors from a range of Color elements.
     *
     * @tparam R: Range whose value type satisfies ColorConcept.
     * @param[in] vertColors Range of Color objects. Must have exactly
     * the pointsCount() size. Each color is converted to ABGR format (uint).
     */
    template<Range R>
    requires ColorConcept<std::ranges::range_value_t<R>>
    void setVertexColors(R&& vertColors)
    {
        assert(std::ranges::size(vertColors) == mVerPosCount);

        // Compute padding to ensure the buffer size is a multiple of 16 bytes.
        uint padding = (4 - (mVerPosCount % 4)) % 4;
        uint nv      = mVerPosCount + padding;

        VertexBuffer vColsBuff;

        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<uint>(nv);

        for (uint i = 0; const auto& c : vertColors) {
            buffer[i] = c.abgr();
            ++i;
        }

        vColsBuff.create(
            buffer,
            nv,
            bgfx::Attrib::Color0,
            4,
            PrimitiveType::UCHAR,
            true,
            releaseFn);
        mVertexColors.setOwned(std::move(vColsBuff));
        mIsUpdateProgramNeeded = true;
    }

    void setVertices(const uint vertexCount, const VertexBuffer& verts);

    void setVertexColors(const VertexBuffer& vertColors);

    /**
     * @brief Sets the width of point splats (in screen-space pixels or
     * normalized units).
     * @param[in] width: The splat width value.
     */
    void setWidth(float width) { mWidth = width; }

    /**
     * @brief Sets the color mode for point rendering.
     * @param[in] colorToUse: Whether to use per-point colors or a general
     * uniform color.
     */
    void setColorSetting(ColorSetting colorToUse)
    {
        mColorSetting          = colorToUse;
        mIsUpdateProgramNeeded = true;
    }

    /**
     * @brief Sets the visual shape of each point splat.
     * @param[in] shape: The splat shape (SQUARE or CIRCLE).
     */
    void setShapeSetting(Shape shape)
    {
        mShape                 = shape;
        mIsUpdateProgramNeeded = true;
    }

    /**
     * @brief Sets the general (uniform) color used when color mode is GENERAL.
     * @param[in] generalColor: The fallback color for all points.
     */
    void setGeneralColor(const Color& generalColor)
    {
        mGeneralColor = generalColor;
    }

    void draw(bgfx::ViewId viewId) const;

private:
    void                checkAndUpdateProgram() const;
    bgfx::ProgramHandle screenspacePointsProgramSelector() const;

    static constexpr uint POINTS_POSITIONS_STAGE = 0;
    static constexpr uint POINTS_COLORS_STAGE    = 1;
};

} // namespace vcl

#endif // VCL_BGFX_SCREENSPACE_PRIMITIVES_SCREENSPACE_POINTS_H
