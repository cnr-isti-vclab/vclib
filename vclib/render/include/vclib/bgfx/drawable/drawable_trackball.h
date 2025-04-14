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
    bool mVisible = true;

    VertexBuffer mVertexCoordsColorBuffer;
    IndexBuffer  mEdgeIndexBuffer;

    // TODO: can we be sure that this is called after the context initialization
    // triggered by a window?
    bgfx::ProgramHandle mProgram =
        Context::instance()
            .programManager()
            .getProgram<VertFragProgram::DRAWABLE_TRACKBALL>();

    DrawableTrackballUniforms mUniforms;

    vcl::Matrix44f mTransform = vcl::Matrix44f::Identity();

public:
    DrawableTrackBall();

    DrawableTrackBall(const DrawableTrackBall& other);

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
    void swap(DrawableTrackBall& other);

    friend void swap(DrawableTrackBall& a, DrawableTrackBall& b) { a.swap(b); }

    /**
     * @brief Update the dragging status of the trackball.
     * @param[in] isDragging: true if the trackball is being dragged, false
     * otherwise.
     */
    void updateDragging(bool isDragging);

    void setTransform(const vcl::Matrix44f& mtx);

    // copy and swap idiom
    DrawableTrackBall& operator=(DrawableTrackBall other);

    // DrawableObject interface

    void draw(uint viewId) const override;

    Box3d boundingBox() const override;

    std::shared_ptr<DrawableObject> clone() const& override;

    std::shared_ptr<DrawableObject> clone() && override;

    bool isVisible() const override;

    void setVisibility(bool vis) override;

private:
    void createBuffers();
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_TRACKBALL_H
