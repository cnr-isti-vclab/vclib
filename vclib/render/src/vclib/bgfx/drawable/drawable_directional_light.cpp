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

#include <vclib/bgfx/drawable/drawable_directional_light.h>

#include <vclib/math/transform.h>
#include <vclib/render/viewer/matrix.h>

namespace vcl {

DrawableDirectionalLight::DrawableDirectionalLight()
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
    setLinesColor(mColor);
}

DrawableDirectionalLight::DrawableDirectionalLight(
    const DrawableDirectionalLight& other) :
        DrawableObject(other), mVisible(other.mVisible),
        mTransform(other.mTransform), mVertices(other.mVertices),
        mColor(other.mColor), mUniform(other.mUniform), mProgram(other.mProgram)
{
    createVertexBuffer();
}

DrawableDirectionalLight::DrawableDirectionalLight(
    DrawableDirectionalLight&& other)
{
    swap(other);
}

DrawableDirectionalLight::~DrawableDirectionalLight()
{
    if (bgfx::isValid(mVertexCoordBH)) {
        bgfx::destroy(mVertexCoordBH);
    }
}

DrawableDirectionalLight& DrawableDirectionalLight::operator=(
    DrawableDirectionalLight other)
{
    swap(other);
    return *this;
}

void DrawableDirectionalLight::swap(DrawableDirectionalLight& other)
{
    std::swap(mVisible, other.mVisible);
    mVertices.swap(other.mVertices);
    std::swap(mColor, other.mColor);
    std::swap(mUniform, other.mUniform);
    std::swap(mProgram, other.mProgram);
    std::swap(mTransform, other.mTransform);
    std::swap(mVertexCoordBH, other.mVertexCoordBH);
}

void DrawableDirectionalLight::updateRotation(const Matrix44f& rot)
{
    mTransform = rot;
}

void DrawableDirectionalLight::setLinesColor(const Color& c)
{
    mColor = c;
    mUniform.setColor(mColor);
}

void DrawableDirectionalLight::draw(uint viewId) const
{
    if (isVisible()) {
        if (bgfx::isValid(mProgram)) {
            bgfx::setState(
                0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL |
                BGFX_STATE_PT_LINES);

            bgfx::setTransform(mTransform.data());

            mUniform.bind();

            bgfx::setVertexBuffer(0, mVertexCoordBH);

            bgfx::submit(viewId, mProgram);
        }
    }
}

Box3d DrawableDirectionalLight::boundingBox() const
{
    return Box3d();
}

std::shared_ptr<DrawableObject> DrawableDirectionalLight::clone() const
{
    return std::make_shared<DrawableDirectionalLight>(*this);
}

void DrawableDirectionalLight::createVertexBuffer()
{
    // vertex buffer (positions)
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .end();

    mVertexCoordBH = bgfx::createVertexBuffer(
        bgfx::makeRef(mVertices.data(), mVertices.size() * sizeof(float)),
        layout);
}

} // namespace vcl
