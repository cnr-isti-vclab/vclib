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
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_EXT_BGFX_DRAWABLE_AXIS_H
#define VCL_EXT_BGFX_DRAWABLE_AXIS_H

#include <bgfx/bgfx.h>

#include <vclib/meshes/tri_mesh.h>
#include <vclib/render/interfaces/drawable_object_i.h>
#include <vclib/space/matrix.h>

#include "mesh_render_buffers.h"
#include "shader_programs/drawable_axis_shader_program.h"
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

    bgfx::ProgramHandle program = BGFX_INVALID_HANDLE;

    bgfx::VertexBufferHandle vertexCoordBH   = BGFX_INVALID_HANDLE;
    bgfx::VertexBufferHandle vertexNormalBH  = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle  triangleIndexBH = BGFX_INVALID_HANDLE;

    DrawableAxisUniforms uniforms;

public:
    DrawableAxis(double size = 1, bool fromOrigin = false);

    ~DrawableAxis() = default;

    void setSize(double size);

    void setShaderProgram(const DrawableAxisShaderProgram& sp);

    // DrawableObject interface

    void draw(uint viewID) override;

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
