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

#ifndef VCL_EXT_BGFX_DRAWABLE_AXIS_H
#define VCL_EXT_BGFX_DRAWABLE_AXIS_H

#include <bgfx/bgfx.h>

#include <vclib/meshes/tri_mesh.h>
#include <vclib/render/interfaces/drawable_object_i.h>
#include <vclib/space/matrix.h>

#include <vclib/ext/bgfx/context.h>

#include "mesh_render_buffers.h"
#include "uniforms/drawable_axis_uniforms.h"

namespace vcl::bgf {

class DrawableAxis : public DrawableObjectI
{
    bool visible = false;

    const vcl::Color colors[3] = {
        vcl::Color::Red,
        vcl::Color::Green,
        vcl::Color::Blue};

    vcl::Matrix44f matrices[3] = {
        vcl::Matrix44f::Zero(),
        vcl::Matrix44f::Zero(),
        vcl::Matrix44f::Zero()};

    MeshRenderBuffers<vcl::TriMesh> mrbArrow[2];

    bgfx::ProgramHandle program =
        Context::programManager().getProgram(VclProgram::DRAWABLE_AXIS);
    ;

    bgfx::VertexBufferHandle vertexCoordBH   = BGFX_INVALID_HANDLE;
    bgfx::VertexBufferHandle vertexNormalBH  = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle  triangleIndexBH = BGFX_INVALID_HANDLE;

    DrawableAxisUniforms uniforms;

public:
    DrawableAxis(double size = 1, bool fromOrigin = false);

    ~DrawableAxis() = default;

    void setSize(double size);

    // DrawableObject interface

    void setShaderProgram(const ShaderProgramI& sp) override;

    void draw(uint viewId) override;

    Point3d center() const override { return Point3d(); };

    double radius() const override { return 1.0; }

    DrawableObjectI* clone() const override { return new DrawableAxis(*this); }

    bool isVisible() const override { return visible; }

    void setVisibility(bool vis) override { visible = vis; }

private:
    void updateMatrices(double size);

    void createAxis(bool fromOrigin);
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_DRAWABLE_AXIS_H
