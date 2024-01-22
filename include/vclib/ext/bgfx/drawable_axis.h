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

#include "mesh_render_buffers.h"
#include "shader_programs/drawable_axis_shader_program.h"
#include "uniforms/drawable_axis_uniforms.h"

namespace vcl::bgf {

class DrawableAxis : public DrawableObject
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

    bgfx::VertexBufferHandle vertexCoordBH  = BGFX_INVALID_HANDLE;
    bgfx::VertexBufferHandle vertexNormalBH = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle triangleIndexBH  = BGFX_INVALID_HANDLE;

    DrawableAxisUniforms uniforms;

public:
    DrawableAxis(double size = 1, bool fromOrigin = false)
    {
        createAxis(fromOrigin);

        updateMatrices(size);
    }

    ~DrawableAxis() = default;

    void setSize(double size)
    {
        updateMatrices(size);
    }

    void setShaderProgram(const DrawableAxisShaderProgram& sp)
    {
        program = sp.getProgram();
    }

    // DrawableObject interface

    void draw()
    {
        if (isVisible()) {
            if (bgfx::isValid(program)) {
                uint64_t state = 0 | BGFX_STATE_WRITE_RGB |
                                 BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
                                 BGFX_STATE_DEPTH_TEST_LEQUAL |
                                 BGFX_STATE_MSAA;
                for (uint i = 0; i < 3; i++) {
                    for (uint j = 0; j < 2; j++) {
                        if (j == 0) // cylinders
                            uniforms.setColor(colors[i]);
                        else // rest (cone, spheres...)
                            uniforms.setColor(vcl::Color::White);
                        uniforms.bind();

                        mrbArrow[j].bindVertexBuffers();
                        mrbArrow[j].bindIndexBuffers();

                        bgfx::setTransform(matrices[i].data());

                        bgfx::submit(0, program);
                    }
                }
            }
        }
    }

    Point3d center() const { return Point3d(); };

    double radius() const { return 1.0; }

    DrawableObject* clone() const { return new DrawableAxis(*this); }

    bool isVisible() const { return visible; }

    void setVisibility(bool vis) { visible = vis; }

private:
    void updateMatrices(double size)
    {
        matrices[0](0, 1) = size;
        matrices[0](1, 0) = -size;
        matrices[0](2, 2) = size;
        matrices[0](3, 3) = 1;

        matrices[1](0, 0) = size;
        matrices[1](1, 1) = size;
        matrices[1](2, 2) = size;
        matrices[1](3, 3) = 1;

        matrices[2](0, 0) = size;
        matrices[2](1, 2) = -size;
        matrices[2](2, 1) = size;
        matrices[2](3, 3) = 1;
    }

    void createAxis(bool fromOrigin)
    {
        const double unitLength = 1.0;

        const double cylLength = fromOrigin ? unitLength : unitLength * 2;
        const double cylRadius = cylLength * 0.0025;

        const double coneRadius = cylRadius * 10;
        const double coneLength = cylLength * 0.1;

        const double firstSphereRadius = unitLength * 0.02;
        const double commonSphereRadius = unitLength * 0.008;

        vcl::TriMesh cylinder =
            vcl::createCylinder<vcl::TriMesh>(cylRadius, cylLength);
        if (fromOrigin) {
            vcl::translate(cylinder, vcl::Point3d(0, unitLength * 0.5, 0));
        }

        vcl::updatePerVertexNormals(cylinder);

        vcl::TriMesh rest =
            vcl::createCone<vcl::TriMesh>(coneRadius, 0, coneLength);
        double transl = unitLength + (coneLength * 0.5);
        vcl::translate(rest, vcl::Point3d(0, transl, 0));

        if (!fromOrigin) {
            vcl::Sphered s(vcl::Point3d(0, -1, 0), firstSphereRadius);
            vcl::TriMesh sp = vcl::createSphere<vcl::TriMesh>(s);
            rest.append(sp);
        }

        for (uint i = 0; i < 9; ++i) {
            const double step = unitLength * 0.1;
            const double x = step + i * step;
            vcl::Sphered s(vcl::Point3d(0, x, 0), commonSphereRadius);
            vcl::TriMesh sp = vcl::createSphere<vcl::TriMesh>(s);
            rest.append(sp);
            if (!fromOrigin) {
                s.center().y() = -x;
                sp = vcl::createSphere<vcl::TriMesh>(s);
                rest.append(sp);
            }
        }

        const double rad = fromOrigin ? firstSphereRadius : commonSphereRadius;
        vcl::Sphered s = vcl::Sphered(vcl::Point3d(0, 0, 0), rad);
        vcl::TriMesh sp = vcl::createSphere<vcl::TriMesh>(s);
        rest.append(sp);

        vcl::updatePerVertexNormals(rest);

        mrbArrow[0] = MeshRenderBuffers<vcl::TriMesh>(
            cylinder,
            MeshRenderBuffers<vcl::TriMesh>::VERT_NORMALS |
                MeshRenderBuffers<vcl::TriMesh>::TRIANGLES);

        mrbArrow[1] = MeshRenderBuffers<vcl::TriMesh>(
            rest,
            MeshRenderBuffers<vcl::TriMesh>::VERT_NORMALS |
                MeshRenderBuffers<vcl::TriMesh>::TRIANGLES);
    }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_DRAWABLE_AXIS_H
