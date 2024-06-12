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

#ifndef VCL_BGFX_RENDER_DRAWABLE_DRAWABLE_MESH_H
#define VCL_BGFX_RENDER_DRAWABLE_DRAWABLE_MESH_H

#include <bgfx/bgfx.h>

#include <vclib/render/interfaces/drawable_mesh_i.h>

#include <vclib_bgfx/render/context.h>
#include <vclib_bgfx/render/drawable/mesh/mesh_render_buffers.h>
#include <vclib_bgfx/render/drawable/uniforms/drawable_mesh_uniforms.h>
#include <vclib_bgfx/render/drawable/uniforms/mesh_render_settings_uniforms.h>

namespace vcl {

template<MeshConcept MeshType>
class DrawableMesh : public DrawableMeshI, public MeshType
{
    MeshRenderBuffers<MeshType> mMRB;

    bgfx::ProgramHandle mProgram =
        Context::programManager().getProgram(VclProgram::DRAWABLE_MESH);

    DrawableMeshUniforms               mMeshUniforms;
    mutable MeshRenderSettingsUniforms mMeshRenderSettingsUniforms;

public:
    DrawableMesh() = default;

    DrawableMesh(const MeshType& mesh) : DrawableMeshI(mesh), MeshType(mesh)
    {
        updateBuffers();
    }

    ~DrawableMesh() = default;

    void updateBuffers()
    {
        if constexpr (HasName<MeshType>) {
            DrawableMeshI::name() = MeshType::name();
        }

        mMRB = MeshRenderBuffers<MeshType>(*this);
        mMRS.setRenderCapabilityFrom(*this);
        mMeshRenderSettingsUniforms.updateSettings(mMRS);
        mMeshUniforms.update(mMRB);
    }

    // DrawableObject implementation

    void init() override {}

    void draw(uint viewId) const override
    {
        if (bgfx::isValid(mProgram)) {
            uint64_t state =
                0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL |
                BGFX_STATE_BLEND_FUNC(
                    BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);

            if (mMRS.isSurfaceVisible()) {
                mMRB.bindTextures(); // Bind textures before vertex buffers!!
                mMRB.bindVertexBuffers(mMRS);
                mMRB.bindIndexBuffers();
                bindUniforms(VCL_MRS_DRAWING_SURFACE);

                bgfx::setState(state);

                bgfx::submit(viewId, mProgram);
            }

            if (mMRS.isWireframeVisible()) {
                mMRB.bindVertexBuffers(mMRS);
                mMRB.bindIndexBuffers(mMRB.WIREFRAME);
                bindUniforms(VCL_MRS_DRAWING_WIREFRAME);

                bgfx::setState(state | BGFX_STATE_PT_LINES);

                bgfx::submit(viewId, mProgram);
            }

            if (mMRS.isPointCloudVisible()) {
                mMRB.bindVertexBuffers(mMRS);
                bindUniforms(VCL_MRS_DRAWING_POINTS);

                bgfx::setState(state | BGFX_STATE_PT_POINTS);

                bgfx::submit(viewId, mProgram);
            }

            if (mMRS.isEdgesVisible()) {
                mMRB.bindVertexBuffers(mMRS);
                mMRB.bindIndexBuffers(mMRB.EDGES);
                bindUniforms(VCL_MRS_DRAWING_EDGES);

                bgfx::setState(state | BGFX_STATE_PT_LINES);

                bgfx::submit(viewId, mProgram);
            }
        }
    }

    Box3d boundingBox() const override
    {
        return Box3d(mMRB.bbMin(), mMRB.bbMax());
    }

    std::shared_ptr<DrawableObjectI> clone() const override
    {
        return std::make_shared<DrawableMesh>(*this);
    }

    void setVisibility(bool vis) override
    {
        DrawableMeshI::setVisibility(vis);
        mMeshRenderSettingsUniforms.updateSettings(mMRS);
    }

    void setRenderSettings(const MeshRenderSettings& rs) override
    {
        DrawableMeshI::setRenderSettings(rs);
        mMeshRenderSettingsUniforms.updateSettings(rs);
    }

private:
    void bindUniforms(uint primitive) const
    {
        mMeshRenderSettingsUniforms.updatePrimitive(primitive);
        mMeshRenderSettingsUniforms.bind();
        mMeshUniforms.bind();
    }
};

} // namespace vcl

#endif // VCL_BGFX_RENDER_DRAWABLE_DRAWABLE_MESH_H
