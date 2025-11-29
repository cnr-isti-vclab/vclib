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
#include "texture_descriptor.h"

#include <vclib/base.h>

namespace vcl {

/**
 * @brief Material class represents a material with properties needed for
 * rendering (PBR).
 */
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
    inline static const uint N_TEXTURE_TYPE =
        static_cast<uint>(TextureType::COUNT);

    std::string mName;

    // essential PBR properties
    Color mBaseColor = Color::White;

    float mMetallic  = 1.0f;

    float mRoughness = 1.0f;

    // optional PBR properties
    Color mEmissiveColor = Color::Black;

    AlphaMode mAlphaMode = AlphaMode::ALPHA_OPAQUE;

    float mAlphaCutoff = 0.5f; // only used when mAlphaMode is MASK

    float mNormalScale = 1.0f;

    float mOcclusionStrength = 1.0f;

    std::array<TextureDescriptor, N_TEXTURE_TYPE> mTextureDescriptors;
    
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

    const TextureDescriptor& baseColorTextureDescriptor() const
    {
        using enum TextureType;
        return mTextureDescriptors[toUnderlying(BASE_COLOR)];
    }

    TextureDescriptor& baseColorTextureDescriptor()
    {
        using enum TextureType;
        return mTextureDescriptors[toUnderlying(BASE_COLOR)];
    }

    const TextureDescriptor& textureDescriptor(uint type) const
    {
        assert(type < N_TEXTURE_TYPE);
        return mTextureDescriptors[type];
    }

    const TextureDescriptor& textureDescriptor(TextureType type) const
    {
        return textureDescriptor(toUnderlying(type));
    }

    TextureDescriptor& textureDescriptor(uint type)
    {
        assert(type < N_TEXTURE_TYPE);
        return mTextureDescriptors[type];
    }

    TextureDescriptor& textureDescriptor(TextureType type)
    {
        return textureDescriptor(toUnderlying(type));
    }

    void serialize(std::ostream& os) const
    {
        vcl::serialize(os, mName);
        mBaseColor.serialize(os);
        vcl::serialize(os, mMetallic, mRoughness);
        mEmissiveColor.serialize(os);
        vcl::serialize(os, mAlphaMode, mAlphaCutoff);
        vcl::serialize(os, mNormalScale);
        vcl::serialize(os, mOcclusionStrength);
        vcl::serialize(os, mTextureDescriptors);
        vcl::serialize(os, mDoubleSided);
    }

    void deserialize(std::istream& is)
    {
        vcl::deserialize(is, mName);
        mBaseColor.deserialize(is);
        vcl::deserialize(is, mMetallic, mRoughness);
        mEmissiveColor.deserialize(is);
        vcl::deserialize(is, mAlphaMode, mAlphaCutoff);
        vcl::deserialize(is, mNormalScale);
        vcl::deserialize(is, mOcclusionStrength);
        vcl::deserialize(is, mTextureDescriptors);
        vcl::deserialize(is, mDoubleSided);
    }

    bool operator==(const Material& other) const = default;

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
