// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/primitives/points.h>

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/uniforms/cross_section_uniforms.h>
#include <vclib/bgfx/primitives/uniforms/points_uniforms.h>

#include <stdexcept>

namespace vcl {

/**
 * @brief Constructs a point set by referencing existing VertexBuffers.
 *
 * @param[in] vertexCount: Number of points.
 * @param[in] verts: VertexBuffer containing point positions.
 * Expected layout: an array of `float` with 3 components per point (x, y,
 * z), stored as consecutive floats: [x0, y0, z0, x1, y1, z1, ..., xn-1,
 * yn-1, zn-1]. The buffer must remain valid for the lifetime of this object.
 * @param[in] normals: Optional VertexBuffer containing per-point normals.
 * Expected layout: an array of `float` with 3 components per normal (x, y,
 * z), stored as consecutive floats: [nx0, ny0, nz0, nx1, ny1, nz1, ...,
 * nxn-1, nyn-1, nzn-1]. The buffer must remain valid for the lifetime of this
 * object.
 * @param[in] vertColors: Optional VertexBuffer containing per-point colors.
 * Expected layout: an array of `uint` with 4 channels per color in ABGR
 * order (A, B, G, R packed as a single 32-bit integer). The buffer must remain
 * valid for the lifetime of this object.
 */
Points::Points(
    const uint          vertexCount,
    const VertexBuffer& verts,
    const VertexBuffer& normals,
    const VertexBuffer& vertColors)
{
    setVertices(vertexCount, verts);
    if (normals.isValid()) {
        setVertexNormals(vertexCount, normals);
    }
    if (vertColors.isValid()) {
        setVertexColors(vertexCount, vertColors);
    }
}

/**
 * @brief Sets point positions by referencing an existing VertexBuffer.
 *
 * @param[in] vertexCount: Number of points in the VertexBuffer.
 * @param[in] verts: VertexBuffer containing point positions.
 * Expected layout: an array of `float` with 3 components per point (x, y,
 * z), stored as consecutive floats: [x0, y0, z0, x1, y1, z1, ..., xn-1,
 * yn-1, zn-1]. The buffer must remain valid for the lifetime of this object.
 */
void Points::setVertices(const uint vertexCount, const VertexBuffer& verts)
{
    mVerPosCount = vertexCount;
    mVertexPositions.setReferenced(&verts);
    mIsUpdateProgramNeeded = true;
}

/**
 * @brief Sets per-point normals by referencing an existing VertexBuffer.
 *
 * @param[in] vNorCount: Number of normals in the VertexBuffer.
 * @param[in] vertNormals: VertexBuffer containing per-point normals.
 * Expected layout: an array of `float` with 3 components per normal (x, y,
 * z), stored as consecutive floats: [nx0, ny0, nz0, nx1, ny1, nz1, ...,
 * nxn-1, nyn-1, nzn-1]. The buffer must remain valid for the lifetime of this
 * object.
 */
void Points::setVertexNormals(uint vNorCount, const VertexBuffer& vertNormals)
{
    mVerNorCount = vNorCount;
    mVertexNormals.setReferenced(&vertNormals);
    mIsUpdateProgramNeeded = true;
}

/**
 * @brief Sets per-point colors by referencing an existing VertexBuffer.
 *
 * @param[in] vColsCount: Number of colors in the VertexBuffer.
 * @param[in] vertColors: VertexBuffer containing per-point colors.
 * Expected layout: an array of `uint` with 4 channels per color in
 * ABGR order (A, B, G, R packed as a single 32-bit integer). The buffer
 * must remain valid for the lifetime of this object.
 */
void Points::setVertexColors(uint vColsCount, const VertexBuffer& vertColors)
{
    mVerColCount = vColsCount;
    mVertexColors.setReferenced(&vertColors);
    mIsUpdateProgramNeeded = true;
}

/**
 * @brief Sets per-point selection state by referencing an existing
 * BooleanBuffer.
 *
 * @param[in] vSelCount: Number of selection states in the BooleanBuffer.
 * @param[in] vertSels: The BooleanBuffer containing selection state.
 */
void Points::setSelection(uint vSelCount, const BooleanBuffer& vertSels)
{
    mVerSelCount = vSelCount;
    mSelectionBuffer.setReferenced(&vertSels);
    mIsUpdateProgramNeeded = true;
}

/**
 * @brief Draws the point splats in world space on the specified view.
 *
 * Renders all 3D points as screen-space splats using programmable vertex
 * pulling. Points are positioned through the standard camera projection,
 * and each point is expanded into a quad procedurally depending on the
 * configured Shape setting.
 *
 * @param[in] viewId: The bgfx view ID to submit the rendering commands to.
 */
void Points::draw(bgfx::ViewId viewId) const
{
    // Skip rendering if there are no vertices or the position buffer is invalid
    if (mVerPosCount == 0 || !mVertexPositions.isValid()) {
        return;
    }

    checkAndUpdateProgram();

    // Upload rendering settings to the shader via uniform.
    PointsUniforms::setWidth(mSettings.width);
    PointsUniforms::setGeneralColor(mSettings.generalColor);
    PointsUniforms::setDepthOffset(mSettings.depthOffset);
    PointsUniforms::setSelectionColor(mSettings.selectionColor);

    // Bind the position buffer as a compute buffer (SSBO) for vertex shader
    // access. The point positions are read by the vertex pulling mechanism.
    mVertexPositions.get().bindCompute(
        POINTS_POSITIONS_STAGE, bgfx::Access::Read);

    if (mVertexNormals.isValid()) {
        mVertexNormals.get().bindCompute(
            POINTS_NORMALS_STAGE, bgfx::Access::Read);
    }

    if (mVertexColors.isValid()) {
        mVertexColors.get().bindCompute(
            POINTS_COLORS_STAGE, bgfx::Access::Read);
    }

    if (mSettings.selectionVisibility) {
        mSelectionBuffer.get().bind(POINTS_SELECTION_STAGE, bgfx::Access::Read);
    }

    bgfx::setVertexCount(mVerPosCount * 6);

    bgfx::setState(
        0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
        BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_BLEND_ALPHA);

    // Bind the updated uniforms to the shader stage.
    PointsUniforms::bind();

    // cross section settings
    if (mSettings.crossSectionSettings.isEnabled()) {
        using enum CrossSectionSettings::CrossSectionType;
        CrossSectionUniforms::set(
            mSettings.crossSectionSettings.lower(),
            mSettings.crossSectionSettings.upper(),
            mSettings.crossSectionSettings.type() == PER_FRAGMENT);
    }
    else {
        CrossSectionUniforms::set();
    }
    CrossSectionUniforms::bind();

    bgfx::submit(viewId, mProgram);
}

/**
 * @brief Draws the point splats in ID mode on the specified view.
 *
 * @param[in] viewId: The bgfx view ID to submit the rendering commands to.
 * @param[in] id: The ID to render the points with.
 */
void Points::drawId(bgfx::ViewId viewId, uint32_t id) const
{
    // Skip rendering if there are no vertices or the position buffer is invalid
    if (mVerPosCount == 0 || !mVertexPositions.isValid()) {
        return;
    }

    checkAndUpdateProgram();

    // Upload rendering settings to the shader via uniform.
    PointsUniforms::setWidth(mSettings.width);
    PointsUniforms::setDepthOffset(mSettings.depthOffset);
    PointsUniforms::setId(id);

    // Bind the position buffer as a compute buffer (SSBO) for vertex shader
    // access. The point positions are read by the vertex pulling mechanism.
    mVertexPositions.get().bindCompute(
        POINTS_POSITIONS_STAGE, bgfx::Access::Read);

    bgfx::setVertexCount(mVerPosCount * 6);

    bgfx::setState(
        0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
        BGFX_STATE_DEPTH_TEST_LEQUAL |
        BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_ZERO));

    // Bind the updated uniforms to the shader stage.
    PointsUniforms::bind();

    // cross section settings
    if (mSettings.crossSectionSettings.isEnabled()) {
        using enum CrossSectionSettings::CrossSectionType;
        CrossSectionUniforms::set(
            mSettings.crossSectionSettings.lower(),
            mSettings.crossSectionSettings.upper(),
            mSettings.crossSectionSettings.type() == PER_FRAGMENT);
    }
    else {
        CrossSectionUniforms::set();
    }
    CrossSectionUniforms::bind();

    bgfx::submit(viewId, mIdProgram);
}

/**
 * @brief Checks if the shader program needs to be updated and updates it.
 *
 * Validates that required buffers (normals, colors) are available based on
 * the current shading and color settings, throwing an exception if invalid.
 * Then it selects the appropriate shader program.
 */
void Points::checkAndUpdateProgram() const
{
    if (!mIsUpdateProgramNeeded) {
        return;
    }

    if (mSettings.colorSetting == ColorSetting::PER_VERTEX) {
        if (!mVertexColors.isValid()) {
            throw std::runtime_error(
                "Points: PER_VERTEX color setting requires a valid vertex "
                "color buffer.");
        }
        if (mVerColCount != mVerPosCount) {
            throw std::runtime_error(
                "Points: The number of vertex colors must match the number of "
                "vertices.");
        }
    }
    if (mSettings.shading == Shading::PER_VERTEX) {
        if (!mVertexNormals.isValid()) {
            throw std::runtime_error(
                "Points: PER_VERTEX shading setting requires a valid vertex "
                "normal buffer.");
        }
        if (mVerNorCount != mVerPosCount) {
            throw std::runtime_error(
                "Points: The number of vertex normals must match the number of "
                "vertices.");
        }
    }

    if (mSettings.selectionVisibility) {
        if (!mSelectionBuffer.isValid()) {
            throw std::runtime_error(
                "Points: Selection visibility is ON but the selection buffer "
                "is invalid.");
        }
        if (mVerSelCount != mVerPosCount) {
            throw std::runtime_error(
                "Points: The number of selection elements must match the "
                "number of vertices.");
        }
    }

    mProgram               = pointsProgramSelector();
    mIdProgram             = pointsIdProgramSelector();
    mIsUpdateProgramNeeded = false;
}

/**
 * @brief Selects the correct bgfx shader program based on current settings.
 *
 * @return The appropriate bgfx::ProgramHandle for the current configuration.
 */
bgfx::ProgramHandle Points::pointsProgramSelector() const
{
    using enum VertFragProgram;

    constexpr uint N_SHADING_MODES   = 2;
    constexpr uint N_COLOR_MODES     = 2;
    constexpr uint N_SHAPE_MODES     = 2;
    constexpr uint N_SELECTION_MODES = 2;
    constexpr uint N_SECTION_MODES   = 2;

    uint shading = toUnderlying(mSettings.shading);
    uint color   = toUnderlying(mSettings.colorSetting);
    uint shape   = toUnderlying(mSettings.shape);
    uint select  = mSettings.selectionVisibility ? 1 : 0;
    uint section = mSettings.crossSectionSettings.isEnabled() ? 1 : 0;

    // the first shader of all the combinations
    uint base = toUnderlying(
        PRIMITIVE_POINTS_SHADING_NONE_COLOR_GENERAL_SHAPE_SQUARE_SELECTION_OFF_SECTION_OFF);

    uint offset = linearizeIndex<
        N_SHADING_MODES,
        N_COLOR_MODES,
        N_SHAPE_MODES,
        N_SELECTION_MODES,
        N_SECTION_MODES>(shading, color, shape, select, section);

    uint program = base + offset;

    ProgramManager& pm = Context::instance().programManager();
    return pm.getProgram(VertFragProgram(program));
}

/**
 * @brief Selects the correct bgfx shader program for ID picking based on
 * current settings.
 *
 * @return The appropriate bgfx::ProgramHandle for the current configuration.
 */
bgfx::ProgramHandle Points::pointsIdProgramSelector() const
{
    using enum VertFragProgram;

    constexpr uint N_SHAPE_MODES     = 2;
    constexpr uint N_SECTION_MODES   = 2;

    uint shape   = toUnderlying(mSettings.shape);
    uint section = mSettings.crossSectionSettings.isEnabled() ? 1 : 0;

    // the first shader of all the combinations
    uint base = toUnderlying(
        PRIMITIVE_POINTS_ID_SHADING_NONE_COLOR_GENERAL_SHAPE_SQUARE_SECTION_OFF);

    uint offset =
        linearizeIndex<N_SHAPE_MODES, N_SECTION_MODES>(shape, section);

    uint program = base + offset;

    ProgramManager& pm = Context::instance().programManager();
    return pm.getProgram(VertFragProgram(program));
}

} // namespace vcl
