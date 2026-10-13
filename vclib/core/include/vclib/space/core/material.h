// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_SPACE_CORE_MATERIAL_H
#define VCL_SPACE_CORE_MATERIAL_H

#include "color.h"
#include "image.h"
#include "texture_descriptor.h"

#include <vclib/base.h>

namespace vcl {

/**
 * @brief Represents a Physically-Based Rendering (PBR) material.
 *
 * This class encapsulates all the properties required to describe a surface
 * for rendering, following the PBR metallic-roughness workflow. It includes
 * base color, metallic and roughness factors, emissive color, and textures.
 * It also handles transparency modes and other rendering-related attributes.
 *
 * @ingroup space_core
 */
class Material
{
public:
    /**
     * @brief Defines the alpha rendering mode of the material.
     */
    enum class AlphaMode {
        ALPHA_OPAQUE, ///< The material is fully opaque. Alpha values are
                      ///< ignored.
        ALPHA_MASK,   ///< The material is rendered as either fully opaque or
                      ///< fully transparent, based on the alphaCutoff value.
        ALPHA_BLEND ///< The material is rendered with alpha blending, allowing
                    ///< for smooth transparency.
    };

    /**
     * @brief Defines the types of textures used in the PBR material model.
     */
    enum class TextureType {
        BASE_COLOR =
            0, ///< The base color (albedo) texture. Stored in sRGB color space.
        METALLIC_ROUGHNESS, ///< Metallic (B channel) and roughness (G channel)
                            ///< texture. Stored in linear color space.
        NORMAL, ///< The tangent-space normal map. Stored in linear color space.
        OCCLUSION, ///< The ambient occlusion map (R channel). Stored in linear
                   ///< color space.
        EMISSIVE,  ///< The emissive color texture. Stored in sRGB color space.
        COUNT      ///< Utility value to get the number of texture types.
    };

    inline static const std::
        array<std::string, toUnderlying(TextureType::COUNT)>
            TEXTURE_TYPE_NAMES = {
                "baseColorTex",
                "metallicRoughnessTex",
                "normalTex",
                "occlusionTex",
                "emissiveTex"};

private:
    inline static const uint N_TEXTURE_TYPE =
        static_cast<uint>(TextureType::COUNT);

    std::string mName;

    // essential PBR properties
    Color mBaseColor = Color::White;

    float mMetallic = 0.0f;

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
    /**
     * @brief Default constructor.
     *
     * Creates a default material.
     * The default material has:
     * - Name: empty string
     * - Base Color: White
     * - Metallic: 0.0
     * - Roughness: 1.0
     * - Emissive Color: Black
     * - Alpha Mode: ALPHA_OPAQUE
     * - Alpha Cutoff: 0.5
     * - Double Sided: false
     * - Normal Scale: 1.0
     * - Occlusion Strength: 1.0
     * - No textures assigned.
     */
    Material() {}

    /**
     * @brief Gets the name of the material.
     * @return A reference to the material's name.
     */
    decltype(auto) name(this auto&& self) { return (self.mName); }

    /**
     * @brief Gets the base color of the material.
     * This is used as a tint if a base color texture is present.
     * @return A reference to the base color.
     */
    decltype(auto) baseColor(this auto&& self) { return (self.mBaseColor); }

    /**
     * @brief Gets the metallic factor of the material.
     * @return A reference to the metallic factor, in the range [0.0, 1.0].
     */
    decltype(auto) metallic(this auto&& self) { return (self.mMetallic); }

    /**
     * @brief Gets the roughness factor of the material.
     * @return A reference to the roughness factor, in the range [0.0, 1.0].
     */
    decltype(auto) roughness(this auto&& self) { return (self.mRoughness); }

    /**
     * @brief Gets the emissive color of the material.
     * This is the color emitted by the material, independent of lighting.
     * @return A reference to the emissive color.
     */
    decltype(auto) emissiveColor(this auto&& self) { return (self.mEmissiveColor); }

    /**
     * @brief Gets the alpha rendering mode.
     * @return A reference to the current alpha mode (Opaque, Mask, or Blend).
     */
    decltype(auto) alphaMode(this auto&& self) { return (self.mAlphaMode); }

    /**
     * @brief Gets the alpha cutoff value.
     * This value is used only when the alpha mode is `ALPHA_MASK`.
     * @return A reference to the alpha cutoff value, in the range [0.0, 1.0].
     */
    decltype(auto) alphaCutoff(this auto&& self) { return (self.mAlphaCutoff); }

    /**
     * @brief Checks if the material is double-sided.
     * If true, both front and back faces of polygons with this material should
     * be rendered.
     * @return A reference to the double-sided flag.
     */
    decltype(auto) doubleSided(this auto&& self) { return (self.mDoubleSided); }

    /**
     * @brief Gets the scalar multiplier for the normal map.
     * @return A reference to the normal scale factor.
     */
    decltype(auto) normalScale(this auto&& self) { return (self.mNormalScale); }

    /**
     * @brief Gets the strength of the ambient occlusion effect.
     * @return A reference to the occlusion strength, in the range [0.0, 1.0].
     */
    decltype(auto) occlusionStrength(this auto&& self) { return (self.mOcclusionStrength); }

    /**
     * @brief Gets the texture descriptor for the base color texture.
     * @return A reference to the base color texture descriptor.
     */
    decltype(auto) baseColorTextureDescriptor(this auto&& self)
    {
        using enum TextureType;
        return (self.mTextureDescriptors[toUnderlying(BASE_COLOR)]);
    }

    /**
     * @brief Gets the texture descriptor for a given texture type index.
     * @param[in] type: The integer index of the texture type. Must be less than
     * `TextureType::COUNT`.
     * @return A reference to the corresponding texture descriptor.
     */
    decltype(auto) textureDescriptor(this auto&& self, uint type)
    {
        assert(type < N_TEXTURE_TYPE);
        return (self.mTextureDescriptors[type]);
    }

    /**
     * @brief Gets the texture descriptor for a given texture type.
     * @param[in] type: The TextureType enum value.
     * @return A reference to the corresponding texture descriptor.
     */
    decltype(auto) textureDescriptor(this auto&& self, TextureType type)
    {
        return self.textureDescriptor(toUnderlying(type));
    }

    /**
     * @brief Serializes the material's data to an output stream.
     * @param[in/out] os: The output stream to write to.
     */
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

    /**
     * @brief Deserializes the material's data from an input stream.
     * @param[in/out] is: The input stream to read from.
     */
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

    /**
     * @brief Defaulted equality comparison operator.
     * @param[in] other: The material to compare against.
     * @return True if all members of this material are equal to the other,
     * false otherwise.
     */
    bool operator==(const Material& other) const = default;

    /**
     * @brief Determines the appropriate color space for a given texture type.
     *
     * Base color and emissive textures contain color information and should be
     * treated as sRGB. Other textures (metallic-roughness, normal, occlusion)
     * contain data and should be treated as linear.
     *
     * @param[in] type: The texture type.
     * @return Image::ColorSpace::SRGB or Image::ColorSpace::LINEAR.
     */
    static Image::ColorSpace textureTypeToColorSpace(TextureType type)
    {
        switch (type) {
        case TextureType::BASE_COLOR:
        case TextureType::EMISSIVE: return Image::ColorSpace::SRGB;
        case TextureType::METALLIC_ROUGHNESS:
        case TextureType::NORMAL:
        case TextureType::OCCLUSION:
        default: return Image::ColorSpace::LINEAR;
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
