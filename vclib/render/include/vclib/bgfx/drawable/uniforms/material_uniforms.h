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
    
    std::array<float, 4> mMaterialColor = {1.0, 1.0, 1.0, 1.0};

    std::array<float, 4> mMetallicRoughness = {
        1.0, // metallic
        1.0, // roughness
        0.0,
        0.0};

    std::array<float, 4> mEmissiveColor = {0.0, 0.0, 0.0, 1.0};

    std::array<float, 4> mAlphaCutoff = {0.5, 0.0, 0.0, 0.0};

    std::array<float, 4> mSettings = {0.0, 0.0, 0.0, 0.0};

    Uniform mMaterialColorUniform =
        Uniform("u_materialColor", bgfx::UniformType::Vec4);

    Uniform mMetallicRoughnessUniform =
        Uniform("u_metallicRoughness", bgfx::UniformType::Vec4);

    Uniform mEmissiveColorUniform =
        Uniform("u_emissiveColor", bgfx::UniformType::Vec4);

    Uniform mAlphaCutoffUniform =
        Uniform("u_alphaCutoff", bgfx::UniformType::Vec4);

    Uniform mSettingsUniform = Uniform("u_settings", bgfx::UniformType::Vec4);

public:
    MaterialUniforms() = default;

    const std::array<float, 4>& currentMaterialColor() const { return mMaterialColor; }

    const std::array<float, 4>& currentMetallicRoughness() const { return mMetallicRoughness; }

    const std::array<float, 4>& currentEmissiveColor() const { return mEmissiveColor; }

    const std::array<float, 4>& currentAlphaCutoff() const { return mAlphaCutoff; }

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

        if(!m.texture(Material::Textures::BASE_COLOR).image().isNull()) {
            settings |= 1 << 2; // base color texture available
        }

        mSettings[0] = float(settings);

        mMaterialColor[0] = m.baseColor().redF();
        mMaterialColor[1] = m.baseColor().greenF();
        mMaterialColor[2] = m.baseColor().blueF();
        mMaterialColor[3] = m.baseColor().alphaF();

        mMetallicRoughness[0] = m.metallic();
        mMetallicRoughness[1] = m.roughness();

        mEmissiveColor[0] = m.emissiveColor().redF();
        mEmissiveColor[1] = m.emissiveColor().greenF();
        mEmissiveColor[2] = m.emissiveColor().blueF();
        mEmissiveColor[3] = m.emissiveColor().alphaF();
    }

    void bind() const
    {
        mMaterialColorUniform.bind(&mMaterialColor);
        mMetallicRoughnessUniform.bind(&mMetallicRoughness);
        mEmissiveColorUniform.bind(&mEmissiveColor);
        mAlphaCutoffUniform.bind(&mAlphaCutoff);
        mSettingsUniform.bind(&mSettings);
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_MATERIAL_UNIFORMS_H
