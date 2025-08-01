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

#ifndef VCL_BGFX_PRIMITIVES_LINES_H
#define VCL_BGFX_PRIMITIVES_LINES_H

#include <vclib/bgfx/primitives/lines/cpu_generated_lines.h>
#include <vclib/bgfx/uniform.h>

#include <vclib/base.h>

#include <bgfx/bgfx.h>

namespace vcl {

class Lines
{
public:
    enum class ColorToUse {
        PER_VERTEX, // Select color form vertex color
        PER_EDGE,   // Select color from edge buffer color
        GENERAL     // Use general color in uniform data
    };

    enum class ImplementationType {
        CPU_GENERATED = 0, // Buffers pre-generated in CPU

        // TODO: uncomment when they will be implemented
        // GPU_GENERATED,     // Buffers pre-generated in GPU with computes
        // CPU_INSTANCING,    // Using Instancing with buffers generated in CPU
        // GPU_INSTANCING,    // Using Instancing with buffer generated in GPU
        //                    // computes
        // TEXTURE_INSTANCING, // Using Instancing with textures generated in GPU
        // // computes

        COUNT
    };

private:
    uint8_t             mThickness    = 5;
    ColorToUse          mColorToUse   = ColorToUse::PER_VERTEX;
    Color               mGeneralColor = Color::ColorABGR::LightGray;
    ImplementationType  mType         = ImplementationType::CPU_GENERATED;
    // TODO: add setting that allows to set shading: per vertex (using normal)
    // or solid

    Uniform mSettingUH = Uniform("u_settings", bgfx::UniformType::Vec4);
    detail::CPUGeneratedLines mLinesImplementation;

public:
    Lines() = default;

    Lines(
        const std::vector<float>& vertCoords,
        const std::vector<float>& vertNormals,
        const std::vector<uint>&  vertColors,
        const std::vector<uint>&  lineColors,
        uint8_t                   thickness = 5,
        ColorToUse                colorToUse = ColorToUse::PER_VERTEX,
        ImplementationType        type = ImplementationType::CPU_GENERATED) :
            mThickness(thickness), mColorToUse(colorToUse)
    {
        setImplementationType(type);
        mLinesImplementation.setPoints(
            vertCoords, vertNormals, vertColors, lineColors);
    }

    Lines(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  lineIndices,
        const std::vector<float>& vertNormals,
        const std::vector<uint>&  vertColors,
        const std::vector<uint>&  lineColors,
        uint8_t                   thickness = 5,
        ColorToUse                colorToUse = ColorToUse::PER_VERTEX,
        ImplementationType        type = ImplementationType::CPU_GENERATED) :
            mThickness(thickness), mColorToUse(colorToUse)
    {
        setImplementationType(type);
        mLinesImplementation.setPoints(
            vertCoords, lineIndices, vertNormals, vertColors, lineColors);
    }

    void setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<float>& vertNormals,
        const std::vector<uint>&  vertColors,
        const std::vector<uint>&  lineColors)
    {
        mLinesImplementation.setPoints(
            vertCoords, vertNormals, vertColors, lineColors);
    }

    void setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<uint>& lineIndices,
        const std::vector<float>& vertNormals,
        const std::vector<uint>&  vertColors,
        const std::vector<uint>&  lineColors)
    {
        mLinesImplementation.setPoints(
            vertCoords, lineIndices, vertNormals, vertColors, lineColors);
    }

    uint8_t thickness() const { return mThickness; }

    uint8_t& thickness() { return mThickness; }

    ColorToUse colorToUse() const { return mColorToUse; }

    ColorToUse& colorToUse() { return mColorToUse; }

    Color generalColor() const { return mGeneralColor; }

    Color& generalColor() { return mGeneralColor; }

    ImplementationType type() const { return mType; }

    bool setImplementationType(ImplementationType type)
    {
        using enum ImplementationType;
        if (mType == type)
            return false; // no change

        // TODO: check whether caps allow the new implementation type
        // then set the implementation and the type
        switch(type){
            case CPU_GENERATED: // always supported
                mLinesImplementation = detail::CPUGeneratedLines();
                mType                = type;
                return true;
            default:
                return false; // not supported
        }
    }

    void draw(uint viewId) const
    {
        bindSettingsUniform();
        if (mType == ImplementationType::CPU_GENERATED)
            mLinesImplementation.draw(viewId);
    }

    void swap(Lines& other)
    {
        using std::swap;

        swap(mThickness, other.mThickness);
        swap(mColorToUse, other.mColorToUse);
        swap(mSettingUH, other.mSettingUH);

        swap(mType, other.mType);
        swap(mLinesImplementation, other.mLinesImplementation);
    }

    friend void swap(Lines& a, Lines& b) { a.swap(b); }

private:
    void bindSettingsUniform() const
    {
        float data[] = {
            static_cast<float>(mThickness),
            static_cast<float>(mColorToUse),
            std::bit_cast<float>(mGeneralColor.abgr()),
            0};
        mSettingUH.bind(data);
    }
};

} // namespace vcl

#endif // VCL_BGFX_PRIMITIVES_LINES_H
