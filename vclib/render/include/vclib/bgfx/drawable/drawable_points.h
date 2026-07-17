// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_POINTS_H
#define VCL_BGFX_DRAWABLE_DRAWABLE_POINTS_H

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/primitives/points.h>
#include <vclib/render/drawable/drawable_object.h>

#include <vclib/algorithms/core.h>

#include <bgfx/bgfx.h>

namespace vcl {

/**
 * @brief A DrawableObject that renders a set of 3D points.
 *
 * This class wraps the `vcl::Points` primitive to be used within the VCLib
 * rendering framework as a `DrawableObject`. It maintains a local CPU copy
 * of the vertices, normals, and colors to support `DrawableObject` interfaces
 * such as bounding box computation and cloning.
 */
class DrawablePoints : public DrawableObject, public Points
{
    bool mVisible = true;

    std::vector<vcl::Point3d> mPositions;
    std::vector<vcl::Point3d> mNormals;
    std::vector<vcl::Color>   mColors;

    using Points::draw;
    using Points::setVertexColors;
    using Points::setVertexNormals;
    using Points::setVertices;

public:
    using Points::ColorSetting;
    using Points::Shading;
    using Points::Shape;

    /**
     * @brief Default constructor. Creates an empty drawable point set.
     */
    DrawablePoints() = default;

    /**
     * @brief Copy constructor. Creates a deep copy of the point set.
     */
    DrawablePoints(const DrawablePoints& other) :
            DrawableObject(other),
            Points(other.mPositions, other.mNormals, other.mColors),
            mVisible(other.mVisible), mPositions(other.mPositions),
            mNormals(other.mNormals), mColors(other.mColors)
    {
    }

    /**
     * @brief Move constructor.
     */
    DrawablePoints(DrawablePoints&& other) { swap(other); }

    ~DrawablePoints() = default;

    /**
     * @brief Copy assignment operator.
     */
    DrawablePoints& operator=(DrawablePoints other)
    {
        swap(other);
        return *this;
    }

    /**
     * @brief Swaps the contents of this object with another.
     */
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

    /**
     * @brief Sets point positions from a range of 3D points and stores a local
     * copy.
     *
     * @tparam R Range type satisfying Point3Concept.
     * @param verts Range of 3D points.
     */
    template<Range R>
    requires Point3Concept<std::ranges::range_value_t<R>>
    void setVertices(R&& verts)
    {
        Points::setVertices(verts);
        mPositions.assign(std::ranges::begin(verts), std::ranges::end(verts));
    }

    /**
     * @brief Sets per-point normals from a range of 3D points and stores a
     * local copy.
     *
     * @tparam R Range type satisfying Point3Concept.
     * @param vertNormals Range of 3D points representing normals.
     */
    template<Range R>
    requires Point3Concept<std::ranges::range_value_t<R>>
    void setVertexNormals(R&& vertNormals)
    {
        Points::setVertexNormals(vertNormals);
        mNormals.assign(
            std::ranges::begin(vertNormals), std::ranges::end(vertNormals));
    }

    /**
     * @brief Sets per-point colors from a range of Colors and stores a local
     * copy.
     *
     * @tparam R Range type satisfying ColorConcept.
     * @param vertColors Range of Colors.
     */
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
