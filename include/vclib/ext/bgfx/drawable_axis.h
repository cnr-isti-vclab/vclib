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

#include <vclib/algorithms/create.h>
#include <vclib/meshes/tri_mesh.h>

#include <vclib/render/drawable_object.h>
#include <vclib/render/mesh_render_buffers.h>

#include "shader_programs/drawable_axis_shader_program.h"

namespace vcl::bgf {

class DrawableAxis : public DrawableObject
{
    bool visible = false;

    // TODO: add a 'append' member function to vcl::Mesh
    // and use a single mesh render buffer
    MeshRenderBuffers<vcl::TriMesh> mrbCylinder;
    MeshRenderBuffers<vcl::TriMesh> mrbCone;

    bgfx::ProgramHandle program = BGFX_INVALID_HANDLE;

public:
    DrawableAxis()
    {
        vcl::TriMesh cylinder = vcl::createCylinder<vcl::TriMesh>(0.02, 2);
        vcl::updatePerVertexNormals(cylinder);

        vcl::TriMesh cone = vcl::createCone<vcl::TriMesh>(0.05, 0, 0.2);
        vcl::updatePerVertexNormals(cone);
        vcl::translate(cone, vcl::Point3d(0, 1, 0));

        mrbCylinder = MeshRenderBuffers<vcl::TriMesh>(cylinder);
        mrbCone     = MeshRenderBuffers<vcl::TriMesh>(cone);
    }

    ~DrawableAxis() = default;

    void setShaderProgram(const DrawableAxisShaderProgram& sp)
    {
        program = sp.getProgram();
    }

    // DrawableObject interface

    void draw()
    {
        if (isVisible()) {
            if (bgfx::isValid(program)) {
                // todo

                uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                                 BGFX_STATE_WRITE_Z |
                                 BGFX_STATE_DEPTH_TEST_LEQUAL | BGFX_STATE_MSAA;

                bgfx::submit(0, program);
            }
        }
    }

    Point3d center() const { return Point3d(); };

    double radius() const { return 1.0; }

    DrawableObject* clone() const { return new DrawableAxis(*this); }

    bool isVisible() const { return visible; }

    void setVisibility(bool vis) { visible = vis; }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_DRAWABLE_AXIS_H
