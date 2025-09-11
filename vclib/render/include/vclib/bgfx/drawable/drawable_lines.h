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

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_LINES_H
#define VCL_BGFX_DRAWABLE_DRAWABLE_LINES_H

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/primitives/lines.h>
#include <vclib/render/drawable/drawable_object.h>

#include <bgfx/bgfx.h>

namespace vcl {

class DrawableLines : public Lines, public vcl::DrawableObject
{
    bool mVisible        = true;
    bool mUseLineIndices = false; // true whether a vector of line indices has been
                                  // provided (even if empty)

    std::vector<float> mVertCoords;
    std::vector<uint>  mVertColors;
    std::vector<float> mVertNormals;
    std::vector<uint>  mLineColors;
    std::vector<uint>  mLineIndices;

public:
    DrawableLines() = default;

    DrawableLines(
        const std::vector<float>& vertCoords,
        const std::vector<float>& vertNormals = std::vector<float>(),
        const std::vector<uint>&  vertColors  = std::vector<uint>(),
        const std::vector<uint>&  lineColors  = std::vector<uint>())
    {
        setPoints(vertCoords, vertNormals, vertColors, lineColors);
    }

    DrawableLines(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  lineIndices,
        const std::vector<float>& vertNormals = std::vector<float>(),
        const std::vector<uint>&  vertColors  = std::vector<uint>(),
        const std::vector<uint>&  lineColors  = std::vector<uint>())
    {
        setPoints(
            vertCoords, lineIndices, vertNormals, vertColors, lineColors);
    }

    DrawableLines(const DrawableLines& other) :
            DrawableObject(other), mVertCoords(other.mVertCoords),
            Lines(
                other.mVertCoords,
                other.mLineIndices,
                other.mVertNormals,
                other.mVertColors,
                other.mLineColors,
                other.thickness(),
                other.shadingPerVertex(),
                other.colorToUse(),
                other.implementationType()),
            mVisible(other.mVisible), mUseLineIndices(other.mUseLineIndices),
            mVertColors(other.mVertColors), mVertNormals(other.mVertNormals),
            mLineColors(other.mLineColors), mLineIndices(other.mLineIndices)
    {
    }

    DrawableLines(DrawableLines&& other) { swap(other); }

    ~DrawableLines() = default;

    DrawableLines& operator=(DrawableLines other)
    {
        swap(other);
        return *this;
    }

    void swap(DrawableLines& other)
    {
        using std::swap;
        DrawableObject::swap(other);
        swap(static_cast<Lines&>(*this), static_cast<Lines&>(other));

        swap(mVisible, other.mVisible);
        swap(mUseLineIndices, other.mUseLineIndices);

        swap(mVertCoords, other.mVertCoords);
        swap(mLineIndices, other.mLineIndices);
        swap(mVertColors, other.mVertColors);
        swap(mVertNormals, other.mVertNormals);
        swap(mLineColors, other.mLineColors);
    }

    friend void swap(DrawableLines& first, DrawableLines& second)
    {
        first.swap(second);
    }

    void setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<float>& vertNormals,
        const std::vector<uint>&  vertColors,
        const std::vector<uint>&  lineColors)
    {
        mUseLineIndices = false;
        mVertCoords  = vertCoords;
        mVertColors  = vertColors;
        mVertNormals = vertNormals;
        mLineColors  = lineColors;
        Lines::setPoints(vertCoords, vertNormals, vertColors, mLineColors);
    }

    void setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  lineIndices,
        const std::vector<float>& vertNormals,
        const std::vector<uint>&  vertColors,
        const std::vector<uint>&  lineColors)
    {
        mUseLineIndices = true;
        mVertCoords  = vertCoords;
        mLineIndices = lineIndices;
        mVertColors  = vertColors;
        mVertNormals = vertNormals;
        mLineColors  = lineColors;
        Lines::setPoints(
            vertCoords, lineIndices, vertNormals, vertColors, mLineColors);
    }

    // DrawableObject interface

    void draw(uint viewId) const override { Lines::draw(viewId); }

    vcl::Box3d boundingBox() const override
    {
        return vcl::Box3d(vcl::Point3d(-1, -1, -1), vcl::Point3d(1, 1, 1));
    }

    std::shared_ptr<DrawableObject> clone() const& override
    {
        return std::make_shared<DrawableLines>(*this);
    }

    std::shared_ptr<DrawableObject> clone() && override
    {
        return std::make_shared<DrawableLines>(std::move(*this));
    }

    bool isVisible() const override { return mVisible; }

    void setVisibility(bool vis) override { mVisible = vis; }

    void setImplementationType(ImplementationType type)
    {
        if (mUseLineIndices) {
            Lines::setPoints(
                mVertCoords,
                mLineIndices,
                mVertNormals,
                mVertColors,
                mLineColors,
                type);
        }
        else {
            Lines::setPoints(
                mVertCoords, mVertNormals, mVertColors, mLineColors, type);
        }
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_LINES_H
