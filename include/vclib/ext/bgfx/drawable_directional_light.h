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

#ifndef VCL_EXT_BGFX_DRAWABLE_DIRECTIONAL_LIGHT_H
#define VCL_EXT_BGFX_DRAWABLE_DIRECTIONAL_LIGHT_H

#include <bgfx/bgfx.h>

#include <vclib/render/interfaces/drawable_object_i.h>
#include <vclib/render/lights/directional_light.h>
#include <vclib/space/matrix.h>

#include <vclib/ext/bgfx/context.h>

#include "uniforms/drawable_directional_light_uniforms.h"

namespace vcl::bgf {

class DrawableDirectionalLight : public DrawableObjectI
{
    bool mVisible = false;

    float mTransform[32]; // two 4x4 matrices, first rotate, second translate

    std::vector<float> mVertices; // vertices of the drawn lines
    vcl::Color         mColor = vcl::Color::Yellow; // color of the lines

    bgfx::VertexBufferHandle mVertexCoordBH = BGFX_INVALID_HANDLE;

    DrawableDirectionalLightUniforms mUniform;

    bgfx::ProgramHandle mProgram = Context::programManager().getProgram(
        VclProgram::DRAWABLE_DIRECTIONAL_LIGHT);

public:
    DrawableDirectionalLight();

    DrawableDirectionalLight(const DrawableDirectionalLight& other);

    DrawableDirectionalLight(DrawableDirectionalLight&& other);

    ~DrawableDirectionalLight();

    DrawableDirectionalLight& operator=(DrawableDirectionalLight other);

    void swap(DrawableDirectionalLight& other);

    void update(const DirectionalLight<float>& l);

    const vcl::Color& linesColor() const { return mColor; }

    void setLinesColor(const vcl::Color& c);

    // DrawableObjectI interface

    void draw(uint viewId);

    Point3d center() const;

    double radius() const;

    DrawableObjectI* clone() const;

    bool isVisible() const { return mVisible; }

    void setVisibility(bool vis) { mVisible = vis; }

private:
    void createVertexBuffer();

    void updateTransform(uint i, const vcl::Matrix44f& matrix);
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_DRAWABLE_DIRECTIONAL_LIGHT_H
