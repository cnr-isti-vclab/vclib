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
#include "image.h"
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
        BASE_COLOR = 0,
        METALLIC_ROUGHNESS,
        NORMAL,
        OCCLUSION,
        EMISSIVE,
        COUNT
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
    
    Texture mBaseColorTexture;
    
    Texture mMetallicRoughnessTexture;
    
    Texture mNormalTexture;

    float mNormalScale = 1.0f;
    
    Texture mOcclusionTexture;

    float mOcclusionStrength = 1.0f;
    
    Texture mEmissiveTexture;
    
    bool mDoubleSided = false;
    
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

    float normalScale() const { return mNormalScale; }

    float& normalScale() { return mNormalScale; }

    float occlusionStrength() const { return mOcclusionStrength; }

    float& occlusionStrength() { return mOcclusionStrength; }

    const Texture& baseColorTexture() const { return mBaseColorTexture; }

    Texture& baseColorTexture() { return mBaseColorTexture; }

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

    const Texture& texture(uint type) const
    {
        return texture(static_cast<TextureType>(type));
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

    Texture& texture(uint type)
    {
        return texture(static_cast<TextureType>(type));
    }

    void serialize(std::ostream& os) const
    {
        vcl::serialize(os, mName);
        mBaseColor.serialize(os);
        vcl::serialize(os, mMetallic, mRoughness);
        mEmissiveColor.serialize(os);
        vcl::serialize(os, mAlphaMode, mAlphaCutoff);
        mBaseColorTexture.serialize(os);
        mMetallicRoughnessTexture.serialize(os);
        mNormalTexture.serialize(os);
        vcl::serialize(os, mNormalScale);
        mOcclusionTexture.serialize(os);
        vcl::serialize(os, mOcclusionStrength);
        mEmissiveTexture.serialize(os);
        vcl::serialize(os, mDoubleSided);
    }

    void deserialize(std::istream& is)
    {
        vcl::deserialize(is, mName);
        mBaseColor.deserialize(is);
        vcl::deserialize(is, mMetallic, mRoughness);
        mEmissiveColor.deserialize(is);
        vcl::deserialize(is, mAlphaMode, mAlphaCutoff);
        mBaseColorTexture.deserialize(is);
        mMetallicRoughnessTexture.deserialize(is);
        mNormalTexture.deserialize(is);
        vcl::deserialize(is, mNormalScale);
        mOcclusionTexture.deserialize(is);
        vcl::deserialize(is, mOcclusionStrength);
        mEmissiveTexture.deserialize(is);
        vcl::deserialize(is, mDoubleSided);
    }

    bool operator==(const Material& other) const
    {
        return mName == other.mName && mBaseColor == other.mBaseColor &&
               mMetallic == other.mMetallic && mRoughness == other.mRoughness &&
               mEmissiveColor == other.mEmissiveColor &&
               mAlphaMode == other.mAlphaMode &&
               mAlphaCutoff == other.mAlphaCutoff &&
               mBaseColorTexture.path() == other.mBaseColorTexture.path() &&
               mMetallicRoughnessTexture.path() ==
                   other.mMetallicRoughnessTexture.path() &&
               mNormalTexture.path() == other.mNormalTexture.path() &&
               mNormalScale == other.mNormalScale &&
               mOcclusionTexture.path() == other.mOcclusionTexture.path() &&
               mOcclusionStrength == other.mOcclusionStrength &&
               mEmissiveTexture.path() == other.mEmissiveTexture.path() &&
               mDoubleSided == other.mDoubleSided;
    };

    static Image::ColorSpace textureTypeToColorSpace(TextureType type)
    {
        switch (type) {
        case TextureType::BASE_COLOR:
        case TextureType::EMISSIVE:
            return Image::ColorSpace::SRGB;
        case TextureType::METALLIC_ROUGHNESS:
        case TextureType::NORMAL:
        case TextureType::OCCLUSION:
        default:
            return Image::ColorSpace::LINEAR;
        }
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
