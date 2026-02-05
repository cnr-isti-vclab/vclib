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

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_MATERIAL_UNIFORMS_H
#define VCL_BGFX_DRAWABLE_UNIFORMS_MATERIAL_UNIFORMS_H

#include <vclib/bgfx/drawable/mesh/pbr_macros.h>
#include <vclib/bgfx/uniform.h>
#include <vclib/render/settings/draw_object_settings.h>

#include <vclib/mesh.h>

namespace vcl {

class MaterialUniforms
{
    static const uint N_TEXTURES = toUnderlying(Material::TextureType::COUNT);

    std::array<float, 4> mBaseColor = {1.0, 1.0, 1.0, 1.0};

    // metallic, roughness and occlusion are stored in the B, G and R channels
    // respectively for consistency with textures
    std::array<float, 4> mFactorsPack = {
        1.0, // occlusion strength
        1.0, // roughness
        1.0, // metallic
        1.0  // normal scale
    };

    // emissive color factor stored in RGB channels, alpha channel is unused so
    // it can be used to store the alpha cutoff when needed
    std::array<float, 4> mEmissiveAlphaCutoffPack = {0.0, 0.0, 0.0, 0.5};

    // settings packed in a vec4
    // .x : pbr settings
    // .y : texture settings
    std::array<float, 4> mSettings = {0.0, 0.0, 0.0, 0.0};

    Uniform mBaseColorUniform =
        Uniform("u_baseColorFactor", bgfx::UniformType::Vec4);

    Uniform mFactorsPackUniform =
        Uniform("u_FactorsPack", bgfx::UniformType::Vec4);

    Uniform mEmissiveAlphaCutoffPackUniform =
        Uniform("u_emissiveAlphaCutoffPack", bgfx::UniformType::Vec4);

    Uniform mSettingsUniform = Uniform("u_settings", bgfx::UniformType::Vec4);

public:
    MaterialUniforms() = default;

    const std::array<float, 4>& currentBaseColor() const { return mBaseColor; }

    const std::array<float, 4>& currentFactorsPack() const
    {
        return mFactorsPack;
    }

    const std::array<float, 4>& currentEmissiveAlphaCutoffPack() const
    {
        return mEmissiveAlphaCutoffPack;
    }

    const std::array<float, 4>& currentSettings() const { return mSettings; }

    void update(
        const Material&                     m,
        bool                                vertexColorAvailable,
        const std::array<bool, N_TEXTURES>& textureAvailable,
        bool                                vertexTangentAvailable,
        bool                                imageBasedLighting)
    {
        uint pbrSettingBits = 0;

        if (vertexColorAvailable) // per-vertex color available
            pbrSettingBits |= 1 << VCL_PBR_VERTEX_COLOR;

        if (vertexTangentAvailable) // per-vertex tangent available
            pbrSettingBits |= 1 << VCL_PBR_VERTEX_TANGENT;

        if (m.alphaMode() ==
            Material::AlphaMode::ALPHA_MASK) { // alpha mode is MASK
            pbrSettingBits |= 1 << VCL_PBR_IS_ALPHA_MODE_MASK;
            mEmissiveAlphaCutoffPack[3] = m.alphaCutoff();
        }

        if (imageBasedLighting) {
            pbrSettingBits |= 1 << VCL_PBR_IMAGE_BASED_LIGHTING;
        }

        mSettings[0] = std::bit_cast<float>(pbrSettingBits);

        uint textureSettings = 0;

        for (int i = 0; i < N_TEXTURES; ++i) {
            if (textureAvailable[i]) {
                // texture available, uses settings from 0 to N_TEXTURES
                textureSettings |= 1 << (VCL_PBR_TEXTURE_BASE_COLOR + i);
            }
        }

        mSettings[1] = std::bit_cast<float>(textureSettings);

        mBaseColor[0] = m.baseColor().redF();
        mBaseColor[1] = m.baseColor().greenF();
        mBaseColor[2] = m.baseColor().blueF();
        mBaseColor[3] = m.baseColor().alphaF();

        // metallic, roughness and occlusion are stored in the B, G and R
        // channels respectively for consistency with textures
        mFactorsPack[0] = m.occlusionStrength();
        mFactorsPack[1] = m.roughness();
        mFactorsPack[2] = m.metallic();
        mFactorsPack[3] = m.normalScale();

        mEmissiveAlphaCutoffPack[0] = m.emissiveColor().redF();
        mEmissiveAlphaCutoffPack[1] = m.emissiveColor().greenF();
        mEmissiveAlphaCutoffPack[2] = m.emissiveColor().blueF();
    }

    void bind() const
    {
        mBaseColorUniform.bind(&mBaseColor);
        mFactorsPackUniform.bind(&mFactorsPack);
        mEmissiveAlphaCutoffPackUniform.bind(&mEmissiveAlphaCutoffPack);
        mSettingsUniform.bind(&mSettings);
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_MATERIAL_UNIFORMS_H
