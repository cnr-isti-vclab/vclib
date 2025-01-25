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

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_TRACKBALL_H
#define VCL_BGFX_DRAWABLE_DRAWABLE_TRACKBALL_H

#include "uniforms/drawable_trackball_uniforms.h"

#include <vclib/algorithms/core/create.h>
#include <vclib/math/transform.h>
#include <vclib/render/drawable/drawable_object.h>
#include <vclib/space/core/matrix.h>

#include <vclib/bgfx/context.h>

namespace vcl {

class DrawableTrackBall : public DrawableObject
{
    inline static const uint N_POINTS = 128;
    inline static const auto TRACKBALL_DATA =
        createTrackBall<float, uint16_t>(1.0, N_POINTS);

    bool mVisible = true;

    bgfx::VertexBufferHandle mVertexCoordBH = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle  mEdgeIndexBH   = BGFX_INVALID_HANDLE;

    // TODO: can we be sure that this is called after the context initialization
    // triggered by a window?
    bgfx::ProgramHandle mProgram =
        Context::instance().programManager().getProgram(
            VclProgram::DRAWABLE_TRACKBALL);

    DrawableTrackballUniforms mUniforms;

    vcl::Matrix44f mTransform = vcl::Matrix44f::Identity();

public:
    DrawableTrackBall()
    {
        mUniforms.setNumberOfVerticesPerAxis(N_POINTS);

        createBuffers();
    }

    ~DrawableTrackBall() = default;

    void updateDragging(bool isDragging) { mUniforms.setDragging(isDragging); }

    void setTransform(const vcl::Matrix44f& mtx) { mTransform = mtx; }

    // DrawableObject interface

    void draw(uint viewId) const override
    {
        if (isVisible()) {
            if (bgfx::isValid(mProgram)) {
                bgfx::setState(
                    0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_Z |
                    BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_PT_LINES |
                    BGFX_STATE_BLEND_NORMAL);

                bgfx::setVertexBuffer(0, mVertexCoordBH);
                bgfx::setIndexBuffer(mEdgeIndexBH);

                bgfx::setTransform(mTransform.data());

                mUniforms.bind();

                bgfx::submit(viewId, mProgram);
            }
        }
    }

    Box3d boundingBox() const override { return Box3d(); }

    std::shared_ptr<DrawableObject> clone() const override
    {
        return std::make_shared<DrawableTrackBall>(*this);
    }

    bool isVisible() const override { return mVisible; }

    void setVisibility(bool vis) override { mVisible = vis; }

private:
    void createBuffers()
    {
        // vertex buffer
        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .end();

        mVertexCoordBH = bgfx::createVertexBuffer(
            bgfx::makeRef(
                TRACKBALL_DATA.first.data(),
                TRACKBALL_DATA.first.size() * 3 * sizeof(float)),
            layout);

        mEdgeIndexBH = bgfx::createIndexBuffer(bgfx::makeRef(
            TRACKBALL_DATA.second.data(),
            TRACKBALL_DATA.second.size() * sizeof(uint16_t)));
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_TRACKBALL_H
