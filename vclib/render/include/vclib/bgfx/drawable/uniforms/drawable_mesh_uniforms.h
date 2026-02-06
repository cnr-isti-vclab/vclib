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

class DrawableMeshUniforms
{
    float mMeshColor[4] = {0.5, 0.5, 0.5, 1.0};

    // x component: first chunk primitive id drawn (as uint)
    // y component: 8 texture stages with 4 bit each, to specify if texture is
    //    used (value != 15) and which stage is used for each texture (as uint):
    //    none|none|brdfLut|emissive|occlusion|normal|metallRough|baseColor
    float mMeshData[4] = {
        0.0, // as uint: first chunk primitive id drawn
        std::bit_cast<float>(0xFFFFFFFF), // 8 texture stages with 4 bit each
        0.0,
        0.0};

    Uniform mMeshColorUniform = Uniform("u_meshColor", bgfx::UniformType::Vec4);
    Uniform mMeshDataUniform  = Uniform("u_meshData", bgfx::UniformType::Vec4);

public:
    DrawableMeshUniforms() = default;

    template<MeshConcept MeshType>
    void setMeshColor(const MeshType& m)
    {
        if constexpr (HasColor<MeshType>) {
            mMeshColor[0] = m.color().redF();
            mMeshColor[1] = m.color().greenF();
            mMeshColor[2] = m.color().blueF();
            mMeshColor[3] = m.color().alphaF();
        }
    }

    void setFirstChunkIndex(uint firstChunkIndex)
    {
        mMeshData[0] = std::bit_cast<float>(firstChunkIndex);
    }

    void setBaseColorTextureStage(uint8_t stage)
    {
        // base color is the first texture stage
        setTextureStage(0, stage);
    }

    void bind() const
    {
        mMeshColorUniform.bind(mMeshColor);
        mMeshDataUniform.bind(mMeshData);
    }

private:
    void setTextureStage(uint8_t pos, uint8_t stage)
    {
        uint value = std::bit_cast<uint>(mMeshData[1]);

        set4BitStageValue(value, pos, stage);

        mMeshData[1] = std::bit_cast<float>(value);
    }


    static void set4BitStageValue(uint& value, uint8_t pos, uint8_t stage)
    {
        // value is a uint where 8 stages with 4 bit each are encoded
        // pos is a value between 0 and 8 to specify the bit positions to set
        // stage is a value between 0 and 15 to specify the stage to set
        // only the 4 bits corresponding to pos are set to stage, the others are
        // left unchanged

        assert(pos < 8);
        assert(stage < 16);

        uint mask = 0xF << (pos * 4); // mask to clear the bits at pos
        // clear bits at pos and set new stage
        value = (value & ~mask) | (uint(stage) << (pos * 4));
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_MESH_UNIFORMS_H
