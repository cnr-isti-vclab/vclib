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

#ifndef VCL_BGFX_DRAWABLE_POLYLINES_POLYLINE_SETTINGS_H
#define VCL_BGFX_DRAWABLE_POLYLINES_POLYLINE_SETTINGS_H

#include <vclib/bgfx/drawable/lines/line_settings.h>

namespace vcl {

enum class PolyLineJoint {
    ROUND_JOINT = 0x00000000, // Joint with round shape
    BEVEL_JOINT = 0x00000001, // Joint with square shape
    MITER_JOINT = 0x00000002, // Joint with a miter
};

class PolylineSettings : public LineSettings
{
private:
    uint8_t  mMiterLimit = mThickness * 2;

    PolyLineJoint  mJoint      = PolyLineJoint::ROUND_JOINT;

public:
    PolylineSettings() = default;

    PolyLineJoint getJoint() const { return mJoint; }

    void setMiterLimit(uint8_t miterLimit)
    {
        if (miterLimit < mThickness)
            assert(
                (void("Miter limit must be greatest then thickness * 2"),
                 false));
        mMiterLimit = miterLimit;
    }

    void setJoint(PolyLineJoint joint) { mJoint = joint; }

    void bindUniform() const
    {
        uint32_t thickness_antialias_border_miterlimit =
            (0 | mThickness << 24 | mAntialias << 16 | mBorder << 8 |
             mMiterLimit);

        uint32_t caps_joint_color =
            (0 | static_cast<uint8_t>(mLeftCap) << 6 |
             static_cast<uint8_t>(mRigthCap) << 4 |
             static_cast<uint8_t>(mJoint) << 2 |
             static_cast<uint8_t>(mColorToUse));

        uint32_t data[] = {
            mGeneralColor,
            thickness_antialias_border_miterlimit,
            mBorderColor,
            caps_joint_color};
        mDataUH.bind(data);
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_POLYLINES_POLYLINE_SETTINGS_H
