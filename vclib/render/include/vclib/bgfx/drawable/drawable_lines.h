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
#include <vclib/render/drawable/drawable_object.h>
#include <vclib/render/primitives/lines.h>

#include <bgfx/bgfx.h>

namespace vcl {

class DrawableLines : public vcl::DrawableObject
{
    bool                mVisible = true;
    Lines               mLines;
    std::vector<float>  mVertCoords;
    std::vector<uint>   mVertColors;
    std::vector<float>  mVertNormals;
    std::vector<uint>   mLineColors;

public:
    DrawableLines() = default;

    DrawableLines(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  vertColors = std::vector<uint>(),
        const std::vector<float>& vertNormals = std::vector<float>(),
        const std::vector<uint>& lineColors = std::vector<uint>()) :
            mVertCoords(vertCoords)
    {
        if (!vertColors.empty())  mVertColors = vertColors;
        else                      mVertColors = std::vector<uint>(vertCoords.size(), 0x000000FF);

        if (!vertNormals.empty()) mVertNormals = vertNormals;
        else                      mVertNormals = std::vector<float>(vertCoords.size(), 0.0f);

        if (!lineColors.empty())  mLineColors = lineColors;
        else                      mLineColors = std::vector<uint>(vertCoords.size() / 6, 0xFFFFFFFF);

        mLines.setPoints(mVertCoords, mVertColors, mVertNormals, mLineColors);
    }

    DrawableLines(const DrawableLines& other) :
            DrawableObject(other), mVertCoords(other.mVertCoords),
            mVertColors(other.mVertColors), mVertNormals(other.mVertNormals),
            mLineColors(other.mLineColors), mVisible(other.mVisible),
            mLines(other.mVertCoords, other.mVertColors, other.mVertNormals, other.mLineColors)
    {
        mLines.setThickness(other.mLines.getThickness());
        mLines.setColorToUse(other.mLines.getColorToUse());
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

        swap(mVertCoords, other.mVertCoords);
        swap(mVertColors, other.mVertColors);
        swap(mVertNormals, other.mVertNormals);
        swap(mLineColors, other.mLineColors);
        swap(mLines, other.mLines);
        swap(mVisible, other.mVisible);
    }

    friend void swap(DrawableLines& first, DrawableLines& second)
    {
        first.swap(second);
    }

    void setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  vertColors,
        const std::vector<float>& vertNormals,
        const std::vector<uint>& lineColors)
    {
        mVertCoords = vertCoords;
        mVertColors = vertColors;
        mVertNormals = vertNormals;
        mLineColors = lineColors;
        mLines.setPoints(vertCoords, vertColors, vertNormals, mLineColors);
    }

    uint8_t getThickness() const { return mLines.getThickness(); }
        
    LinesColorToUse getColorToUse() const { return mLines.getColorToUse(); }
        
    void setThickness(uint8_t thickness) { mLines.setThickness(thickness); }
        
    void setColorToUse(LinesColorToUse colorToUse) { mLines.setColorToUse(colorToUse); }

    // DrawableObject interface

    void draw(uint viewId) const override { mLines.draw(viewId); }

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
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_LINES_H
