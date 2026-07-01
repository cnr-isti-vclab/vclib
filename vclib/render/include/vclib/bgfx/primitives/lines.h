// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_PRIMITIVES_LINES_H
#define VCL_BGFX_PRIMITIVES_LINES_H

#include <vclib/bgfx/buffers.h>

namespace vcl {

/**
 * @brief Renders a set of 3D lines as line segments with variable width.
 *
 * Each line segment is rendered using programmable vertex pulling.
 *
 * Lines are rendered using either consecutive vertex pairs or a strip topology.
 * Optional indices can be provided to define the vertex order; if omitted,
 * vertices are used in declaration order.
 */
class Lines
{
public:
    /**
     * @brief Specifies how line colors are determined during rendering.
     */
    enum class ColorSetting {
        PER_VERTEX = 0, ///< Each vertex uses color from per-vertex color buffer
        PER_LINE   = 1, ///< Each line uses color from per-line color buffer
        GENERAL    = 2  ///< All vertices use a single general color
    };

    /**
     * @brief Specifies the topology used for rendering lines.
     *
     * - LINES: Lines are defined by consecutive pairs of vertices in the vertex
     *     buffer. (vertices 0-1 form line 1, vertices 2-3 form line 2, etc.)
     * - LINE_STRIP: Lines are defined by a strip of vertices, where each vertex
     *     after the first forms a line segment with the previous vertex.
     *     (vertices 0-1 form line 1, vertices 1-2 form line 2, etc.)
     *
     * The topology determines how the vertex data is interpreted to form lines.
     * The default is LINES.
     */
    enum class Topology { LINES = 0, LINE_STRIP = 1 };

    /**
     * @brief Specifies how lines are shaded.
     */
    enum class Shading {
        NONE       = 0, ///< No shading applied to lines.
        PER_VERTEX = 1, ///< Lighting computed using vertex normals.
        PER_LINE   = 2  ///< Lighting computed using line normals.
    };

private:
    uint mVerPosCount    = 0;
    uint mIndexCount     = 0;
    uint mVerColCount    = 0;
    uint mLineColorCount = 0;
    uint mVerNorCount    = 0;
    uint mLineNorCount   = 0;

    float        mWidth        = 1.0f;
    Topology     mTopology     = Topology::LINES;
    ColorSetting mColorSetting = ColorSetting::GENERAL;
    Shading      mShading      = Shading::NONE;
    Color        mGeneralColor = Color::Black;
    float        mDepthOffset  = 0.0f;

    OwnedOrRefBuffer<VertexBuffer> mVertexPositions;
    OwnedOrRefBuffer<VertexBuffer> mVertexColors;
    OwnedOrRefBuffer<VertexBuffer> mVertexNormals;

    OwnedOrRefBuffer<IndexBuffer>  mIndices;
    OwnedOrRefBuffer<IndexBuffer>  mLineColors;
    OwnedOrRefBuffer<VertexBuffer> mLineNormals;

    mutable bool mIsUpdateProgramNeeded = true;
    mutable bgfx::ProgramHandle mProgram = BGFX_INVALID_HANDLE;

public:
    /**
     * @brief Default constructor — creates an empty line set with no lines.
     */
    Lines() = default;

    /**
     * @brief Constructs a line set from ranges of 3D coordinates.
     *
     * @param[in] verts: Range of elements convertible to Point3 (must
     * provide x(), y(), and z()).
     *
     * @note This constructor creates an owned buffer copy of the input data.
     * The original ranges are not referenced after construction.
     */
    template<Range RV>
    requires Point3Concept<std::ranges::range_value_t<RV>>
    Lines(RV&& verts)
    {
        setVertices(verts);
    }

    Lines(const uint vertexCount, const VertexBuffer& verts);

    /**
     * @brief Returns the width of line segments.
     *
     * @return The width of the line segments in pixels.
     */
    float width() const { return mWidth; }

    /**
     * @brief Returns the topology used for rendering lines.
     */
    Topology topology() const { return mTopology; }

    /**
     * @brief Returns the color setting used for rendering lines.
     */
    ColorSetting colorSetting() const { return mColorSetting; }

    /**
     * @brief Returns the shading mode used for rendering lines.
     */
    Shading shading() const { return mShading; }

    /**
     * @brief Returns the general color used for rendering lines when
     * ColorSetting is GENERAL.
     */
    Color generalColor() const { return mGeneralColor; }

    /**
     * @brief Returns the depth offset applied to the lines.
     * @return The depth offset applied to the lines.
     */
    float depthOffset() const { return mDepthOffset; }

    /**
     * @brief Returns whether the line set has valid vertex positions.
     */
    bool hasPositions() const { return mVertexPositions.isValid(); }

    /**
     * @brief Returns whether the line set has valid indices.
     */
    bool hasIndices() const { return mIndices.isValid(); }

    /**
     * @brief Returns whether the line set has valid vertex colors.
     */
    bool hasVertexColors() const { return mVertexColors.isValid(); }

    /**
     * @brief Returns whether the line set has valid line colors.
     */
    bool hasLineColors() const { return mLineColors.isValid(); }

    /**
     * @brief Returns whether the line set has valid vertex normals.
     */
    bool hasVertexNormals() const { return mVertexNormals.isValid(); }

    /**
     * @brief Returns whether the line set has valid line normals.
     */
    bool hasLineNormals() const { return mLineNormals.isValid(); }

    /**
     * @brief Returns the number of vertices in the set.
     */
    uint vertexCount() const { return mVerPosCount; }

    /**
     * @brief Returns the number of indices in the set.
     */
    uint indexCount() const { return mIndexCount; }

    /**
     * @brief Sets line coordinates from a range of 3D points.
     *
     * If no indices are set, the interpretation depends on the topology:
     * - For LINES topology, each pair of vertices defines one line segment.
     * - For LINE_STRIP topology, each vertex after the first forms a line
     *   segment with the previous vertex.
     *
     * @tparam R: Range whose value type satisfies Point3Concept (must provide
     * x(), y(), z()).
     * @param[in] verts: Range of 3D points.
     *
     * @note This creates an owned buffer copy. The original range is not
     * referenced.
     */
    template<Range R>
    requires Point3Concept<std::ranges::range_value_t<R>>
    void setVertices(R&& verts)
    {
        mVerPosCount = std::ranges::size(verts);

        uint padding = (4 - (mVerPosCount % 4)) % 4;
        uint nv = mVerPosCount + padding;

        VertexBuffer vertBuff;
        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<float>(nv * 3);

        size_t i = 0;
        for (const auto& v : verts) {
            buffer[i * 3 + 0] = v.x();
            buffer[i * 3 + 1] = v.y();
            buffer[i * 3 + 2] = v.z();
            ++i;
        }

        // Use create for compute binding (vertex pulling)
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
     * @brief Sets line indices from a range of unsigned integers.
     */
    template<Range R>
    requires std::integral<std::ranges::range_value_t<R>>
    void setIndices(R&& indices)
    {
        mIndexCount = std::ranges::size(indices);

        IndexBuffer indexBuff;
        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<uint>(mIndexCount);

        size_t i = 0;
        for (const auto& idx : indices) {
            buffer[i] = idx;
            ++i;
        }

        indexBuff.create(buffer, mIndexCount, releaseFn);

        mIndices.setOwned(std::move(indexBuff));
        mIsUpdateProgramNeeded = true;
    }

    /**
     * @brief Sets per-vertex colors from a range of Color elements.
     */
    template<Range R>
    requires ColorConcept<std::ranges::range_value_t<R>>
    void setVertexColors(R&& vertColors)
    {
        mVerColCount = std::ranges::size(vertColors);

        uint padding = (4 - (mVerColCount % 4)) % 4;
        uint nc = mVerColCount + padding;

        VertexBuffer vColsBuff;
        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<uint>(nc);

        uint i = 0;
        for (const auto& c : vertColors) {
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

    /**
     * @brief Sets per-line colors from a range of Color elements.
     */
    template<Range R>
    requires ColorConcept<std::ranges::range_value_t<R>>
    void setLineColors(R&& linColors)
    {
        mLineColorCount = std::ranges::size(linColors);

        IndexBuffer lColsBuff;
        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<uint>(mLineColorCount);

        uint i = 0;
        for (const auto& c : linColors) {
            buffer[i] = c.abgr();
            ++i;
        }

        lColsBuff.create(buffer, mLineColorCount, releaseFn);
        mLineColors.setOwned(std::move(lColsBuff));
        mIsUpdateProgramNeeded = true;
    }

    /**
     * @brief Sets per-vertex normals from a range of 3D points.
     */
    template<Range R>
    requires Point3Concept<std::ranges::range_value_t<R>>
    void setVertexNormals(R&& vertNormals)
    {
        mVerNorCount = std::ranges::size(vertNormals);

        uint padding = (4 - (mVerNorCount % 4)) % 4;
        uint nn = mVerNorCount + padding;

        VertexBuffer vNormsBuff;
        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<float>(nn * 3);

        size_t i = 0;
        for (const auto& n : vertNormals) {
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
     * @brief Sets per-line normals from a range of 3D points.
     */
    template<Range R>
    requires Point3Concept<std::ranges::range_value_t<R>>
    void setLineNormals(R&& lineNormals)
    {
        mLineNorCount = std::ranges::size(lineNormals);

        uint padding = (4 - (mLineNorCount % 4)) % 4;
        uint nn = mLineNorCount + padding;

        VertexBuffer lNormsBuff;
        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<float>(nn * 3);

        size_t i = 0;
        for (const auto& n : lineNormals) {
            buffer[i * 3 + 0] = n.x();
            buffer[i * 3 + 1] = n.y();
            buffer[i * 3 + 2] = n.z();
            ++i;
        }

        lNormsBuff.create(
            buffer,
            nn,
            bgfx::Attrib::Normal,
            3,
            PrimitiveType::FLOAT,
            releaseFn);
        mLineNormals.setOwned(std::move(lNormsBuff));
        mIsUpdateProgramNeeded = true;
    }

    /**
     * @brief Sets vertex positions by referencing an existing VertexBuffer.
     *
     * @param[in] vertexCount: Number of vertices in the VertexBuffer.
     * @param[in] verts: VertexBuffer containing vertex positions.
     * Expected layout: an array of `float` with 3 components per vertex (x, y,
     * z), stored as consecutive floats: [x0, y0, z0, x1, y1, z1, ..., xn-1,
     * yn-1, zn-1].
     *
     * @note The buffer must remain valid for the lifetime of this object.
     */
    void setVertices(uint vertexCount, const VertexBuffer& verts);

    /**
     * @brief Sets line indices by referencing an existing IndexBuffer.
     *
     * @param[in] indexCount: Number of indices in the IndexBuffer.
     * @param[in] indices: IndexBuffer to use for lines.
     * The interpretation of indices depends on the topology:
     * - For LINES topology, each pair of indices defines one line segment.
     * - For LINE_STRIP topology, each index after the first forms a line
     *   segment with the previous index.
     *
     * @note The buffer must remain valid for the lifetime of this object.
     */
    void setIndices(uint indexCount, const IndexBuffer& indices);

    /**
     * @brief Sets per-vertex colors by referencing an existing VertexBuffer.
     *
     * @param[in] vColsCount: Number of vertex colors in the VertexBuffer.
     * @param[in] vertexColors: VertexBuffer containing per-vertex colors.
     * Expected layout: an array of `uint` with 4 channels per color in
     * ABGR order (A, B, G, R packed as a single 32-bit integer).
     *
     * @note The buffer must remain valid for the lifetime of this object.
     */
    void setVertexColors(uint vColsCount, const VertexBuffer& vertexColors);

    /**
     * @brief Sets per-line colors by referencing an existing IndexBuffer.
     *
     * @param[in] lColorCount: Number of line colors in the IndexBuffer.
     * @param[in] lineColors: IndexBuffer containing per-line colors.
     * Expected layout: an array of `uint` with 4 channels per color in
     * ABGR order (A, B, G, R packed as a single 32-bit integer).
     *
     * @note The buffer must remain valid for the lifetime of this object.
     */
    void setLineColors(uint lColorCount, const IndexBuffer& lineColors);

    /**
     * @brief Sets vertex normals by referencing an existing VertexBuffer.
     *
     * @param[in] vNorCount: Number of vertex normals in the VertexBuffer.
     * @param[in] vertexNormals: VertexBuffer containing vertex normals.
     * Expected layout: an array of `float` with 3 components per vertex (x, y,
     * z), stored as consecutive floats: [x0, y0, z0, x1, y1, z1, ..., xn-1,
     * yn-1, zn-1].
     *
     * @note The buffer must remain valid for the lifetime of this object.
     */
    void setVertexNormals(uint vNorCount, const VertexBuffer& vertexNormals);

    /**
     * @brief Sets per-line normals by referencing an existing VertexBuffer.
     *
     * @param[in] lNorCount: Number of line normals in the VertexBuffer.
     * @param[in] lineNormals: VertexBuffer containing per-line normals.
     * Expected layout: an array of `float` with 3 components per line (x, y,
     * z), stored as consecutive floats: [x0, y0, z0, x1, y1, z1, ..., xn-1,
     * yn-1, zn-1].
     *
     * @note The buffer must remain valid for the lifetime of this object.
     */
    void setLineNormals(uint lNorCount, const VertexBuffer& lineNormals);

    /**
     * @brief Sets the width of line segments.
     * @param[in] width: The line width value.
     */
    void setWidth(float width) { mWidth = width; }

    /**
     * @brief Sets the topology used for rendering lines.
     * @param[in] topo: The desired line topology (LINES or LINE_STRIP).
     */
    void setTopology(Topology topo)
    {
        mTopology              = topo;
        mIsUpdateProgramNeeded = true;
    }

    /**
     * @brief Sets the color mode for line rendering.
     * @param[in] colorToUse: Whether to use per-vertex colors, per-line colors, or general color.
     */
    void setColorSetting(ColorSetting colorToUse)
    {
        mColorSetting          = colorToUse;
        mIsUpdateProgramNeeded = true;
    }

    /**
     * @brief Sets the shading mode for line rendering.
     * @param[in] shading: Whether to apply no shading, per-vertex, or per-line shading.
     */
    void setShading(Shading shading)
    {
        mShading               = shading;
        mIsUpdateProgramNeeded = true;
    }

    /**
     * @brief Sets the general (uniform) color used when color mode is GENERAL.
     * @param[in] generalColor: The fallback color for all lines.
     */
    void setGeneralColor(const Color& generalColor)
    {
        mGeneralColor = generalColor;
    }

    /**
     * @brief Sets the depth offset applied to the lines.
     * @param[in] depthOffset: The depth offset value.
     */
    void setDepthOffset(float depthOffset)
    {
        mDepthOffset = depthOffset;
    }

    void draw(bgfx::ViewId viewId) const;

private:
    static constexpr uint V_POS_STAGE = 0;
    static constexpr uint V_COL_STAGE = 1;
    static constexpr uint L_IND_STAGE = 2;
    static constexpr uint L_COL_STAGE = 3;
    static constexpr uint V_NOR_STAGE = 4;
    static constexpr uint L_NOR_STAGE = 5;

    void checkAndUpdateProgram() const;
    uint vertexPullingInstances() const;
};

} // namespace vcl

#endif // VCL_BGFX_PRIMITIVES_LINES_H
