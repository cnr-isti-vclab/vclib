// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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

void DrawableBox3::draw(const DrawObjectSettings& settings)
{
    if (mVisible) {
        mBoxLines.draw(settings.viewId);
    }
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
