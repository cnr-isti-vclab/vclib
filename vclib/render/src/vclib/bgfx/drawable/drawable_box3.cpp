/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#include <vclib/bgfx/drawable/drawable_box3.h>

namespace vcl {

DrawableBox3::DrawableBox3(const DrawableBox3& other) :
        DrawableObject(other), mBox(other.mBox)
{
    mBoxLines.generalColor() = other.mBoxLines.generalColor();
    mBoxLines.thickness()    = other.mBoxLines.thickness();
    updateLines();
}

DrawableBox3& DrawableBox3::operator=(DrawableBox3 other)
{
    swap(other);
    return *this;
}

void DrawableBox3::swap(DrawableBox3& other)
{
    using std::swap;
    DrawableObject::swap(other);
    swap(mBox, other.mBox);
    swap(mBoxLines, other.mBoxLines);
}

void DrawableBox3::setThickness(float thickness)
{
    mBoxLines.thickness() = thickness;
}

void DrawableBox3::setColor(const Color& color)
{
    mBoxLines.generalColor() = color;
}

void DrawableBox3::draw(const DrawObjectSettings& settings) const
{
    if (mVisible) {
        mBoxLines.draw(settings.viewId);
    }
}

std::shared_ptr<DrawableObject> DrawableBox3::clone() const&
{
    return std::make_shared<DrawableBox3>(*this);
}

std::shared_ptr<DrawableObject> DrawableBox3::clone() &&
{
    return std::make_shared<DrawableBox3>(std::move(*this));
}

void DrawableBox3::updateLines()
{
    // clang-format off
    std::vector<float> points = {
        mBox.min().x(), mBox.min().y(), mBox.min().z(),
        mBox.max().x(), mBox.min().y(), mBox.min().z(),
        mBox.min().x(), mBox.max().y(), mBox.min().z(),
        mBox.max().x(), mBox.max().y(), mBox.min().z(),
        mBox.min().x(), mBox.min().y(), mBox.max().z(),
        mBox.max().x(), mBox.min().y(), mBox.max().z(),
        mBox.min().x(), mBox.max().y(), mBox.max().z(),
        mBox.max().x(), mBox.max().y(), mBox.max().z()
    };

    std::vector<uint> lineIndices = {
        0, 1,
        1, 3,
        3, 2,
        2, 0,
        4, 5,
        5, 7,
        7, 6,
        6, 4,
        0, 4,
        1, 5,
        2, 6,
        3, 7
    };
    // clang-format on
    mBoxLines.setPoints(points, lineIndices, {}, {}, {});
}

} // namespace vcl
