// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/drawable/drawable_directional_light_bgfx.h>

#include <vclib/bgfx/drawable/uniforms/drawable_directional_light_uniforms.h>

#include <vclib/algorithms/core.h>

namespace vcl {

DrawableDirectionalLightBGFX::DrawableDirectionalLightBGFX()
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

DrawableDirectionalLightBGFX::DrawableDirectionalLightBGFX(
    const DrawableDirectionalLightBGFX& other) :
        DrawableObject(other), mVisible(other.mVisible),
        mTransform(other.mTransform), mVertices(other.mVertices),
        mColor(other.mColor)
{
    createVertexBuffer();
}

DrawableDirectionalLightBGFX::DrawableDirectionalLightBGFX(
    DrawableDirectionalLightBGFX&& other)
{
    swap(other);
}

DrawableDirectionalLightBGFX::~DrawableDirectionalLightBGFX()
{
}

DrawableDirectionalLightBGFX& DrawableDirectionalLightBGFX::operator=(
    DrawableDirectionalLightBGFX other)
{
    swap(other);
    return *this;
}

void DrawableDirectionalLightBGFX::swap(DrawableDirectionalLightBGFX& other)
{
    using std::swap;
    swap(mVisible, other.mVisible);
    mVertices.swap(other.mVertices);
    swap(mColor, other.mColor);
    swap(mTransform, other.mTransform);
    swap(mVertexPosBuffer, other.mVertexPosBuffer);
}

void DrawableDirectionalLightBGFX::updateRotation(const Matrix44f& rot)
{
    mTransform = rot;
}

void DrawableDirectionalLightBGFX::setLinesColor(const Color& c)
{
    mColor = c;
}

void DrawableDirectionalLightBGFX::draw(const DrawObjectSettings& settings)
{
    using enum VertFragProgram;

    ProgramManager& pm = Context::instance().programManager();

    if (isVisible()) {
        bgfx::setState(
            0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
            BGFX_STATE_DEPTH_TEST_LEQUAL | BGFX_STATE_PT_LINES);

        bgfx::setTransform(mTransform.data());

        DrawableDirectionalLightUniforms::setColor(mColor);
        DrawableDirectionalLightUniforms::bind();

        mVertexPosBuffer.bind(0);

        bgfx::submit(
            settings.viewId, pm.getProgram<DRAWABLE_DIRECTIONAL_LIGHT>());
    }
}

Box3d DrawableDirectionalLightBGFX::boundingBox() const
{
    return Box3d();
}

std::shared_ptr<DrawableObject> DrawableDirectionalLightBGFX::clone() const&
{
    return std::make_shared<DrawableDirectionalLightBGFX>(*this);
}

std::shared_ptr<DrawableObject> DrawableDirectionalLightBGFX::clone() &&
{
    return std::make_shared<DrawableDirectionalLightBGFX>(std::move(*this));
}

void DrawableDirectionalLightBGFX::createVertexBuffer()
{
    mVertexPosBuffer.create(
        mVertices.data(),
        mVertices.size() / 3,
        bgfx::Attrib::Position,
        3,
        PrimitiveType::FLOAT);
}

} // namespace vcl
