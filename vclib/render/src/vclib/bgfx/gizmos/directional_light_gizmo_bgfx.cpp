// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/gizmos/directional_light_gizmo_bgfx.h>

#include <vclib/bgfx/gizmos/uniforms/directional_light_gizmo_uniforms.h>

#include <vclib/algorithms/core.h>

namespace vcl {

DirectionalLightGizmoBGFX::DirectionalLightGizmoBGFX()
{
    // create a vertex buffer that will contain the endpoints of the lines that
    // describe the light
    // there are 4 x 4 lines having Z direction in the [-0.75, 0.75] viewport

    mVertices.reserve(16 * 2 * 3);

    const float low  = -1.f;
    const float high = 1.f;

    const float zlow  = -2.f;
    const float zhigh = 2.f;
    const uint  n     = 4;
    const float step  = (high - low) / (n - 1);

    for (uint i = 0; i < n; ++i) {
        for (uint j = 0; j < n; ++j) {
            mVertices.push_back(low + i * step);
            mVertices.push_back(low + j * step);
            mVertices.push_back(zlow);

            mVertices.push_back(low + i * step);
            mVertices.push_back(low + j * step);
            mVertices.push_back(zhigh);
        }
    }

    createVertexBuffer();
}

void DirectionalLightGizmoBGFX::swap(DirectionalLightGizmoBGFX& other)
{
    using std::swap;
    swap(mVisible, other.mVisible);
    mVertices.swap(other.mVertices);
    swap(mColor, other.mColor);
    swap(mTransform, other.mTransform);
    swap(mVertexPosBuffer, other.mVertexPosBuffer);
}

void DirectionalLightGizmoBGFX::updateRotation(const Matrix44f& rot)
{
    mTransform = rot;
}

void DirectionalLightGizmoBGFX::setLinesColor(const Color& c)
{
    mColor = c;
}

void DirectionalLightGizmoBGFX::draw(uint viewId)
{
    using enum VertFragProgram;

    ProgramManager& pm = Context::instance().programManager();

    if (isVisible()) {
        bgfx::setState(
            0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
            BGFX_STATE_DEPTH_TEST_LEQUAL | BGFX_STATE_PT_LINES);

        bgfx::setTransform(mTransform.data());

        DirectionalLightGizmoUniforms::setColor(mColor);
        DirectionalLightGizmoUniforms::bind();

        mVertexPosBuffer.bind(0);

        bgfx::submit(viewId, pm.getProgram<DRAWABLE_DIRECTIONAL_LIGHT>());
    }
}

void DirectionalLightGizmoBGFX::createVertexBuffer()
{
    mVertexPosBuffer.create(
        mVertices.data(),
        mVertices.size() / 3,
        bgfx::Attrib::Position,
        3,
        PrimitiveType::FLOAT);
}

} // namespace vcl
