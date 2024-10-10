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

#ifndef VCL_BGFX_RENDER_DRAWABLE_UNIFORMS_DRAWABLE_MESH_UNIFORMS_H
#define VCL_BGFX_RENDER_DRAWABLE_UNIFORMS_DRAWABLE_MESH_UNIFORMS_H

#include <vclib/render/drawable/mesh/mesh_render_data.h>

#include "shader_uniform.h"

namespace vcl {

class DrawableMeshUniforms
{
    float mMeshColor[4] = {0.5, 0.5, 0.5, 1.0};

    float mModelMatrix[16] = { // identity matrix
        1.0,
        0.0,
        0.0,
        0.0,
        0.0,
        1.0,
        0.0,
        0.0,
        0.0,
        0.0,
        1.0,
        0.0,
        0.0,
        0.0,
        0.0,
        1.0};

    ShaderUniform mMeshColorUniform =
        ShaderUniform("u_meshColor", bgfx::UniformType::Vec4);

    // ShaderUniform modelUH =
    //     ShaderUniform("u_model", bgfx::UniformType::Mat4);

public:
    DrawableMeshUniforms() = default;

    template<MeshConcept MeshType>
    void update(const MeshRenderData<MeshType>& mrb)
    {
        std::copy(
            mrb.meshColorBufferData(),
            mrb.meshColorBufferData() + 4,
            mMeshColor);
    }

    void bind() const
    {
        mMeshColorUniform.bind(mMeshColor);
        // modelUH.bind(mModelMatrix);
    }
};

} // namespace vcl

#endif // VCL_BGFX_RENDER_DRAWABLE_UNIFORMS_DRAWABLE_MESH_UNIFORMS_H
