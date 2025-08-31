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

#include <vclib/bgfx/primitives/lines/primitive_lines.h>
#include <vclib/bgfx/primitives/lines/cpu_generated_lines.h>
#include <vclib/bgfx/uniform.h>

#include <vclib/base.h>
#include <vclib/space/core.h>

#include <bgfx/bgfx.h>

namespace vcl {

// TODO: add shading per-line (flat, per-line normal) (?)
class Lines
{
public:
    enum class ColorToUse {
        PER_VERTEX, // Select color form vertex color
        PER_EDGE,   // Select color from edge buffer color
        GENERAL     // Use general color in uniform data
    };

    enum class ImplementationType {
        PRIMITIVE     = 0   , // Use bgfx primitive lines (not implemented)
        CPU_GENERATED = 1   , // Buffers pre-generated in CPU

        // TODO: uncomment when they will be implemented
        // GPU_GENERATED,     // Buffers pre-generated in GPU with computes
        // CPU_INSTANCING,    // Using Instancing with buffers generated in CPU
        // GPU_INSTANCING,    // Using Instancing with buffer generated in GPU
        //                    // computes
        // TEXTURE_INSTANCING, // Using Instancing with textures generated in
                               // GPU computes

        COUNT
    };

private:
    float mThickness = 5.0f;
    // TODO: shading should become a enum with options: PER_VERTEX, PER_EDGE,
    // NONE
    // PER_EDGE means that we use a buffer of normals for each edge
    bool mShadingPerVertex = false;

    BitSet8    mColorCapability = {false, false, true}; // general color only
    ColorToUse mColorToUse      = ColorToUse::GENERAL;
    Color      mGeneralColor    = Color::ColorABGR::LightGray;
    ImplementationType mType    = ImplementationType::COUNT;

    Uniform mSettingUH = Uniform("u_settings", bgfx::UniformType::Vec4);
    std::variant<detail::PrimitiveLines, detail::CPUGeneratedLines> mLinesImplementation;

public:
    Lines() { setImplementationType(ImplementationType::PRIMITIVE); };

    Lines(
        const std::vector<float>& vertCoords,
        const std::vector<float>& vertNormals,
        const std::vector<uint>&  vertColors,
        const std::vector<uint>&  lineColors,
        float                     thickness        = 5.0f,
        bool                      shadingPerVertex = false,
        ColorToUse                colorToUse       = ColorToUse::GENERAL,
        ImplementationType        type = ImplementationType::PRIMITIVE) :
            mThickness(thickness), mShadingPerVertex(shadingPerVertex)
    {
        setImplementationType(type);
        setPoints(vertCoords, vertNormals, vertColors, lineColors);
        setColorToUse(colorToUse);
    }

    Lines(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  lineIndices,
        const std::vector<float>& vertNormals,
        const std::vector<uint>&  vertColors,
        const std::vector<uint>&  lineColors,
        float                     thickness        = 5.0f,
        bool                      shadingPerVertex = false,
        ColorToUse                colorToUse       = ColorToUse::GENERAL,
        ImplementationType        type = ImplementationType::PRIMITIVE) :
            mThickness(thickness), mShadingPerVertex(shadingPerVertex)
    {
        setImplementationType(type);
        setPoints(vertCoords, lineIndices, vertNormals, vertColors, lineColors);
        setColorToUse(colorToUse);
    }

    void setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<float>& vertNormals,
        const std::vector<uint>&  vertColors,
        const std::vector<uint>&  lineColors)
    {
        using enum ImplementationType;

        switch(mType){
            case PRIMITIVE: // always supported
                std::get<detail::PrimitiveLines>(mLinesImplementation).setPoints(
                    vertCoords, vertNormals, vertColors, lineColors
                );
                break;

            case CPU_GENERATED: // always supported
                std::get<detail::CPUGeneratedLines>(mLinesImplementation).setPoints(
                    vertCoords, vertNormals, vertColors, lineColors
                );
                break;
            default:
                break; 
        }
        updateColorCapability(vertColors, lineColors);
    }

    void setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  lineIndices,
        const std::vector<float>& vertNormals,
        const std::vector<uint>&  vertColors,
        const std::vector<uint>&  lineColors)
    {
        using enum ImplementationType;

        switch(mType){
            case PRIMITIVE: // always supported
                std::get<detail::PrimitiveLines>(mLinesImplementation).setPoints(
                    vertCoords, lineIndices, vertNormals, vertColors, lineColors
                );
                break;

            case CPU_GENERATED: // always supported
                std::get<detail::CPUGeneratedLines>(mLinesImplementation).setPoints(
                    vertCoords, lineIndices, vertNormals, vertColors, lineColors
                );
                break;
            default:
                break; 
        }
        updateColorCapability(vertColors, lineColors);
    }

    float thickness() const { return mThickness; }

    float& thickness() { return mThickness; }

    ColorToUse colorToUse() const { return mColorToUse; }

    Color generalColor() const { return mGeneralColor; }

    Color& generalColor() { return mGeneralColor; }

    bool shadingPerVertex() const { return mShadingPerVertex; }

    bool& shadingPerVertex() { return mShadingPerVertex; }

    ImplementationType type() const { return mType; }

    void setColorToUse(ColorToUse color)
    {
        if (!mColorCapability[toUnderlying(color)]) {
            throw std::runtime_error(
                "Lines::setColorToUse(): color option not supported by the "
                "current buffers.");
        }
        else {
            mColorToUse = color;
        }
    }

    bool setImplementationType(ImplementationType type)
    {
        using enum ImplementationType;
        if (mType == type)
            return false; // no change

        // TODO: check whether caps allow the new implementation type
        // then set the implementation and the type
        switch (type) {
            case PRIMITIVE: // always supported
                mLinesImplementation = detail::PrimitiveLines();
                mType                = type;
                return true;

            case CPU_GENERATED: // always supported
                mLinesImplementation = detail::CPUGeneratedLines();
                mType                = type;
                return true;

            default: return false; // not supported
        }
    }

    void draw(uint viewId) const
    {
        bindSettingsUniform();
        if (mType == ImplementationType::PRIMITIVE)
            std::get<detail::PrimitiveLines>(mLinesImplementation).draw(viewId);

        if (mType == ImplementationType::CPU_GENERATED)
            std::get<detail::CPUGeneratedLines>(mLinesImplementation).draw(viewId);
    }

    void swap(Lines& other)
    {
        using std::swap;

        swap(mThickness, other.mThickness);
        swap(mColorToUse, other.mColorToUse);
        swap(mSettingUH, other.mSettingUH);
        swap(mGeneralColor, other.mGeneralColor);
        swap(mShadingPerVertex, other.mShadingPerVertex);
        swap(mColorCapability, other.mColorCapability);

        swap(mType, other.mType);
        swap(mLinesImplementation, other.mLinesImplementation);
    }

    friend void swap(Lines& a, Lines& b) { a.swap(b); }

private:
    void updateColorCapability(
        const std::vector<uint>& vertColors,
        const std::vector<uint>& lineColors)
    {
        using enum ColorToUse;

        mColorCapability[toUnderlying(PER_VERTEX)] = !vertColors.empty();
        mColorCapability[toUnderlying(PER_EDGE)]   = !lineColors.empty();
    }

    void bindSettingsUniform() const
    {
        float data[] = {
            mThickness,
            static_cast<float>(mColorToUse),
            std::bit_cast<float>(mGeneralColor.abgr()),
            static_cast<float>(mShadingPerVertex)};
        mSettingUH.bind(data);
    }
};

} // namespace vcl

#endif // VCL_BGFX_PRIMITIVES_LINES_H
