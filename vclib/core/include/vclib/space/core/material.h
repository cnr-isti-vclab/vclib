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

#ifndef VCL_SPACE_CORE_MATERIAL_H
#define VCL_SPACE_CORE_MATERIAL_H

#include "color.h"
#include "texture.h"

#include <vclib/base.h>

namespace vcl {

/**
 * @brief Material class represents a material with properties needed for
 * rendering (PBR).
 */
// TODO - Add support for textures, normal maps, and other PBR properties.
class Material
{
public:
    enum class AlphaMode { ALPHA_OPAQUE, ALPHA_MASK, ALPHA_BLEND };
    enum class TextureType {
        BASE_COLOR,
        METALLIC_ROUGHNESS,
        NORMAL,
        OCCLUSION,
        EMISSIVE
    };

private:
    std::string mName;

    // essential PBR properties
    Color mBaseColor = Color::White;

    float mMetallic  = 1.0f;

    float mRoughness = 1.0f;

    // optional PBR properties
    Color mEmissiveColor = Color::Black;

    AlphaMode mAlphaMode = AlphaMode::ALPHA_OPAQUE;

    float mAlphaCutoff = 0.5f; // only used when mAlphaMode is MASK

    bool mDoubleSided = false;

    Texture mBaseColorTexture;

    Texture mMetallicRoughnessTexture;

    Texture mNormalTexture;

    Texture mOcclusionTexture;

    Texture mEmissiveTexture;

public:
    Material() {}

    const std::string& name() const { return mName; }

    std::string& name() { return mName; }

    const Color& baseColor() const { return mBaseColor; }

    Color& baseColor() { return mBaseColor; }

    float metallic() const { return mMetallic; }

    float& metallic() { return mMetallic; }

    float roughness() const { return mRoughness; }

    float& roughness() { return mRoughness; }

    const Color& emissiveColor() const { return mEmissiveColor; }

    Color& emissiveColor() { return mEmissiveColor; }

    AlphaMode alphaMode() const { return mAlphaMode; }

    AlphaMode& alphaMode() { return mAlphaMode; }

    float alphaCutoff() const { return mAlphaCutoff; }

    float& alphaCutoff() { return mAlphaCutoff; }

    bool doubleSided() const { return mDoubleSided; }

    bool& doubleSided() { return mDoubleSided; }

    // const Texture& baseColorTexture() const { return mBaseColorTexture; }

    // Texture& baseColorTexture() { return mBaseColorTexture; }

    // const Texture& metallicRoughnessTexture() const { return mMetallicRoughnessTexture; }

    // Texture& metallicRoughnessTexture() { return mMetallicRoughnessTexture; }

    // const Texture& normalTexture() const { return mNormalTexture; }

    // Texture& normalTexture() { return mNormalTexture; }

    // const Texture& occlusionTexture() const { return mOcclusionTexture; }

    // Texture& occlusionTexture() { return mOcclusionTexture; }

    // const Texture& emissiveTexture() const { return mEmissiveTexture; }

    // Texture& emissiveTexture() { return mEmissiveTexture; }

    const Texture& texture(TextureType type) const
    {
        switch(type)
        {
            case TextureType::BASE_COLOR: return mBaseColorTexture;
            case TextureType::METALLIC_ROUGHNESS: return mMetallicRoughnessTexture;
            case TextureType::NORMAL: return mNormalTexture;
            case TextureType::OCCLUSION: return mOcclusionTexture;
            case TextureType::EMISSIVE: return mEmissiveTexture;
            default: throw std::runtime_error("Invalid texture type");
        }
    }

    Texture& texture(TextureType type)
    {
        switch(type)
        {
            case TextureType::BASE_COLOR: return mBaseColorTexture;
            case TextureType::METALLIC_ROUGHNESS: return mMetallicRoughnessTexture;
            case TextureType::NORMAL: return mNormalTexture;
            case TextureType::OCCLUSION: return mOcclusionTexture;
            case TextureType::EMISSIVE: return mEmissiveTexture;
            default: throw std::runtime_error("Invalid texture type");
        }
    }

    void serialize(std::ostream& os) const
    {
        vcl::serialize(os, mName);
        mBaseColor.serialize(os);
        vcl::serialize(os, mMetallic, mRoughness);
        mEmissiveColor.serialize(os);
        vcl::serialize(os, mAlphaMode, mAlphaCutoff);
        mBaseColorTexture.serialize(os);
        vcl::serialize(os, mDoubleSided);
        mMetallicRoughnessTexture.serialize(os);
        mNormalTexture.serialize(os);
        mOcclusionTexture.serialize(os);
        mEmissiveTexture.serialize(os);
    }

    void deserialize(std::istream& is)
    {
        vcl::deserialize(is, mName);
        mBaseColor.deserialize(is);
        vcl::deserialize(is, mMetallic, mRoughness);
        mEmissiveColor.deserialize(is);
        vcl::deserialize(is, mAlphaMode, mAlphaCutoff);
        mBaseColorTexture.deserialize(is);
        vcl::deserialize(is, mDoubleSided);
        mMetallicRoughnessTexture.deserialize(is);
        mNormalTexture.deserialize(is);
        mOcclusionTexture.deserialize(is);
        mEmissiveTexture.deserialize(is);
    }
};

/* Concepts */

/**
 * @brief A concept representing a Material.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a Material class.
 *
 * @tparam T: The type to be tested for conformity to the MaterialConcept.
 *
 * @ingroup space_core
 */
template<typename T>
concept MaterialConcept = std::derived_from< // same type or derived type
    std::remove_cvref_t<T>,
    Material>;

} // namespace vcl

#endif // VCL_SPACE_CORE_MATERIAL_H
