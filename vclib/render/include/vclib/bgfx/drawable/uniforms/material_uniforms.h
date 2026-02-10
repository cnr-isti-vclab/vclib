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

#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>
#include <vclib/bgfx/uniform.h>
#include <vclib/render/settings/draw_object_settings.h>

#include <vclib/mesh.h>

namespace vcl {

/**
 * @brief The MaterialUniforms class is responsible for managing the shader
 * uniforms related to a material.
 *
 * It provides a static interface to set the uniform data based on the
 * current material data and to bind the uniforms to the shader programs.
 */
class MaterialUniforms
{
    static inline std::array<float, 4> sBaseColor = {1.0, 1.0, 1.0, 1.0};

    // metallic, roughness and occlusion are stored in the B, G and R channels
    // respectively for consistency with textures
    static inline std::array<float, 4> sFactorsPack = {
        1.0, // occlusion strength
        1.0, // roughness
        1.0, // metallic
        1.0  // normal scale
    };

    // emissive color factor stored in RGB channels, alpha channel is unused so
    // it can be used to store the alpha cutoff when needed
    static inline std::array<float, 4> sEmissiveAlphaCutoffPack =
        {0.0, 0.0, 0.0, 0.5};

    // settings packed in a vec4
    // .x : pbr settings
    static inline std::array<float, 4> sSettings = {0.0, 0.0, 0.0, 0.0};

    static inline Uniform sBaseColorUniform;
    static inline Uniform sFactorsPackUniform;
    static inline Uniform sEmissiveAlphaCutoffPackUniform ;
    static inline Uniform sSettingsUniform;

public:
    MaterialUniforms() = delete;

    static void set(
        const Material& m,
        bool            vertexColorAvailable,
        bool            vertexTangentAvailable,
        bool            imageBasedLighting)
    {
        uint pbrSettingBits = 0;

        if (vertexColorAvailable) // per-vertex color available
            pbrSettingBits |= 1 << VCL_PBR_VERTEX_COLOR;

        if (vertexTangentAvailable) // per-vertex tangent available
            pbrSettingBits |= 1 << VCL_PBR_VERTEX_TANGENT;

        if (m.alphaMode() ==
            Material::AlphaMode::ALPHA_MASK) { // alpha mode is MASK
            pbrSettingBits |= 1 << VCL_PBR_IS_ALPHA_MODE_MASK;
            sEmissiveAlphaCutoffPack[3] = m.alphaCutoff();
        }

        if (imageBasedLighting) {
            pbrSettingBits |= 1 << VCL_PBR_IMAGE_BASED_LIGHTING;
        }

        sSettings[0] = std::bit_cast<float>(pbrSettingBits);

        sBaseColor[0] = m.baseColor().redF();
        sBaseColor[1] = m.baseColor().greenF();
        sBaseColor[2] = m.baseColor().blueF();
        sBaseColor[3] = m.baseColor().alphaF();

        // metallic, roughness and occlusion are stored in the B, G and R
        // channels respectively for consistency with textures
        sFactorsPack[0] = m.occlusionStrength();
        sFactorsPack[1] = m.roughness();
        sFactorsPack[2] = m.metallic();
        sFactorsPack[3] = m.normalScale();

        sEmissiveAlphaCutoffPack[0] = m.emissiveColor().redF();
        sEmissiveAlphaCutoffPack[1] = m.emissiveColor().greenF();
        sEmissiveAlphaCutoffPack[2] = m.emissiveColor().blueF();
    }

    static void bind()
    {
        // lazy initialization
        // to avoid creating uniforms before bgfx is initialized
        if (!sBaseColorUniform.isValid())
            sBaseColorUniform =
                Uniform("u_baseColorFactor", bgfx::UniformType::Vec4);
        if (!sFactorsPackUniform.isValid())
            sFactorsPackUniform = Uniform("u_FactorsPack", bgfx::UniformType::Vec4);
        if (!sEmissiveAlphaCutoffPackUniform.isValid())
            sEmissiveAlphaCutoffPackUniform =
                Uniform("u_emissiveAlphaCutoffPack", bgfx::UniformType::Vec4);
        if (!sSettingsUniform.isValid())
            sSettingsUniform = Uniform("u_settings", bgfx::UniformType::Vec4);

        sBaseColorUniform.bind(&sBaseColor);
        sFactorsPackUniform.bind(&sFactorsPack);
        sEmissiveAlphaCutoffPackUniform.bind(&sEmissiveAlphaCutoffPack);
        sSettingsUniform.bind(&sSettings);
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_MATERIAL_UNIFORMS_H
