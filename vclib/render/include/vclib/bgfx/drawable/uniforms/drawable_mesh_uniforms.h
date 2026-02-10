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

    // sMeshData[0]: as uint, mesh id
    // sMeshData[1]: as uint, first chunk primitive id drawn
    // sMeshData[2]: 8 texture stages with 4 bit each, to specify if texture is
    //    used (value != 15) and which stage is used for each texture (as uint):
    //    none|none|brdfLut|emissive|occlusion|normal|metallRough|baseColor
    inline static std::array<float, 4> sMeshData =
        {0.0, 0.0, std::bit_cast<float>(0xFFFFFFFF), 0.0};

    inline static Uniform sMeshColorUniform;
    inline static Uniform sMeshDataUniform;

public:
    enum class TextureType {
        BASE_COLOR,
        METALLIC_ROUGHNESS,
        NORMAL,
        OCCLUSION,
        EMISSIVE,
        BRDF_LUT,
        COUNT
    };

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

    static void setMeshId(uint meshId)
    {
        sMeshData[0] = std::bit_cast<float>(meshId);
    }

    static void resetTextureStages()
    {
        // 8 texture stages with 4 bit each, all set to 15 (not used)
        sMeshData[2] = std::bit_cast<float>(0xFFFFFFFF);
    }

    static void setTextureStage(TextureType type, uint8_t stage)
    {
        assert(toUnderlying(type) < toUnderlying(TextureType::COUNT));

        if (toUnderlying(type) < 8) { // use y
            setYTextureStage(toUnderlying(type), stage);
        }
    }

    static void setFirstChunkIndex(uint firstChunkIndex)
    {
        sMeshData[1] = std::bit_cast<float>(firstChunkIndex);
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

private:
    static void setYTextureStage(uint8_t pos, uint8_t stage)
    {
        uint value = std::bit_cast<uint>(sMeshData[2]);

        set4BitStageValue(value, pos, stage);

        sMeshData[2] = std::bit_cast<float>(value);
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
