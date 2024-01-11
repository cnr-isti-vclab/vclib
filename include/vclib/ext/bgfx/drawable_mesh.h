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

#ifndef VCL_EXT_BGFX_DRAWABLE_MESH_DRAWABLE_MESH_H
#define VCL_EXT_BGFX_DRAWABLE_MESH_DRAWABLE_MESH_H

#include <bgfx/bgfx.h>

#include <vclib/render/generic_drawable_mesh.h>
#include <vclib/render/mesh_render_buffers.h>

#include "drawable_mesh_shader_program.h"
#include "uniforms/mesh_render_settings_uniforms.h"

namespace vcl::bgf {

template<MeshConcept MeshType>
class DrawableMesh : public GenericDrawableMesh
{
    using Base = GenericDrawableMesh;

    MeshRenderBuffers<MeshType> mrb;

    bgfx::ProgramHandle program = BGFX_INVALID_HANDLE;

    bgfx::VertexBufferHandle vertexCoordBH  = BGFX_INVALID_HANDLE;
    bgfx::VertexBufferHandle vertexNormalBH = BGFX_INVALID_HANDLE;
    bgfx::VertexBufferHandle vertexColorBH = BGFX_INVALID_HANDLE;

    bgfx::IndexBufferHandle triangleIndexBH = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle triangleColorBH = BGFX_INVALID_HANDLE;

    bgfx::UniformHandle meshColorUH = BGFX_INVALID_HANDLE;
    bgfx::UniformHandle userColorUH = BGFX_INVALID_HANDLE;

    MeshRenderSettingsUniforms meshRenderSettingsUniforms;

public:
    DrawableMesh() { createBGFXUniforms(); };

    DrawableMesh(const DrawableMesh& oth) :
            GenericDrawableMesh(oth), mrb(oth.mrb),
            meshRenderSettingsUniforms(oth.meshRenderSettingsUniforms)
    {
        // each drawable object has its own bgfx buffers
        createBGFXBuffers();
        createBGFXUniforms();
    }

    DrawableMesh(DrawableMesh&& oth) { swap(oth); }

    DrawableMesh(const MeshType& mesh)
    {
        if constexpr (HasName<MeshType>) {
            name() = mesh.name();
        }

        updateBuffers(mesh);
        createBGFXUniforms();

        mrs.setDefaultSettingsFromCapability();
        meshRenderSettingsUniforms.updateSettings(mrs);
    }

    DrawableMesh& operator=(DrawableMesh oth)
    {
        swap(oth);
        return *this;
    }

    ~DrawableMesh()
    {
        destroyBGFXBuffers();
        destroyBGFXUniforms();
    }

    void swap(DrawableMesh& oth)
    {
        GenericDrawableMesh::swap(oth);
        std::swap(program, oth.program);
        std::swap(mrb, oth.mrb);
        std::swap(vertexCoordBH, oth.vertexCoordBH);
        std::swap(vertexNormalBH, oth.vertexNormalBH);
        std::swap(vertexColorBH, oth.vertexColorBH);
        std::swap(triangleIndexBH, oth.triangleIndexBH);
        std::swap(triangleColorBH, oth.triangleColorBH);
        std::swap(meshColorUH, oth.meshColorUH);
        std::swap(userColorUH, oth.userColorUH);
        std::swap(meshRenderSettingsUniforms, oth.meshRenderSettingsUniforms);
    }

    void updateBuffers(const MeshType& m)
    {
        if constexpr (HasName<MeshType>) {
            name() = m.name();
        }

        mrb = MeshRenderBuffers<MeshType>(m);
        mrs.setRenderCapabilityFrom(m);
        meshRenderSettingsUniforms.updateSettings(mrs);

        destroyBGFXBuffers();
        createBGFXBuffers();
    }

    // GenericDrawableMesh implementation

    virtual void setShaderProgram(const GenericDrawableMeshShaderProgram& p)
    {
        const DrawableMeshShaderProgram* ptr =
            dynamic_cast<const DrawableMeshShaderProgram*>(&p);
        if (ptr) {
            program = ptr->program();
        }
    };

    // DrawableObject implementation

    void init() {}

    void draw() const
    {
        if (bgfx::isValid(program)) {
            bgfx::setVertexBuffer(0, vertexCoordBH);

            if (bgfx::isValid(vertexNormalBH)) { // vertex normals
                bgfx::setVertexBuffer(1, vertexNormalBH);
            }

            if (bgfx::isValid(vertexColorBH)) { // vertex colors
                bgfx::setVertexBuffer(2, vertexColorBH);
            }

            bgfx::setIndexBuffer(triangleIndexBH);

            if (bgfx::isValid(triangleColorBH)) { // triangle colors
                bgfx::setBuffer(3, triangleColorBH, bgfx::Access::Read);
            }

            uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                             BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |
                             BGFX_STATE_CULL_CW | BGFX_STATE_MSAA;

            bgfx::setState(state);

            bgfx::setUniform(meshColorUH, mrb.meshColorBufferData());
            bgfx::setUniform(userColorUH, mrs.surfaceUserColorData());

            meshRenderSettingsUniforms.setUniforms();

            bgfx::submit(0, program);
        }
    }

    vcl::Point3d center() const { return (mrb.bbMin() + mrb.bbMax()) / 2; }

    double radius() const { return (mrb.bbMax() - mrb.bbMin()).norm() / 2; }

    DrawableMesh* clone() const { return new DrawableMesh(*this); }

    void setRenderSettings(const MeshRenderSettings& rs)
    {
        GenericDrawableMesh::setRenderSettings(rs);
        meshRenderSettingsUniforms.updateSettings(rs);
    }

private:
    void createBGFXBuffers()
    {
        // vertex buffer (positions)
        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .end();

        vertexCoordBH = bgfx::createVertexBuffer(
            bgfx::makeRef(
                mrb.vertexBufferData(), mrb.vertexBufferSize() * sizeof(float)),
            layout);

        // vertex buffer (normals)
        if (mrb.vertexNormalBufferData()) {
            bgfx::VertexLayout vnlayout;
            vnlayout.begin()
                .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
                .end();

            vertexNormalBH = bgfx::createVertexBuffer(
                bgfx::makeRef(
                    mrb.vertexNormalBufferData(),
                    mrb.vertexBufferSize() * sizeof(float)),
                vnlayout);
        }

        // vertex buffer (colors)
        if (mrb.vertexColorBufferData()) {
            bgfx::VertexLayout vclayout;
            vclayout.begin()
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .end();

            vertexColorBH = bgfx::createVertexBuffer(
                bgfx::makeRef(
                    mrb.vertexColorBufferData(),
                    mrb.vertexBufferSize() / 3 * sizeof(uint32_t)),
                vclayout);
        }

        // triangle index buffer
        triangleIndexBH = bgfx::createIndexBuffer(
            bgfx::makeRef(
                mrb.triangleBufferData(),
                mrb.triangleBufferSize() * sizeof(uint32_t)),
            BGFX_BUFFER_INDEX32);

        // triangle color buffer
        if (mrb.triangleColorBufferData()) {
            triangleColorBH = bgfx::createIndexBuffer(
                bgfx::makeRef(
                    mrb.triangleColorBufferData(),
                    mrb.triangleBufferSize() / 3 * sizeof(uint32_t)),
                BGFX_BUFFER_INDEX32 | BGFX_BUFFER_COMPUTE_READ);
        }
    }

    void createBGFXUniforms()
    {
        meshColorUH = bgfx::createUniform("meshColor", bgfx::UniformType::Vec4);
        userColorUH = bgfx::createUniform("userColor", bgfx::UniformType::Vec4);
    }

    void destroyBGFXBuffers()
    {
        if (bgfx::isValid(vertexCoordBH))
            bgfx::destroy(vertexCoordBH);

        if (bgfx::isValid(vertexNormalBH))
            bgfx::destroy(vertexNormalBH);

        if (bgfx::isValid(vertexColorBH))
            bgfx::destroy(vertexColorBH);

        if (bgfx::isValid(triangleIndexBH))
            bgfx::destroy(triangleIndexBH);

        if (bgfx::isValid(triangleColorBH))
            bgfx::destroy(triangleColorBH);
    }

    void destroyBGFXUniforms()
    {
        if (bgfx::isValid(meshColorUH))
            bgfx::destroy(meshColorUH);
        if (bgfx::isValid(userColorUH))
            bgfx::destroy(userColorUH);
    }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_DRAWABLE_MESH_DRAWABLE_MESH_H
