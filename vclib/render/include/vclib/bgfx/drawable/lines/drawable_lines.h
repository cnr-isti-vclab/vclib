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

#ifndef VCL_BGFX_DRAWABLE_LINES_DRAWABLE_LINES_H
#define VCL_BGFX_DRAWABLE_LINES_DRAWABLE_LINES_H

#include "lines/cpu_generated_lines.h"
#include "lines/gpu_generated_lines.h"
#include "lines/instancing_based_lines.h"
#include "lines/indirect_based_lines.h"
#include "lines/texture_based_lines.h"

#include "lines_utils.h"

#include <vclib/bgfx/context.h>
#include <vclib/render/drawable/drawable_object.h>

#include <bgfx/bgfx.h>

namespace vcl::lines {

template<typename LinesImplementation = CPUGeneratedLines>
class DrawableLines : public vcl::DrawableObject
{
    std::vector<LinesVertex> mPoints;
    LinesImplementation      mLines;
    bool                     mVisible = true;

public:
    DrawableLines() = default;

    DrawableLines(const std::vector<LinesVertex>& points) :
            mPoints(points), mLines(points)
    {
    }

    DrawableLines(const DrawableLines& other) : DrawableObject(other),
            mPoints(other.mPoints), mLines(other.mPoints),
            mVisible(other.mVisible)
    {
        mLines.setSettings(*other.getSettings());
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
        swap(mPoints, other.mPoints);
        swap(mLines, other.mLines);
        swap(mVisible, other.mVisible);
    }

    friend void swap(DrawableLines& first, DrawableLines& second)
    {
        first.swap(second);
    }

    LinesSettings* getSettings() const { return mLines.getSettings(); }

    void setSettings(const LinesSettings settings)
    {
        mLines.setSettings(settings);
    }

    void update(const std::vector<LinesVertex>& points)
    {
        mLines.update(points);
    }

    // DrawableObject interface

    void draw(uint viewId) const override { mLines.draw(viewId); }

    vcl::Box3d boundingBox() const override
    {
        return vcl::Box3d(vcl::Point3d(-1, -1, -1), vcl::Point3d(1, 1, 1));
    }

    std::shared_ptr<DrawableObject> clone() const override
    {
        return std::make_shared<DrawableLines>(*this);
    }

    bool isVisible() const override { return mVisible; }

    void setVisibility(bool vis) override { mVisible = vis; }
};

using DrawableCPULines = DrawableLines<CPUGeneratedLines>;
using DrawableGPULines = DrawableLines<GPUGeneratedLines>;
using DrawableIndirectLines = DrawableLines<IndirectBasedLines>;
using DrawableInstancingLines = DrawableLines<InstancingBasedLines>;
using DrawableTextureLines = DrawableLines<TextureBasedLines>;

} // namespace vcl::lines

#endif // VCL_BGFX_DRAWABLE_LINES_DRAWABLE_LINES_H
