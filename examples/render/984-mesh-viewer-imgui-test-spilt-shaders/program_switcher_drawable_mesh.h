/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef PROGRAM_SWITCHER_DRAWABLE_MESH_H
#define PROGRAM_SWITCHER_DRAWABLE_MESH_H
 
#include <vclib/algorithms/mesh/stat/bounding_box.h>
#include <vclib/render/drawable/drawable_mesh.h>
 
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers.h> 
#include <vclib/bgfx/drawable/uniforms/mesh_render_settings_uniforms.h>
 
#include <bgfx/bgfx.h>
#include <array>

#include "globals.h"
 
template<vcl::MeshConcept MeshType>
class ProgramSwitcherDrawableMesh : public vcl::DrawableMesh<MeshType>
{   
    using Parent = vcl::DrawableMesh<MeshType>;

    std::array<bgfx::ProgramHandle, 19> surfaceProgramHandles =
    {
        vcl::Context::instance().programManager().getProgram<vcl::VertFragProgram::DRAWABLE_MESH_SURFACE_UBER>(),
        vcl::Context::instance().programManager().getProgram<vcl::VertFragProgram::DRAWABLE_MESH_SURFACE_NONE_COLOR_VERTEX>(),
        vcl::Context::instance().programManager().getProgram<vcl::VertFragProgram::DRAWABLE_MESH_SURFACE_NONE_COLOR_MESH>(),
        vcl::Context::instance().programManager().getProgram<vcl::VertFragProgram::DRAWABLE_MESH_SURFACE_NONE_COLOR_FACE>(),
        vcl::Context::instance().programManager().getProgram<vcl::VertFragProgram::DRAWABLE_MESH_SURFACE_NONE_COLOR_USER>(),
        vcl::Context::instance().programManager().getProgram<vcl::VertFragProgram::DRAWABLE_MESH_SURFACE_NONE_TEX_VERTEX>(),
        vcl::Context::instance().programManager().getProgram<vcl::VertFragProgram::DRAWABLE_MESH_SURFACE_NONE_TEX_WEDGE>(),
        vcl::Context::instance().programManager().getProgram<vcl::VertFragProgram::DRAWABLE_MESH_SURFACE_FLAT_COLOR_VERTEX>(),
        vcl::Context::instance().programManager().getProgram<vcl::VertFragProgram::DRAWABLE_MESH_SURFACE_FLAT_COLOR_MESH>(),
        vcl::Context::instance().programManager().getProgram<vcl::VertFragProgram::DRAWABLE_MESH_SURFACE_FLAT_COLOR_FACE>(),
        vcl::Context::instance().programManager().getProgram<vcl::VertFragProgram::DRAWABLE_MESH_SURFACE_FLAT_COLOR_USER>(),
        vcl::Context::instance().programManager().getProgram<vcl::VertFragProgram::DRAWABLE_MESH_SURFACE_FLAT_TEX_VERTEX>(),
        vcl::Context::instance().programManager().getProgram<vcl::VertFragProgram::DRAWABLE_MESH_SURFACE_FLAT_TEX_WEDGE>(),
        vcl::Context::instance().programManager().getProgram<vcl::VertFragProgram::DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_VERTEX>(),
        vcl::Context::instance().programManager().getProgram<vcl::VertFragProgram::DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_MESH>(),
        vcl::Context::instance().programManager().getProgram<vcl::VertFragProgram::DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_FACE>(),
        vcl::Context::instance().programManager().getProgram<vcl::VertFragProgram::DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_USER>(),
        vcl::Context::instance().programManager().getProgram<vcl::VertFragProgram::DRAWABLE_MESH_SURFACE_SMOOTH_TEX_VERTEX>(),
        vcl::Context::instance().programManager().getProgram<vcl::VertFragProgram::DRAWABLE_MESH_SURFACE_SMOOTH_TEX_WEDGE>()
    };

    bgfx::ProgramHandle surfaceProgramSelector() const {
        uint32_t mul = 0;
        uint32_t off = 0;
        if(mMRS.isSurface(vcl::MeshRenderInfo::Surface::SHADING_FLAT)){
            mul = 1;
        }
        if(mMRS.isSurface(vcl::MeshRenderInfo::Surface::SHADING_SMOOTH)){
            mul = 2;
        }
        if(mMRS.isSurface(vcl::MeshRenderInfo::Surface::COLOR_MESH)){
            off = 1;
        }
        if(mMRS.isSurface(vcl::MeshRenderInfo::Surface::COLOR_FACE)){
            off = 2;
        }
        if(mMRS.isSurface(vcl::MeshRenderInfo::Surface::COLOR_USER)){
            off = 3;
        }
        if(mMRS.isSurface(vcl::MeshRenderInfo::Surface::COLOR_VERTEX_TEX)){
            off = 4;
        }
        if(mMRS.isSurface(vcl::MeshRenderInfo::Surface::COLOR_WEDGE_TEX)){
            off = 5;
        }
        return Globals::useSplitShading ? surfaceProgramHandles[6 * mul + off + 1] : surfaceProgramHandles[0];
    }

    protected:

        using vcl::AbstractDrawableMesh::mMRS;
        using Parent::mMRB;
        using Parent::bindUniforms;

    public:
        using Parent::Parent;

        std::shared_ptr<vcl::DrawableObject> clone() const& override
        {
            return std::make_shared<ProgramSwitcherDrawableMesh>(*this);
        }

        std::shared_ptr<vcl::DrawableObject> clone() && override
        {
            return std::make_shared<ProgramSwitcherDrawableMesh>(std::move(*this));
        }

        void draw(vcl::uint viewId) const override
        {
            uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                         BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL |
                         BGFX_STATE_BLEND_NORMAL;
            
            if(mMRS.isSurface(vcl::MeshRenderInfo::Surface::VISIBLE)){
                bgfx::setState(state);
                mMRB.bindTextures();
                mMRB.bindVertexBuffers(mMRS);
                mMRB.bindIndexBuffers(mMRS);
                bindUniforms();
                bgfx::submit(viewId, surfaceProgramSelector());
            }
            
        }
};
 
#endif // PROGRAM_SWITCHER_DRAWABLE_MESH_H
 