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

    float mMaterialColor[4] = {1.0, 1.0, 1.0, 1.0};

    float mMetallicRoughness[4] = {
        0.0, // metallic
        0.5, // roughness
        0.0, 
        0.0};

    float mSettings[4] = {0.0, 0.0, 0.0, 0.0};

    float mModelMatrix[16] = { // identity matrix
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

    Uniform mMeshColorUniform = Uniform("u_meshColor", bgfx::UniformType::Vec4);

    Uniform mMaterialColorUniform = Uniform("u_materialColor", bgfx::UniformType::Vec4);

    Uniform mMetallicRoughnessUniform = Uniform("u_metallicRoughness", bgfx::UniformType::Vec4);

    Uniform mSettingsUniform = Uniform("u_settings", bgfx::UniformType::Vec4);

    // ShaderUniform modelUH =
    //     ShaderUniform("u_model", bgfx::UniformType::Mat4);

public:
    DrawableMeshUniforms() = default;

    const float* currentMeshColor() const { return mMeshColor; }

    const float* currentMaterialColor() const { return mMaterialColor; }

    const float* currentMetallicRoughness() const { return mMetallicRoughness; }

    const float* currentSettings() const { return mSettings; }

    const float* currentModelMatrix() const { return mModelMatrix; }

    template<MeshConcept MeshType>
    void update(const MeshType& m)
    {
        if constexpr (HasColor<MeshType>) {
            mMeshColor[0] = m.color().redF();
            mMeshColor[1] = m.color().greenF();
            mMeshColor[2] = m.color().blueF();
            mMeshColor[3] = m.color().alphaF();
        }

        if constexpr (HasMaterials<MeshType>) {

            int settings = 0;
            if(isPerVertexColorAvailable(m)) settings |= 1 << 0; // per-vertex color available
            if(m.materialsNumber() > 0) settings |= 1 << 1; // material available
            mSettings[0] = float(settings);

            if(settings & 1 << 1) { // material available
                mMaterialColor[0] = m.materials()[0].baseColor().redF();
                mMaterialColor[1] = m.materials()[0].baseColor().greenF();
                mMaterialColor[2] = m.materials()[0].baseColor().blueF();
                mMaterialColor[3] = m.materials()[0].baseColor().alphaF();

                mMetallicRoughness[0] = m.materials()[0].metallic();
                mMetallicRoughness[1] = m.materials()[0].roughness();
            }
        }
    }

    void bind() const
    {
        mMeshColorUniform.bind(mMeshColor);
        mMaterialColorUniform.bind(mMaterialColor);
        mMetallicRoughnessUniform.bind(mMetallicRoughness);
        mSettingsUniform.bind(mSettings);
        // modelUH.bind(mModelMatrix);
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_MESH_UNIFORMS_H
