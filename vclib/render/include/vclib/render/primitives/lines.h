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

#ifndef VCL_RENDER_PRIMITIVES_LINES_H
#define VCL_RENDER_PRIMITIVES_LINES_H

#include <vclib/types.h>

#include <bgfx/bgfx.h>

#include <vclib/bgfx/uniform.h>

#include <vclib/render/primitives/lines/cpu_generated_lines.h>

namespace vcl {

class Lines
{
public:
    enum class ColorToUse {
        PER_VERTEX_COLOR, // Select color form vertex color
        PER_EDGE_COLOR,   // Select color from edge buffer color
        GENERAL_COLOR     // Use general color in uniform data
    };

    enum class ImplementationTypes {
        CPU_GENERATED,     // Buffers pre-generated in CPU
        GPU_GENERATED,     // Buffers pre-generated in GPU with computes
        CPU_INSTANCING,    // Using Instancing with buffers generated in CPU
        GPU_INSTANCING,    // Using Instancing with buffer generated in GPU
                           // computes
        TEXTURE_INSTANCING // Using Instancing with textures generated in GPU
        // computes
    };

private:
    uint8_t    mThickness  = 5;
    ColorToUse mColorToUse = ColorToUse::PER_VERTEX_COLOR;
    Uniform    mSettingUH  = Uniform("u_settings", bgfx::UniformType::Vec4);
    ImplementationTypes       mType = ImplementationTypes::CPU_GENERATED;
    detail::CPUGeneratedLines mLinesImplementation;

public:
    Lines() = default;

    Lines(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  vertColors,
        const std::vector<float>& vertNormals,
        const std::vector<uint>&  lineColors)
    {
        mLinesImplementation.setPoints(
            vertCoords, vertColors, vertNormals, lineColors);
    }

    void draw(uint viewId) const
    {
        bindSettingsUniform();
        if (mType == ImplementationTypes::CPU_GENERATED)
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

    void setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  vertColors,
        const std::vector<float>& vertNormals,
        const std::vector<uint>&  lineColors)
    {
        mLinesImplementation.setPoints(
            vertCoords, vertColors, vertNormals, lineColors);
    }

    uint8_t getThickness() const { return mThickness; }

    ColorToUse getColorToUse() const { return mColorToUse; }

    void setThickness(uint8_t thickness) { mThickness = thickness; }

    void setColorToUse(ColorToUse colorToUse) { mColorToUse = colorToUse; }

private:
    void bindSettingsUniform() const
    {
        float data[] = {
            static_cast<float>(mThickness),
            static_cast<float>(mColorToUse),
            0,
            0};
        mSettingUH.bind(data);
    }
};

} // namespace vcl

#endif // VCL_RENDER_PRIMITIVES_LINES_H
