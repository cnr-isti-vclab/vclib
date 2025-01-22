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

#include <vclib/render/drawable/drawable_object.h>
#include <vclib/render/drawable/trackball/trackball_render_data.h>
#include <vclib/space/core/matrix.h>

#include <vclib/bgfx/context.h>

namespace vcl {

class DrawableTrackBall : public DrawableObject, protected TrackballRenderData
{
    bool mVisible = true;

    bgfx::VertexBufferHandle mVertexCoordBH = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle  mEdgeIndexBH   = BGFX_INVALID_HANDLE;

    // TODO: can we be sure that this is called after the context initialization
    // triggered by a window?
    bgfx::ProgramHandle mProgram =
        Context::instance().programManager().getProgram(
            VclProgram::DRAWABLE_TRACKBALL);

    DrawableTrackballUniforms mUniforms;

public:
    using TrackballRenderData::setTransform;

    // TODO: manage copy and swap
    DrawableTrackBall();

    ~DrawableTrackBall();

    void updateDragging(bool isDragging);

    // DrawableObject interface

    void draw(uint viewId) const override;

    Box3d boundingBox() const override { return Box3d(); }

    std::shared_ptr<DrawableObject> clone() const override
    {
        return std::make_shared<DrawableTrackBall>(*this);
    }

    bool isVisible() const override { return mVisible; }

    void setVisibility(bool vis) override { mVisible = vis; }

private:
    void createBuffers();
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_TRACKBALL_H
