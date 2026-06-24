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

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_LINES_H
#define VCL_BGFX_DRAWABLE_DRAWABLE_LINES_H

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/primitives/lines.h>
#include <vclib/render/drawable/drawable_object.h>

#include <bgfx/bgfx.h>

namespace vcl {

/**
 * @brief A DrawableObject that renders a set of 3D lines.
 *
 * This class wraps the `vcl::Lines` primitive to be used within the VCLib
 * rendering framework as a `DrawableObject`. It maintains a local CPU copy
 * of the vertices, indices, normals, and colors to support `DrawableObject` 
 * interfaces such as bounding box computation and cloning.
 */
class DrawableLines : public DrawableObject, public Lines
{
    bool mVisible = true;

    std::vector<vcl::Point3d> mPositions;
    std::vector<uint>         mIndices;
    std::vector<vcl::Color>   mVertexColors;
    std::vector<vcl::Color>   mLineColors;
    std::vector<vcl::Point3d> mVertexNormals;
    std::vector<vcl::Point3d> mLineNormals;

    using Lines::setVertices;
    using Lines::setIndices;
    using Lines::setVertexColors;
    using Lines::setLineColors;
    using Lines::setVertexNormals;
    using Lines::setLineNormals;
    using Lines::draw;

public:
    using Lines::ColorSetting;
    using Lines::Topology;
    using Lines::Shading;

    /**
     * @brief Default constructor. Creates an empty drawable line set.
     */
    DrawableLines() = default;

    /**
     * @brief Copy constructor. Creates a deep copy of the line set.
     */
    DrawableLines(const DrawableLines& other) :
            DrawableObject(other),
            Lines(),
            mVisible(other.mVisible), 
            mPositions(other.mPositions),
            mIndices(other.mIndices),
            mVertexColors(other.mVertexColors),
            mLineColors(other.mLineColors),
            mVertexNormals(other.mVertexNormals),
            mLineNormals(other.mLineNormals)
    {
        if (!mPositions.empty()) Lines::setVertices(mPositions);
        if (!mIndices.empty()) Lines::setIndices(mIndices);
        if (!mVertexColors.empty()) Lines::setVertexColors(mVertexColors);
        if (!mLineColors.empty()) Lines::setLineColors(mLineColors);
        if (!mVertexNormals.empty()) Lines::setVertexNormals(mVertexNormals);
        if (!mLineNormals.empty()) Lines::setLineNormals(mLineNormals);

        Lines::setWidth(other.width());
    }

    /**
     * @brief Move constructor.
     */
    DrawableLines(DrawableLines&& other) { swap(other); }

    ~DrawableLines() = default;

    /**
     * @brief Copy assignment operator.
     */
    DrawableLines& operator=(DrawableLines other)
    {
        swap(other);
        return *this;
    }

    /**
     * @brief Swaps the contents of this object with another.
     */
    void swap(DrawableLines& other)
    {
        using std::swap;
        DrawableObject::swap(other);
        swap(static_cast<Lines&>(*this), static_cast<Lines&>(other));

        swap(mVisible, other.mVisible);

        swap(mPositions, other.mPositions);
        swap(mIndices, other.mIndices);
        swap(mVertexColors, other.mVertexColors);
        swap(mLineColors, other.mLineColors);
        swap(mVertexNormals, other.mVertexNormals);
        swap(mLineNormals, other.mLineNormals);
    }

    /**
     * @brief Sets line coordinates from a range of 3D points and stores a local copy.
     *
     * @tparam R Range type satisfying Point3Concept.
     * @param verts Range of 3D points.
     */
    template<Range R>
    requires Point3Concept<std::ranges::range_value_t<R>>
    void setVertices(R&& verts)
    {
        Lines::setVertices(verts);
        mPositions.assign(std::ranges::begin(verts), std::ranges::end(verts));
    }

    /**
     * @brief Sets line indices from a range of indices and stores a local copy.
     *
     * @tparam R Range type satisfying integral concept.
     * @param indices Range of indices.
     */
    template<Range R>
    requires std::integral<std::ranges::range_value_t<R>>
    void setIndices(R&& indices)
    {
        Lines::setIndices(indices);
        mIndices.assign(std::ranges::begin(indices), std::ranges::end(indices));
    }

    /**
     * @brief Sets per-vertex colors and stores a local copy.
     *
     * @tparam R Range type satisfying ColorConcept.
     * @param vertColors Range of Colors.
     */
    template<Range R>
    requires ColorConcept<std::ranges::range_value_t<R>>
    void setVertexColors(R&& vertColors)
    {
        Lines::setVertexColors(vertColors);
        mVertexColors.assign(
            std::ranges::begin(vertColors), std::ranges::end(vertColors));
    }

    /**
     * @brief Sets per-line colors and stores a local copy.
     *
     * @tparam R Range type satisfying ColorConcept.
     * @param linColors Range of Colors.
     */
    template<Range R>
    requires ColorConcept<std::ranges::range_value_t<R>>
    void setLineColors(R&& linColors)
    {
        Lines::setLineColors(linColors);
        mLineColors.assign(
            std::ranges::begin(linColors), std::ranges::end(linColors));
    }

    /**
     * @brief Sets per-vertex normals and stores a local copy.
     *
     * @tparam R Range type satisfying Point3Concept.
     * @param vertNormals Range of 3D points representing normals.
     */
    template<Range R>
    requires Point3Concept<std::ranges::range_value_t<R>>
    void setVertexNormals(R&& vertNormals)
    {
        Lines::setVertexNormals(vertNormals);
        mVertexNormals.assign(
            std::ranges::begin(vertNormals), std::ranges::end(vertNormals));
    }

    /**
     * @brief Sets per-line normals and stores a local copy.
     *
     * @tparam R Range type satisfying Point3Concept.
     * @param lineNormals Range of 3D points representing normals.
     */
    template<Range R>
    requires Point3Concept<std::ranges::range_value_t<R>>
    void setLineNormals(R&& lineNormals)
    {
        Lines::setLineNormals(lineNormals);
        mLineNormals.assign(
            std::ranges::begin(lineNormals), std::ranges::end(lineNormals));
    }

    // DrawableObject interface

    void draw(const DrawObjectSettings& settings) override
    {
        Lines::draw(settings.viewId);
    }

    vcl::Box3d boundingBox() const override
    {
        return vcl::boundingBox(mPositions);
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
