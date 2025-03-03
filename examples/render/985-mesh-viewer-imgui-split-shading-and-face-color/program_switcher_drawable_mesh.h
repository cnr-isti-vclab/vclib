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

#include "globals.h"
 
template<vcl::MeshConcept MeshType>
class ProgramSwitcherDrawableMesh : public vcl::DrawableMesh<MeshType>
{   
    using Parent = vcl::DrawableMesh<MeshType>;

    bgfx::ProgramHandle surfaceProgramHandles[6] = 
    {
        vcl::loadProgram("shaders/vs_surface", "shaders/fs_surface_none"),
        vcl::loadProgram("shaders/vs_surface", "shaders/fs_surface_flat"),
        vcl::loadProgram("shaders/vs_surface", "shaders/fs_surface_smooth"),
        vcl::loadProgram("shaders/vs_surface", "shaders/fs_surface_none_face_color_only"),
        vcl::loadProgram("shaders/vs_surface", "shaders/fs_surface_flat_face_color_only"),
        vcl::loadProgram("shaders/vs_surface", "shaders/fs_surface_smooth_face_color_only")
    };

    bgfx::ProgramHandle surfaceProgramSelector() const {
        uint32_t mul = Globals::useSplitFaceColor;
        uint32_t off = 0;
        if(mMRS.isSurface(vcl::MeshRenderInfo::Surface::SHADING_FLAT)){
            off = 1;
        }
        if(mMRS.isSurface(vcl::MeshRenderInfo::Surface::SHADING_SMOOTH)){
            off = 2;
        }
        return surfaceProgramHandles[3 * mul + off];
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
            
            if(Globals::useSplitShading){
                if(mMRS.isSurface(vcl::MeshRenderInfo::Surface::VISIBLE)){
                    bgfx::setState(state);
                    mMRB.bindTextures();
                    mMRB.bindVertexBuffers(mMRS);
                    mMRB.bindIndexBuffers(mMRS);
                    bindUniforms();
                    bgfx::submit(viewId, surfaceProgramSelector());
                }
            }else{
                Parent::draw(viewId);
            }
        }
};
 
#endif // PROGRAM_SWITCHER_DRAWABLE_MESH_H
 