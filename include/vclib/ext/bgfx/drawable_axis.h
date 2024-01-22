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
#include "uniforms/drawable_axis_uniforms.h"

namespace vcl::bgf {

class DrawableAxis : public DrawableObject
{
    bool visible = false;

    vcl::Color colors[3] = {
        vcl::Color::Red,
        vcl::Color::Green,
        vcl::Color::Blue};

    vcl::Matrix44f matrices[3] = {
        vcl::Matrix44f::Identity(),
        vcl::Matrix44f::Identity(),
        vcl::Matrix44f::Identity()};

    MeshRenderBuffers<vcl::TriMesh> mrbArrow;

    bgfx::ProgramHandle program = BGFX_INVALID_HANDLE;

    bgfx::VertexBufferHandle vertexCoordBH  = BGFX_INVALID_HANDLE;
    bgfx::VertexBufferHandle vertexNormalBH = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle triangleIndexBH  = BGFX_INVALID_HANDLE;

    DrawableAxisUniforms uniforms;

public:
    DrawableAxis()
    {
        vcl::TriMesh arrow = vcl::createCylinder<vcl::TriMesh>(0.02, 2);
        vcl::updatePerVertexNormals(arrow);

        vcl::TriMesh cone = vcl::createCone<vcl::TriMesh>(0.05, 0, 0.2);
        vcl::updatePerVertexNormals(cone);
        vcl::translate(cone, vcl::Point3d(0, 1, 0));
        arrow.append(cone);

        mrbArrow = MeshRenderBuffers<vcl::TriMesh>(arrow);

        createBuffers();
        updateMatrices();
    }

    ~DrawableAxis()
    {
        destroyBuffers();
    };

    void setShaderProgram(const DrawableAxisShaderProgram& sp)
    {
        program = sp.getProgram();
    }

    // DrawableObject interface

    void draw()
    {
        if (isVisible()) {
            if (bgfx::isValid(program)) {
                for (uint i = 0; i < 3; i++) {
                    bindBuffers();
                    uniforms.setColor(colors[i]);
                    uniforms.bind();

                    bgfx::setTransform(matrices[i].data());

                    uint64_t state = 0 | BGFX_STATE_WRITE_RGB |
                                     BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
                                     BGFX_STATE_DEPTH_TEST_LEQUAL |
                                     BGFX_STATE_MSAA;

                    bgfx::submit(0, program);
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
    void createBuffers()
    {
        // vertex buffer (positions)
        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .end();

        vertexCoordBH = bgfx::createVertexBuffer(
            bgfx::makeRef(
                mrbArrow.vertexBufferData(),
                mrbArrow.vertexBufferSize() * sizeof(float)),
            layout);

        bgfx::VertexLayout vnlayout;
        vnlayout.begin()
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .end();

        vertexNormalBH = bgfx::createVertexBuffer(
            bgfx::makeRef(
                mrbArrow.vertexNormalBufferData(),
                mrbArrow.vertexBufferSize() * sizeof(float)),
            vnlayout);

        triangleIndexBH = bgfx::createIndexBuffer(
            bgfx::makeRef(
                mrbArrow.triangleBufferData(),
                mrbArrow.triangleBufferSize() * sizeof(uint32_t)),
            BGFX_BUFFER_INDEX32);
    }

    void updateMatrices()
    {
        matrices[0](0,0) = 0;
        matrices[0](0,1) = 1;
        matrices[0](1,0) = -1;
        matrices[0](1,1) = 0;

        matrices[2](1,1) = 0;
        matrices[2](1,2) = -1;
        matrices[2](2,1) = 1;
        matrices[2](2,2) = 0;
    }

    void bindBuffers()
    {
        bgfx::setVertexBuffer(0, vertexCoordBH);
        bgfx::setVertexBuffer(1, vertexNormalBH);
        bgfx::setIndexBuffer(triangleIndexBH);
    }

    void destroyBuffers()
    {
        if (bgfx::isValid(vertexCoordBH))
            bgfx::destroy(vertexCoordBH);

        if (bgfx::isValid(vertexNormalBH))
            bgfx::destroy(vertexNormalBH);

        if (bgfx::isValid(triangleIndexBH))
            bgfx::destroy(triangleIndexBH);
    }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_DRAWABLE_AXIS_H
