/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_MESH_UNIFORMS_H
#define VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_MESH_UNIFORMS_H

#include <vclib/bgfx/uniform.h>
#include <vclib/mesh.h>

namespace vcl {

/**
 * @brief The DrawableMeshUniforms class is responsible for managing the
 * shader uniforms related to a drawable mesh.
 *
 * It provides a static interface to set the uniform data based on the
 * current mesh data and to bind the uniforms to the shader programs.
 */
class DrawableMeshUniforms
{
    inline static std::array<float, 4> sMeshColor = {0.5, 0.5, 0.5, 1.0};

    // sMeshData[0] -> as uint, first chunk primitive id drawn
    inline static std::array<float, 4> sMeshData = {0.0, 0.0, 0.0, 0.0};

    inline static Uniform sMeshColorUniform;
    inline static Uniform sMeshDataUniform;

public:
    DrawableMeshUniforms() = delete;

    template<MeshConcept MeshType>
    static void setColor(const MeshType& m)
    {
        if constexpr (HasColor<MeshType>) {
            sMeshColor[0] = m.color().redF();
            sMeshColor[1] = m.color().greenF();
            sMeshColor[2] = m.color().blueF();
            sMeshColor[3] = m.color().alphaF();
        }
    }

    static void setFirstChunkIndex(uint firstChunkIndex)
    {
        sMeshData[0] = std::bit_cast<float>(firstChunkIndex);
    }

    static void bind()
    {
        // lazy initialization
        // to avoid creating uniforms before bgfx is initialized
        if (!sMeshColorUniform.isValid())
            sMeshColorUniform = Uniform("u_meshColor", bgfx::UniformType::Vec4);
        if (!sMeshDataUniform.isValid())
            sMeshDataUniform = Uniform("u_meshData", bgfx::UniformType::Vec4);
        sMeshColorUniform.bind(sMeshColor.data());
        sMeshDataUniform.bind(sMeshData.data());
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_MESH_UNIFORMS_H
