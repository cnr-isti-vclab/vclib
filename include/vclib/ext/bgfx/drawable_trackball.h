/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_EXT_BGFX_DRAWABLE_TRACKBALL_H
#define VCL_EXT_BGFX_DRAWABLE_TRACKBALL_H

#include <vclib/ext/bgfx/context.h>
#include <vclib/render/interfaces/drawable_object_i.h>

#include <vclib/space/matrix.h>

#include "uniforms/drawable_trackball_uniforms.h"

namespace vcl::bgf {

class DrawableTrackBall : public DrawableObjectI
{
    bool mVisible = false;

    std::vector<vcl::Point3f> mVertices;
    std::vector<uint16_t>     mEdges;

    vcl::Matrix44f mTransform = vcl::Matrix44f::Identity();

    bgfx::VertexBufferHandle mVertexCoordBH = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle  mEdgeIndexBH   = BGFX_INVALID_HANDLE;

    bgfx::ProgramHandle mProgram =
        Context::programManager().getProgram(VclProgram::DRAWABLE_TRACKBALL);

    DrawableTrackballUniforms mUniforms;

public:
    // TODO: manage copy and swap
    DrawableTrackBall();

    ~DrawableTrackBall();

    void updateRotation(const vcl::Matrix44f& rot);

    void updateDragging(bool isDragging);

    // DrawableObject interface

    void draw(uint viewId) override;

    Point3d center() const override { return Point3d(); };

    double radius() const override { return 1.0; }

    DrawableObjectI* clone() const override
    {
        return new DrawableTrackBall(*this);
    }

    bool isVisible() const override { return mVisible; }

    void setVisibility(bool vis) override { mVisible = vis; }

private:
    void createBuffers();
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_DRAWABLE_TRACKBALL_H
