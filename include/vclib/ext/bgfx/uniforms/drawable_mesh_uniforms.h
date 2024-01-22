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

#ifndef VCL_EXT_BGFX_UNIFORMS_DRAWABLE_MESH_UNIFORMS_H
#define VCL_EXT_BGFX_UNIFORMS_DRAWABLE_MESH_UNIFORMS_H

#include <vclib/render/mesh_render_buffers.h>

#include "shader_uniform.h"

namespace vcl::bgf {

class DrawableMeshUniforms
{
    float meshColor[4] = {0.5, 0.5, 0.5, 1.0};

    float model[16] = { // identity matrix
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

    ShaderUniform meshColorUniform =
        ShaderUniform("u_meshColor", bgfx::UniformType::Vec4);

    // ShaderUniform modelUH =
    //     ShaderUniform("u_model", bgfx::UniformType::Mat4);

public:
    DrawableMeshUniforms() = default;

    template<MeshConcept MeshType>
    void update(const MeshRenderBuffers<MeshType>& mrb)
    {
        std::copy(
            mrb.meshColorBufferData(),
            mrb.meshColorBufferData() + 4,
            meshColor);
    }

    void bind() const
    {
        meshColorUniform.bind(meshColor);
        //modelUH.bind(model);
    }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_UNIFORMS_DRAWABLE_MESH_UNIFORMS_H
