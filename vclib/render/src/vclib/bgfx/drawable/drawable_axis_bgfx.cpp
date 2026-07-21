// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/drawable/drawable_axis_bgfx.h>

#include <vclib/bgfx/drawable/uniforms/drawable_axis_uniforms.h>

#include <vclib/algorithms/mesh/create.h>

namespace vcl {

/**
 * @brief Constructs a DrawableAxisBGFX with the specified size.
 *
 * @param[in] size: The scale factor for the axis indicator. Larger values
 * produce longer axes. Default is 1.
 */
DrawableAxisBGFX::DrawableAxisBGFX(double size)
{
    createAxis();

    updateMatrices(size);
}

/**
 * @brief Sets the size of the axis indicator.
 *
 * Updates the scale of all three axes uniformly. This modifies the
 * transformation matrices used during rendering.
 *
 * @param[in] size: The new scale factor for the axis indicator.
 */
void DrawableAxisBGFX::setSize(double size)
{
    updateMatrices(size);
}

/**
 * @brief Draws the axis indicator if it is currently visible.
 *
 * Renders three colored axes (X=red, Y=green, Z=blue), each composed of
 * a cylinder body and a cone tip. The axis is drawn at the origin using
 * the current transformation matrices.
 *
 * @param[in] settings: The drawing settings, including view ID and other
 * render parameters.
 *
 * @see DrawableObject::draw()
 */
void DrawableAxisBGFX::draw(const DrawObjectSettings& settings)
{
    using enum VertFragProgram;

    ProgramManager& pm = Context::instance().programManager();

    if (isVisible()) {
        uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                         BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL |
                         BGFX_STATE_MSAA;
        for (uint i = 0; i < 3; i++) {
            for (uint j = 0; j < 2; j++) {
                if (j == 0) // cylinders
                    DrawableAxisUniforms::setColor(AXIS_COLORS[i]);
                else // rest (cone, spheres...)
                    DrawableAxisUniforms::setColor(vcl::Color::White);
                DrawableAxisUniforms::bind();

                mArrowBuffers[j].bindVertexBuffers(MeshRenderSettings());
                mArrowBuffers[j].bindIndexBuffers(MeshRenderSettings());

                bgfx::setTransform(mMatrices[i].data());

                bgfx::submit(settings.viewId, pm.getProgram<DRAWABLE_AXIS>());
            }
        }
    }
}

/**
 * @brief Updates the transformation matrices for all three axes.
 *
 * Computes the scale and rotation matrices that position each axis
 * along its corresponding principal direction (X, Y, Z).
 *
 * @param[in] size: The scale factor to apply to all axes.
 */
void DrawableAxisBGFX::updateMatrices(double size)
{
    mMatrices[0](0, 1) = size;
    mMatrices[0](1, 0) = -size;
    mMatrices[0](2, 2) = size;
    mMatrices[0](3, 3) = 1;

    mMatrices[1](0, 0) = size;
    mMatrices[1](1, 1) = size;
    mMatrices[1](2, 2) = size;
    mMatrices[1](3, 3) = 1;

    mMatrices[2](0, 0) = size;
    mMatrices[2](1, 2) = -size;
    mMatrices[2](2, 1) = size;
    mMatrices[2](3, 3) = 1;
}

/**
 * @brief Initializes GPU render buffers for the axis meshes.
 *
 * Creates and binds vertex/index buffers for the cylinder and cone
 * components of the axis indicator using the static axis meshes.
 */
void DrawableAxisBGFX::createAxis()
{
    using MRI = MeshRenderInfo;
    using enum MRI::Buffers;

    MRI::BuffersBitSet btf = {VERTICES, VERT_NORMALS, TRIANGLES};

    mArrowBuffers[0] = MeshRenderBuffers<vcl::TriMesh>(AXIS_MESHES.first, btf);

    mArrowBuffers[1] = MeshRenderBuffers<vcl::TriMesh>(AXIS_MESHES.second, btf);
}

} // namespace vcl
