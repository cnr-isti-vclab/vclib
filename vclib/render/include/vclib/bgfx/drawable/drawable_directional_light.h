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

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_DIRECTIONAL_LIGHT_H
#define VCL_BGFX_DRAWABLE_DRAWABLE_DIRECTIONAL_LIGHT_H

#include "uniforms/drawable_directional_light_uniforms.h"

#include <vclib/render/interfaces/drawable_object_i.h>
#include <vclib/render/viewer/lights/directional_light.h>
#include <vclib/space/core/matrix.h>

#include <vclib/bgfx/context.h>

#include <bgfx/bgfx.h>

namespace vcl {

class DrawableDirectionalLight : public DrawableObjectI
{
    bool mVisible = false;

    Matrix44f mTransform = vcl::Matrix44f::Identity();

    std::vector<float> mVertices; // vertices of the drawn lines
    vcl::Color         mColor = vcl::Color::Yellow; // color of the lines

    bgfx::VertexBufferHandle mVertexCoordBH = BGFX_INVALID_HANDLE;

    DrawableDirectionalLightUniforms mUniform;

    // TODO: can we be sure that this is called after the context initialization
    // triggered by a window?
    bgfx::ProgramHandle mProgram =
        Context::instance().programManager().getProgram(
            VclProgram::DRAWABLE_DIRECTIONAL_LIGHT);

public:
    DrawableDirectionalLight();

    DrawableDirectionalLight(const DrawableDirectionalLight& other);

    DrawableDirectionalLight(DrawableDirectionalLight&& other);

    ~DrawableDirectionalLight();

    DrawableDirectionalLight& operator=(DrawableDirectionalLight other);

    void swap(DrawableDirectionalLight& other);

    void updateRotation(const vcl::Matrix44f& rot);

    const vcl::Color& linesColor() const { return mColor; }

    void setLinesColor(const vcl::Color& c);

    // DrawableObjectI interface

    void draw(uint viewId) const override;

    Box3d boundingBox() const override;

    std::shared_ptr<DrawableObjectI> clone() const override;

    bool isVisible() const override { return mVisible; }

    void setVisibility(bool vis) override { mVisible = vis; }

private:
    void createVertexBuffer();
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_DIRECTIONAL_LIGHT_H
