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

#include <vclib/serialization.h>

namespace vcl {

/**
 * @brief Material class represents a material with properties needed for
 * rendering (PBR).
 */
// TODO - Add support for textures, normal maps, and other PBR properties.
class Material
{
    vcl::Color mBaseColor;

    float mMetallic;
    float mRoughness;

public:
    Material() :
            mBaseColor(vcl::Color(1.0f, 1.0f, 1.0f)), mMetallic(0.0f),
            mRoughness(0.5f)
    {
    }

    Material(
        const vcl::Color& baseColor,
        float             metallic  = 0.0f,
        float             roughness = 0.5f) :
            mBaseColor(baseColor), mMetallic(metallic), mRoughness(roughness)
    {
    }

    const vcl::Color& baseColor() const { return mBaseColor; }
    vcl::Color& baseColor() { return mBaseColor; }

    float metallic() const { return mMetallic; }
    float& metallic() { return mMetallic; }

    float roughness() const { return mRoughness; }
    float& roughness() { return mRoughness; }

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

} // namespace vcl

#endif // VCL_SPACE_CORE_MATERIAL_H
