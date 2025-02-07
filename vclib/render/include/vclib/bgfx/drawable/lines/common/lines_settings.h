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

#ifndef VCL_BGFX_DRAWABLE_LINES_LINES_SETTINGS_H
#define VCL_BGFX_DRAWABLE_LINES_LINES_SETTINGS_H

#include "lines_utils.h"

#include <vclib/bgfx/uniform.h>

#include <bgfx/bgfx.h>

#include <cassert>

namespace vcl::lines {

enum class Caps {
    BUTT_CAP     = 0x00000000, // No extra space on caps
    SQUARE_CAP   = 0x00000001, // Extra space with square shape
    ROUND_CAP    = 0x00000002, // Extra space with round shape
    TRIANGLE_CAP = 0x00000003  // Extra space with triangle
};

enum class Joins {
    ROUND_JOIN = 0x00000000, // Join with round shape
    BEVEL_JOIN = 0x00000001, // Join with square shape
    MITER_JOIN = 0x00000002, // Join with a miter
};

enum class ColorToUse {
    PER_VERTEX_COLOR = 0x00000000, // Select color form vertex color
    PER_EDGE_COLOR   = 0x00000001, // Select color from edge buffer color
    GENERAL_COLOR    = 0x00000002  // Use general color in uniform data
};

class LinesSettings
{
private:
    uint8_t  mThickness = 5;
    uint8_t  mAntialias = 0;
    uint8_t  mBorder = 0;
    uint32_t mBorderColor = LinesVertex::COLOR(0, 0, 0, 1);
    uint32_t mGeneralColor = LinesVertex::COLOR(0, 0, 0, 1);
    uint8_t  mMiterLimit = mThickness * 2;

    Caps       mLeftCap = Caps::ROUND_CAP;
    Caps       mRigthCap = Caps::ROUND_CAP;
    Joins      mJoin = Joins::ROUND_JOIN;
    ColorToUse mColorToUse = ColorToUse::PER_VERTEX_COLOR;

    Uniform mDataUH = Uniform("u_data", bgfx::UniformType::Vec4);

public:
    LinesSettings() = default;

    Joins getJoin() const { return mJoin; }

    void setThickness(uint8_t thickness) { mThickness = thickness; }

    void setAntialias(uint8_t antialias) { mAntialias = antialias; }

    void setBorder(uint8_t border) { mBorder = border; }

    void setBorderColor(uint32_t borderColor) { mBorderColor = borderColor; }

    void setGeneralColor(uint32_t generalColor)
    {
        mGeneralColor = generalColor;
    }

    void setMiterLimit(uint8_t miterLimit)
    {
        if (miterLimit < mThickness)
            assert(
                (void("Miter limit must be greatest then thickness * 2"),
                 false));
        mMiterLimit = miterLimit;
    }

    void setLeftCap(Caps cap) { mLeftCap = cap; }

    void setRigthCap(Caps cap) { mRigthCap = cap; }

    void setJoin(Joins join) { mJoin = join; }

    void setColorToUse(ColorToUse colorToUse) { mColorToUse = colorToUse; }

public:
    void bindUniformLines() const
    {
        uint32_t thickness_antialias_border_caps_color =
            (0 | mThickness << 24 | mAntialias << 16 | mBorder << 8 |
             static_cast<uint8_t>(mLeftCap) << 4 |
             static_cast<uint8_t>(mRigthCap) << 2 |
             static_cast<uint8_t>(mColorToUse));
        uint32_t data[] = {
            mGeneralColor,
            thickness_antialias_border_caps_color,
            mBorderColor,
            0};
        mDataUH.bind(data);
    }

    void bindUniformPolylines() const
    {
        uint32_t thickness_antialias_border_miterlimit =
            (0 | mThickness << 24 | mAntialias << 16 | mBorder << 8 |
             mMiterLimit);

        uint32_t caps_join_color =
            (0 | static_cast<uint8_t>(mLeftCap) << 6 |
             static_cast<uint8_t>(mRigthCap) << 4 |
             static_cast<uint8_t>(mJoin) << 2 |
             static_cast<uint8_t>(mColorToUse));

        uint32_t data[] = {
            mGeneralColor,
            thickness_antialias_border_miterlimit,
            mBorderColor,
            caps_join_color};
        mDataUH.bind(data);
    }
};

} // namespace vcl::lines

#endif
