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
#include <vclib/bgfx/buffers.h>
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

    VertexBuffer mVertexCoordsBuffer;
    IndexBuffer mEdgeIndexBuffer;

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

    DrawableTrackBall(const DrawableTrackBall& other) :
        mVisible(other.mVisible),
        mProgram(other.mProgram),
        mUniforms(other.mUniforms),
        mTransform(other.mTransform)
    {
        // copy all the members that can be copied, and then re-create the
        // buffers
        createBuffers();
    }

    // default move constructor - buffers can be moved
    DrawableTrackBall(DrawableTrackBall&& other) = default;

    // default destructor - buffers are destroyed by their destructor
    ~DrawableTrackBall() = default;

    /**
     * @brief Swap the content of this object with another DrawableTrackBall
     * object.
     *
     * @param[in] other: the other DrawableTrackBall object.
     */
    void swap(DrawableTrackBall& other)
    {
        using std::swap;
        swap(mVisible, other.mVisible);
        swap(mVertexCoordsBuffer, other.mVertexCoordsBuffer);
        swap(mEdgeIndexBuffer, other.mEdgeIndexBuffer);
        swap(mProgram, other.mProgram);
        swap(mUniforms, other.mUniforms);
        swap(mTransform, other.mTransform);
    }

    friend void swap(DrawableTrackBall& a, DrawableTrackBall& b) { a.swap(b); }

    /**
     * @brief Update the dragging status of the trackball.
     * @param[in] isDragging: true if the trackball is being dragged, false
     * otherwise.
     */
    void updateDragging(bool isDragging) { mUniforms.setDragging(isDragging); }

    void setTransform(const vcl::Matrix44f& mtx) { mTransform = mtx; }

    // copy and swap idiom
    DrawableTrackBall& operator=(DrawableTrackBall other)
    {
        swap(other);
        return *this;
    }

    // DrawableObject interface

    void draw(uint viewId) const override
    {
        if (isVisible()) {
            if (bgfx::isValid(mProgram)) {
                bgfx::setState(
                    0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_Z |
                    BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_PT_LINES |
                    BGFX_STATE_BLEND_NORMAL);

                mVertexCoordsBuffer.bind(0);
                mEdgeIndexBuffer.bind();

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
        mVertexCoordsBuffer.set(
            TRACKBALL_DATA.first.data(),
            TRACKBALL_DATA.first.size() * 3,
            bgfx::Attrib::Position,
            3,
            PrimitiveType::FLOAT);

        // edge index buffer
        mEdgeIndexBuffer.set(
            TRACKBALL_DATA.second.data(), TRACKBALL_DATA.second.size(), false);
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_TRACKBALL_H
