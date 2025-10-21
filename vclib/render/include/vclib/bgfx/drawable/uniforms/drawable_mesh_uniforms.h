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

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_MESH_UNIFORMS_H
#define VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_MESH_UNIFORMS_H

#include <vclib/bgfx/uniform.h>
#include <vclib/mesh.h>

namespace vcl {

class DrawableMeshUniforms
{
    float mMeshColor[4] = {0.5, 0.5, 0.5, 1.0};

    float mMeshData[4] = {
        0.0, // as uint: first chunk primitive id drawn
        0.0,
        0.0,
        0.0};

    Uniform mMeshColorUniform = Uniform("u_meshColor", bgfx::UniformType::Vec4);
    Uniform mMeshDataUniform  = Uniform("u_meshData", bgfx::UniformType::Vec4);

public:
    DrawableMeshUniforms() = default;

    const float* currentMeshColor() const { return mMeshColor; }

    template<MeshConcept MeshType>
    void update(const MeshType& m)
    {
        if constexpr (HasColor<MeshType>) {
            mMeshColor[0] = m.color().redF();
            mMeshColor[1] = m.color().greenF();
            mMeshColor[2] = m.color().blueF();
            mMeshColor[3] = m.color().alphaF();
        }
    }

    void updateFirstChunkIndex(uint firstChunkIndex)
    {
        mMeshData[0] = Uniform::uintBitsToFloat(firstChunkIndex);
    }

    void bind() const
    {
        mMeshColorUniform.bind(mMeshColor);
        mMeshDataUniform.bind(mMeshData);
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_MESH_UNIFORMS_H
