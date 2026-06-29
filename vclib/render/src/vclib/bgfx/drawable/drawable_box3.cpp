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
    mBoxLines.setWidth(other.mBoxLines.width());
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
    mBoxLines.setWidth(thickness);
}

void DrawableBox3::setColor(const Color& color)
{
    mBoxLines.setGeneralColor(color);
}

void DrawableBox3::draw(const DrawObjectSettings& settings)
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
    std::vector<Point3f> points(8);
    for (uint i = 0; i < 8; ++i) {
        points[i] = boxVertex(mBox, i);
    }

    // clang-format off
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
    mBoxLines.setVertices(points);
    mBoxLines.setIndices(lineIndices);
}

} // namespace vcl
