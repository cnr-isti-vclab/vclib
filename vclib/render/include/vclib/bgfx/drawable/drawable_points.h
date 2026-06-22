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

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_POINTS_H
#define VCL_BGFX_DRAWABLE_DRAWABLE_POINTS_H

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/primitives/points.h>
#include <vclib/render/drawable/drawable_object.h>

#include <bgfx/bgfx.h>

namespace vcl {

class DrawablePoints: public DrawableObject, private Points
{
    bool mVisible = true;

    std::vector<vcl::Point3d> mPositions;
    std::vector<vcl::Point3d> mNormals;
    std::vector<vcl::Color>   mColors;

public:
    using Points::ColorSetting;
    using Points::Shading;
    using Points::Shape;
    using Points::setSize;
    using Points::setColorSetting;
    using Points::setShading;
    using Points::setShape;
    using Points::setGeneralColor;

    DrawablePoints() = default;

    DrawablePoints(const DrawablePoints& other) :
            DrawableObject(other),
            Points(other.mPositions, other.mNormals, other.mColors),
            mVisible(other.mVisible), mPositions(other.mPositions),
            mNormals(other.mNormals), mColors(other.mColors)
    {
    }

    DrawablePoints(DrawablePoints&& other) { swap(other); }

    ~DrawablePoints() = default;

    DrawablePoints& operator=(DrawablePoints other)
    {
        swap(other);
        return *this;
    }

    void swap(DrawablePoints& other)
    {
        using std::swap;
        DrawableObject::swap(other);
        swap(static_cast<Points&>(*this), static_cast<Points&>(other));

        swap(mVisible, other.mVisible);

        swap(mPositions, other.mPositions);
        swap(mNormals, other.mNormals);
        swap(mColors, other.mColors);
    }

    template<Range R>
    requires Point3Concept<std::ranges::range_value_t<R>>
    void setVertices(R&& verts)
    {
        Points::setVertices(verts);
        mPositions.assign(std::ranges::begin(verts), std::ranges::end(verts));
    }

    template<Range R>
    requires Point3Concept<std::ranges::range_value_t<R>>
    void setVertexNormals(R&& vertNormals)
    {
        Points::setVertexNormals(vertNormals);
        mNormals.assign(
            std::ranges::begin(vertNormals), std::ranges::end(vertNormals));
    }

    template<Range R>
    requires ColorConcept<std::ranges::range_value_t<R>>
    void setVertexColors(R&& vertColors)
    {
        Points::setVertexColors(vertColors);
        mColors.assign(
            std::ranges::begin(vertColors), std::ranges::end(vertColors));
    }

    // DrawableObject interface

    void draw(const DrawObjectSettings& settings) override
    {
        Points::draw(settings.viewId);
    }

    vcl::Box3d boundingBox() const override
    {
        return vcl::boundingBox(mPositions);
    }

    std::shared_ptr<DrawableObject> clone() const& override
    {
        return std::make_shared<DrawablePoints>(*this);
    }

    std::shared_ptr<DrawableObject> clone() && override
    {
        return std::make_shared<DrawablePoints>(std::move(*this));
    }

    bool isVisible() const override { return mVisible; }

    void setVisibility(bool vis) override { mVisible = vis; }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_POINTS_H
