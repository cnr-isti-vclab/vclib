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

#ifndef VCL_EXT_BGFX_DRAWABLE_MESH_H
#define VCL_EXT_BGFX_DRAWABLE_MESH_H

#include <bgfx/bgfx.h>

#include <vclib/render/interfaces/drawable_mesh_i.h>

#include <vclib/ext/bgfx/context.h>

#include "mesh_render_buffers.h"
#include "uniforms/drawable_mesh_uniforms.h"
#include "uniforms/mesh_render_settings_uniforms.h"

namespace vcl::bgf {

template<MeshConcept MeshType>
class DrawableMesh : public DrawableMeshI
{
    using Base = DrawableMeshI;

    MeshRenderBuffers<MeshType> mrb;

    bgfx::ProgramHandle program =
        Context::programManager().getProgram(VclProgram::DRAWABLE_MESH);

    DrawableMeshUniforms       meshUniforms;
    MeshRenderSettingsUniforms meshRenderSettingsUniforms;

public:
    DrawableMesh() = default;

    DrawableMesh(const MeshType& mesh) : Base(mesh)
    {
        updateBuffers(mesh);
        mrs.setDefaultSettingsFromCapability();
    }

    ~DrawableMesh() = default;

    void updateBuffers(const MeshType& m)
    {
        if constexpr (HasName<MeshType>) {
            name() = m.name();
        }

        mrb = MeshRenderBuffers<MeshType>(m);
        mrs.setRenderCapabilityFrom(m);
        meshRenderSettingsUniforms.updateSettings(mrs);
        meshUniforms.update(mrb);
    }

    // DrawableObject implementation

    void init() {}

    virtual void setShaderProgram(const ShaderProgramI& p)
    {
        // todo
    };

    void draw(uint viewId)
    {
        if (bgfx::isValid(program)) {
            uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                             BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL;

            if (mrs.isPointCloudVisible()) {
                mrb.bindVertexBuffers();
                bindUniforms(VCL_MRS_PRIMITIVE_POINTS);

                bgfx::setState(state | BGFX_STATE_PT_POINTS);

                bgfx::submit(viewId, program);
            }

            if (mrs.isSurfaceVisible()) {
                mrb.bindVertexBuffers();
                mrb.bindIndexBuffers();
                bindUniforms(VCL_MRS_PRIMITIVE_TRIANGLES);

                bgfx::setState(state);

                bgfx::submit(viewId, program);
            }

            if (mrs.isWireframeVisible()) {
                mrb.bindVertexBuffers();
                mrb.bindIndexBuffers(false);
                bindUniforms(VCL_MRS_PRIMITIVE_LINES);

                bgfx::setState(state | BGFX_STATE_PT_LINES);

                bgfx::submit(viewId, program);
            }
        }
    }

    vcl::Point3d center() const { return (mrb.bbMin() + mrb.bbMax()) / 2; }

    double radius() const { return (mrb.bbMax() - mrb.bbMin()).norm() / 2; }

    DrawableMesh* clone() const { return new DrawableMesh(*this); }

    void setVisibility(bool vis)
    {
        DrawableMeshI::setVisibility(vis);
        meshRenderSettingsUniforms.updateSettings(mrs);
    }

    void setRenderSettings(const MeshRenderSettings& rs)
    {
        DrawableMeshI::setRenderSettings(rs);
        meshRenderSettingsUniforms.updateSettings(rs);
    }

private:
    void bindUniforms(uint primitive)
    {
        meshRenderSettingsUniforms.updatePrimitive(primitive);
        meshRenderSettingsUniforms.bind();
        meshUniforms.bind();
    }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_DRAWABLE_MESH_H
