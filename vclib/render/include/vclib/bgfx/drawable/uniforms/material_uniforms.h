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

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_MATERIAL_UNIFORMS_H
#define VCL_BGFX_DRAWABLE_UNIFORMS_MATERIAL_UNIFORMS_H

#include <vclib/bgfx/uniform.h>
#include <vclib/mesh.h>

namespace vcl {

class MaterialUniforms
{
    
    std::array<float, 4> mBaseColor = {1.0, 1.0, 1.0, 1.0};

    // metallic, roughness and occlusion are stored in the B, G and R channels 
    // respectively for consistency with textures
    std::array<float, 4> mMetallicRoughnessOcclusion = {
        1.0, // occlusion strength
        1.0, // roughness
        1.0, // metallic
        0.0};

    std::array<float, 4> mEmissiveColor = {0.0, 0.0, 0.0, 1.0};

    std::array<float, 4> mAlphaCutoff = {0.5, 0.0, 0.0, 0.0};

    std::array<float, 4> mNormalScale = {1.0, 0.0, 0.0, 0.0};

    std::array<float, 4> mSettings = {0.0, 0.0, 0.0, 0.0};

    Uniform mBaseColorUniform =
        Uniform("u_baseColorFactor", bgfx::UniformType::Vec4);

    Uniform mMetallicRoughnessOcclusionUniform =
        Uniform("u_metallicRoughnessOcclusionFactors", bgfx::UniformType::Vec4);

    Uniform mEmissiveColorUniform =
        Uniform("u_emissiveColorFactor", bgfx::UniformType::Vec4);

    Uniform mAlphaCutoffUniform =
        Uniform("u_alphaCutoff", bgfx::UniformType::Vec4);

    Uniform mNormalScaleUniform =
        Uniform("u_normalScale", bgfx::UniformType::Vec4);

    Uniform mSettingsUniform = Uniform("u_settings", bgfx::UniformType::Vec4);

public:
    MaterialUniforms() = default;

    const std::array<float, 4>& currentBaseColor() const { return mBaseColor; }

    const std::array<float, 4>& currentMetallicRoughnessOcclusion() const { return mMetallicRoughnessOcclusion; }

    const std::array<float, 4>& currentEmissiveColor() const { return mEmissiveColor; }

    const std::array<float, 4>& currentAlphaCutoff() const { return mAlphaCutoff; }

    const std::array<float, 4>& currentNormalScale() const { return mNormalScale; }

    const std::array<float, 4>& currentSettings() const { return mSettings; }

    void update(const Material& m, bool vertexColorAvailable)
    {
        int settings = 0;
        if (vertexColorAvailable) // per-vertex color available
            settings |= 1 << 0;

        if (m.alphaMode() ==
            Material::AlphaMode::ALPHA_MASK) { // alpha mode is MASK
            settings |= 1 << 1;
            mAlphaCutoff[0] = m.alphaCutoff();
        }

        for(int i = 0; i < toUnderlying(Material::TextureType::COUNT); ++i) {
            if(!m.texture(static_cast<Material::TextureType>(i)).image().isNull()) {
                settings |= 1 << (2 + i); // texture available, uses settings from 2 to 6
            }
        }

        mSettings[0] = float(settings);

        mBaseColor[0] = m.baseColor().redF();
        mBaseColor[1] = m.baseColor().greenF();
        mBaseColor[2] = m.baseColor().blueF();
        mBaseColor[3] = m.baseColor().alphaF();

        // metallic, roughness and occlusion are stored in the B, G and R channels
        // respectively for consistency with textures
        mMetallicRoughnessOcclusion[0] = m.occlusionStrength();
        mMetallicRoughnessOcclusion[1] = m.roughness();
        mMetallicRoughnessOcclusion[2] = m.metallic();

        mEmissiveColor[0] = m.emissiveColor().redF();
        mEmissiveColor[1] = m.emissiveColor().greenF();
        mEmissiveColor[2] = m.emissiveColor().blueF();
        mEmissiveColor[3] = m.emissiveColor().alphaF();

        mNormalScale[0] = m.normalScale();
    }

    void bind() const
    {
        mBaseColorUniform.bind(&mBaseColor);
        mMetallicRoughnessOcclusionUniform.bind(&mMetallicRoughnessOcclusion);
        mEmissiveColorUniform.bind(&mEmissiveColor);
        mAlphaCutoffUniform.bind(&mAlphaCutoff);
        mNormalScaleUniform.bind(&mNormalScale);
        mSettingsUniform.bind(&mSettings);
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_MATERIAL_UNIFORMS_H
