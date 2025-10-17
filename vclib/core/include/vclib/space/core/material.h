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

private:
    // essential PBR properties
    vcl::Color mBaseColor = vcl::Color(1.0f, 1.0f, 1.0f);

    float mMetallic = 1.0f;
    float mRoughness = 1.0f;

    // optional PBR properties
    vcl::Color mEmissiveColor = vcl::Color(0.0f, 0.0f, 0.0f);

    bool mDoubleSided = false;

    AlphaMode mAlphaMode = AlphaMode::ALPHA_OPAQUE;

    float mAlphaCutoff = 0.5f;         // only used when mAlphaMode is MASK

public:
    Material() :
            mBaseColor(), 
            mMetallic(),
            mRoughness(),
            mEmissiveColor(), 
            mDoubleSided(),
            mAlphaMode(),
            mAlphaCutoff()
    {
    }

    Material(
        const vcl::Color& baseColor,
        float             metallic,
        float             roughness,
        const vcl::Color& emissiveColor,
        bool              doubleSided,
        AlphaMode         alphaMode,
        float             alphaCutoff
    ) :
            mBaseColor(baseColor), 
            mMetallic(metallic), 
            mRoughness(roughness), 
            mEmissiveColor(emissiveColor),
            mDoubleSided(doubleSided),
            mAlphaMode(alphaMode),
            mAlphaCutoff(alphaCutoff)
    {
    }

    const vcl::Color& baseColor() const { return mBaseColor; }
    vcl::Color& baseColor() { return mBaseColor; }

    float metallic() const { return mMetallic; }
    float& metallic() { return mMetallic; }

    float roughness() const { return mRoughness; }
    float& roughness() { return mRoughness; }

    const vcl::Color& emissiveColor() const { return mEmissiveColor; }
    vcl::Color& emissiveColor() { return mEmissiveColor; }

    bool doubleSided() const { return mDoubleSided; }
    bool& doubleSided() { return mDoubleSided; }

    AlphaMode alphaMode() const { return mAlphaMode; }
    AlphaMode& alphaMode() { return mAlphaMode; }

    float alphaCutoff() const { return mAlphaCutoff; }
    float& alphaCutoff() { return mAlphaCutoff; }

    void serialize(std::ostream& os) const
    {
        mBaseColor.serialize(os);
        vcl::serialize(os, mMetallic, mRoughness);
    }

    void deserialize(std::istream& is)
    {
        mBaseColor.deserialize(is);
        vcl::deserialize(is, mMetallic, mRoughness);
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
