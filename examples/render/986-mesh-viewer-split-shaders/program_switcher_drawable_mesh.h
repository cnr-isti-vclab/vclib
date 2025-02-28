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
#include <vclib/bgfx/drawable/drawable_mesh.h>
 
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers.h> 
#include <vclib/bgfx/drawable/uniforms/mesh_render_settings_uniforms.h>
 
#include <bgfx/bgfx.h>
 
template<vcl::MeshConcept MeshType>
class ProgramSwitcherDrawableMesh : public vcl::DrawableMesh<MeshType>
{   
    using Parent = vcl::DrawableMesh<MeshType>;

    bgfx::ProgramHandle mProgramSurfaceFlat =
        vcl::loadProgram(
            "shaders/vs_surface",
            "shaders/fs_surface_flat"
        );

    bgfx::ProgramHandle mProgramSurfaceSmooth =
        vcl::loadProgram(
            "shaders/vs_surface",
            "shaders/fs_surface_smooth"
        );

    bgfx::ProgramHandle mProgramSurfaceNone =
        vcl::loadProgram(
            "shaders/vs_surface",
            "shaders/fs_surface_none"
        );


    protected:

        using vcl::AbstractDrawableMesh::mMRS;
        using Parent::mMRB;
        using Parent::bindUniforms;

    public:
        using Parent::Parent;

        static inline bool *useSplitPrograms;

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
            
            if(*useSplitPrograms){
                if(mMRS.isSurface(vcl::MeshRenderInfo::Surface::VISIBLE)){
                    bgfx::ProgramHandle *selectedProgram;
                    if(mMRS.isSurface(vcl::MeshRenderInfo::Surface::SHADING_NONE)){
                        selectedProgram = &mProgramSurfaceNone;
                    }else if(mMRS.isSurface(vcl::MeshRenderInfo::Surface::SHADING_FLAT)){
                        selectedProgram = &mProgramSurfaceFlat;
                    }else if(mMRS.isSurface(vcl::MeshRenderInfo::Surface::SHADING_SMOOTH)){
                        selectedProgram = &mProgramSurfaceSmooth;
                    }
                    bgfx::setState(state);
                    mMRB.bindTextures();
                    mMRB.bindVertexBuffers(mMRS);
                    mMRB.bindIndexBuffers(mMRS);
                    bindUniforms();
                    bgfx::submit(viewId, *selectedProgram);
                }
            }else{
                Parent::draw(viewId);
            }
        }
};
 
#endif // PROGRAM_SWITCHER_DRAWABLE_MESH_H
 