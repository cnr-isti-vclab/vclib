// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_MATERIAL_UNIFORMS_H
#define VCL_BGFX_DRAWABLE_UNIFORMS_MATERIAL_UNIFORMS_H

#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>
#include <vclib/bgfx/static_uniform.h>
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
    // it can be used to store the emissive strength
    static inline std::array<float, 4> sEmissivePack =
        {0.0, 0.0, 0.0, 1.0};

    // alpha cutoff and maybe other alpha related settings can be stored here
    static inline std::array<float, 4> sAlphaPack = {-1.0, 0.0, 0.0, 0.0};

    static inline StaticUniform sBaseColorUniform {
        "u_baseColorFactor",
        bgfx::UniformType::Vec4};
    static inline StaticUniform sFactorsPackUniform {
        "u_FactorsPack",
        bgfx::UniformType::Vec4};
    static inline StaticUniform sEmissivePackUniform {
        "u_emissivePack",
        bgfx::UniformType::Vec4};
    static inline StaticUniform sAlphaPackUniform {
        "u_alphaPack",
        bgfx::UniformType::Vec4};

public:
    MaterialUniforms() = delete;

    static void set(const Material& m)
    {
        if (m.alphaMode() ==
            Material::AlphaMode::ALPHA_MASK) { // alpha mode is MASK
            sAlphaPack[0] = m.alphaCutoff();
        }
        else {
            sAlphaPack[0] = -1.0f;
        }

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


        sEmissivePack[0] = m.emissiveColor().redF();
        sEmissivePack[1] = m.emissiveColor().greenF();
        sEmissivePack[2] = m.emissiveColor().blueF();
        sEmissivePack[3] = m.emissiveStrength();
    }

    static void bind()
    {
        sBaseColorUniform.bind(sBaseColor.data());
        sFactorsPackUniform.bind(sFactorsPack.data());
        sEmissivePackUniform.bind(sEmissivePack.data());
        sAlphaPackUniform.bind(sAlphaPack.data());
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_MATERIAL_UNIFORMS_H
