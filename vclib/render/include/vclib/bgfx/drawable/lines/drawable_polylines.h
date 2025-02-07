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

#ifndef VCL_BGFX_DRAWABLE_LINES_DRAWABLE_POLYLINES_H
#define VCL_BGFX_DRAWABLE_LINES_DRAWABLE_POLYLINES_H

#include "polylines/cpu_generated_polylines.h"
#include "polylines/gpu_generated_polylines.h"
#include "polylines/indirect_based_polylines.h"
#include "polylines/instancing_based_polylines.h"
#include "polylines/texture_based_polylines.h"

#include <vclib/bgfx/context.h>
#include <vclib/render/drawable/drawable_object.h>

#include <bgfx/bgfx.h>

namespace vcl {

template<typename PolylinesImplementation = CPUGeneratedPolylines>
class DrawablePolylines : public vcl::DrawableObject
{
    std::vector<LinesVertex> mPoints;
    PolylinesImplementation  mPolylines;
    bool                     mVisible = true;

public:
    DrawablePolylines() = default;

    DrawablePolylines(const std::vector<LinesVertex>& points) : mPoints(points)
    {
    }

    DrawablePolylines(const DrawablePolylines& other) : DrawableObject(other),
            mPoints(other.mPoints), mPolylines(other.mPoints),
            mVisible(other.mVisible)
    {
        mPolylines.settings() = other.settings();
    }

    DrawablePolylines(DrawablePolylines&& other) { swap(other); }

    ~DrawablePolylines() = default;

    DrawablePolylines& operator=(DrawablePolylines other)
    {
        swap(other);
        return *this;
    }

    void swap(DrawablePolylines& other)
    {
        using std::swap;
        DrawableObject::swap(other);
        swap(mPoints, other.mPoints);
        swap(mPolylines, other.mPolylines);
        swap(mVisible, other.mVisible);
    }

    friend void swap(DrawablePolylines& first, DrawablePolylines& second)
    {
        first.swap(second);
    }

    LineSettings& settings() { return mPolylines.settings(); }

    const LineSettings& settings() const
    {
        return mPolylines.settings();
    }

    void update(const std::vector<LinesVertex>& points)
    {
        mPoints = points;
        mPolylines.update(points);
    }

    // DrawableObject interface

    void draw(uint viewId) const override { mPolylines.draw(viewId); }

    vcl::Box3d boundingBox() const override
    {
        return vcl::Box3d(vcl::Point3d(-1, -1, -1), vcl::Point3d(1, 1, 1));
    }

    std::shared_ptr<DrawableObject> clone() const override
    {
        return std::make_shared<DrawablePolylines>(*this);
    }

    bool isVisible() const override { return mVisible; }

    void setVisibility(bool vis) override { mVisible = vis; }
};

using DrawableCPUPolylines = DrawablePolylines<CPUGeneratedPolylines>;
using DrawableGPUPolylines = DrawablePolylines<GPUGeneratedPolylines>;
using DrawableIndirectPolylines = DrawablePolylines<IndirectBasedPolylines>;
using DrawableInstancingPolylines = DrawablePolylines<InstancingBasedPolylines>;
using DrawableTexturePolylines = DrawablePolylines<TextureBasedPolylines>;

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_LINES_DRAWABLE_POLYLINES_H
