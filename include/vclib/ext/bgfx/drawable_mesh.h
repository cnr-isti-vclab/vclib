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
#include "uniforms/drawable_mesh_uniforms.h"
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
    bgfx::VertexBufferHandle vertexColorBH  = BGFX_INVALID_HANDLE;

    bgfx::IndexBufferHandle triangleIndexBH  = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle triangleNormalBH = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle triangleColorBH  = BGFX_INVALID_HANDLE;

    bgfx::IndexBufferHandle edgeIndexBH = BGFX_INVALID_HANDLE;

    DrawableMeshUniforms       meshUniforms;
    MeshRenderSettingsUniforms meshRenderSettingsUniforms;

public:
    DrawableMesh() = default;

    DrawableMesh(const DrawableMesh& oth) :
            GenericDrawableMesh(oth), mrb(oth.mrb),
            meshRenderSettingsUniforms(oth.meshRenderSettingsUniforms),
            meshUniforms(oth.meshUniforms)
    {
        // each drawable object has its own bgfx buffers
        createBGFXBuffers();
    }

    DrawableMesh(DrawableMesh&& oth) { swap(oth); }

    DrawableMesh(const MeshType& mesh)
    {
        updateBuffers(mesh);
        mrs.setDefaultSettingsFromCapability();
    }

    DrawableMesh& operator=(DrawableMesh oth)
    {
        swap(oth);
        return *this;
    }

    ~DrawableMesh() { destroyBGFXBuffers(); }

    void swap(DrawableMesh& oth)
    {
        GenericDrawableMesh::swap(oth);
        std::swap(program, oth.program);
        std::swap(mrb, oth.mrb);
        std::swap(vertexCoordBH, oth.vertexCoordBH);
        std::swap(vertexNormalBH, oth.vertexNormalBH);
        std::swap(vertexColorBH, oth.vertexColorBH);
        std::swap(triangleIndexBH, oth.triangleIndexBH);
        std::swap(triangleNormalBH, oth.triangleNormalBH);
        std::swap(triangleColorBH, oth.triangleColorBH);
        std::swap(edgeIndexBH, oth.edgeIndexBH);
        std::swap(meshUniforms, oth.meshUniforms);
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
        meshUniforms.update(mrb);

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

    void draw()
    {
        if (bgfx::isValid(program)) {
            uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                             BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL |
                             BGFX_STATE_MSAA;

            if (mrs.isPointCloudVisible()) {
                bindVertexBuffers();
                bindUniforms(VCL_MRS_PRIMITIVE_POINTS);

                bgfx::setState(state | BGFX_STATE_PT_POINTS);

                bgfx::submit(0, program);
            }

            if (mrs.isSurfaceVisible()) {
                bindVertexBuffers();
                bindIndexBuffers();
                bindUniforms(VCL_MRS_PRIMITIVE_TRIANGLES);

                bgfx::setState(state);

                bgfx::submit(0, program);
            }

            if (mrs.isWireframeVisible()) {
                bindVertexBuffers();
                bindIndexBuffers(false);
                bindUniforms(VCL_MRS_PRIMITIVE_LINES);

                bgfx::setState(state | BGFX_STATE_PT_LINES);

                bgfx::submit(0, program);
            }
        }
    }

    vcl::Point3d center() const { return (mrb.bbMin() + mrb.bbMax()) / 2; }

    double radius() const { return (mrb.bbMax() - mrb.bbMin()).norm() / 2; }

    DrawableMesh* clone() const { return new DrawableMesh(*this); }

    void setVisibility(bool vis)
    {
        GenericDrawableMesh::setVisibility(vis);
        meshRenderSettingsUniforms.updateSettings(mrs);
    }

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
                    mrb.vertexNumber() * sizeof(uint32_t)),
                vclayout);
        }

        // triangle index buffer
        if (mrb.triangleBufferData()) {
            triangleIndexBH = bgfx::createIndexBuffer(
                bgfx::makeRef(
                    mrb.triangleBufferData(),
                    mrb.triangleBufferSize() * sizeof(uint32_t)),
                BGFX_BUFFER_INDEX32);
        }

        // triangle normal buffer
        if (mrb.triangleNormalBufferData()) {
            triangleNormalBH = bgfx::createIndexBuffer(
                bgfx::makeRef(
                    mrb.triangleNormalBufferData(),
                    mrb.triangleNumber() * 3 * sizeof(float)),
                BGFX_BUFFER_COMPUTE_FORMAT_32X1 |
                    BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_COMPUTE_TYPE_FLOAT);
        }

        // triangle color buffer
        if (mrb.triangleColorBufferData()) {
            triangleColorBH = bgfx::createIndexBuffer(
                bgfx::makeRef(
                    mrb.triangleColorBufferData(),
                    mrb.triangleNumber() * sizeof(uint32_t)),
                BGFX_BUFFER_INDEX32 | BGFX_BUFFER_COMPUTE_READ);
        }

        // edge index buffer
        if (mrb.edgeBufferData()) {
            edgeIndexBH = bgfx::createIndexBuffer(
                bgfx::makeRef(
                    mrb.edgeBufferData(),
                    mrb.edgeBufferSize() * sizeof(uint32_t)),
                BGFX_BUFFER_INDEX32);
        }
    }

    void bindVertexBuffers()
    {
        bgfx::setVertexBuffer(0, vertexCoordBH);

        if (bgfx::isValid(vertexNormalBH)) { // vertex normals
            bgfx::setVertexBuffer(1, vertexNormalBH);
        }

        if (bgfx::isValid(vertexColorBH)) { // vertex colors
            bgfx::setVertexBuffer(2, vertexColorBH);
        }
    }

    void bindIndexBuffers(bool triangles = true)
    {
        if (triangles) {
            bgfx::setIndexBuffer(triangleIndexBH);

            if (bgfx::isValid(triangleColorBH)) { // triangle colors
                bgfx::setBuffer(1, triangleColorBH, bgfx::Access::Read);
            }

            if (bgfx::isValid(triangleNormalBH)) { // triangle normals
                bgfx::setBuffer(2, triangleNormalBH, bgfx::Access::Read);
            }
        }
        else {
            bgfx::setIndexBuffer(edgeIndexBH);
        }
    }

    void bindUniforms(uint primitive)
    {
        meshRenderSettingsUniforms.updatePrimitive(primitive);
        meshRenderSettingsUniforms.bind();
        meshUniforms.bind();
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

        if (bgfx::isValid(triangleNormalBH))
            bgfx::destroy(triangleNormalBH);

        if (bgfx::isValid(triangleColorBH))
            bgfx::destroy(triangleColorBH);

        if (bgfx::isValid(edgeIndexBH))
            bgfx::destroy(edgeIndexBH);
    }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_DRAWABLE_MESH_DRAWABLE_MESH_H
