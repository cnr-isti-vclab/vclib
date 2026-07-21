// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_AXIS_H
#define VCL_BGFX_DRAWABLE_DRAWABLE_AXIS_H

#include "mesh/mesh_render_buffers.h"

#include <vclib/algorithms/mesh/create.h>
#include <vclib/meshes/tri_mesh.h>
#include <vclib/render/drawable/drawable_object.h>
#include <vclib/space/core/matrix.h>

#include <vclib/bgfx/context.h>

#include <bgfx/bgfx.h>

namespace vcl {

/**
 * @brief A drawable object representing a 3D coordinate axis indicator.
 *
 * DrawableAxis renders a standard 3D axis gizmo with colored arrows for each
 * principal direction: red for X, green for Y, and blue for Z. Each axis
 * consists of a cylinder body and a cone tip, both drawn with appropriate
 * coloring.
 *
 * The axis indicator is centered at the origin and can be scaled uniformly
 * using the constructor parameter or the setSize() method. It is typically
 * used in 3D viewers to provide spatial orientation reference.
 *
 * @note The axis is initially invisible by default; call setVisibility(true)
 * to enable rendering.
 *
 * @code{.cpp}
 * auto axis = std::make_shared<DrawableAxis>(2.0);
 * axis->setVisibility(true);
 * viewer->addDrawable(axis);
 * @endcode
 */
class DrawableAxis : public DrawableObject
{
    inline static const std::pair<vcl::TriMesh, vcl::TriMesh> AXIS_MESHES =
        vcl::createAxisDisjoint<vcl::TriMesh>();

    inline static const vcl::Color AXIS_COLORS[3] = {
        vcl::Color::Red,
        vcl::Color::Green,
        vcl::Color::Blue};

    bool mVisible = false;

    vcl::Matrix44f mMatrices[3] = {
        vcl::Matrix44f::Zero(),
        vcl::Matrix44f::Zero(),
        vcl::Matrix44f::Zero()};

    MeshRenderBuffers<vcl::TriMesh> mArrowBuffers[2]; // 0: cylinder, 1: cone

public:
    DrawableAxis(double size = 1);

    /**
     * @brief Copy constructor.
     *
     * Creates a new DrawableAxis that is a copy of the specified one,
     * including its visibility state and size.
     *
     * @param[in] other: The DrawableAxis to copy.
     */
    DrawableAxis(const DrawableAxis& other) : mVisible(other.mVisible)
    {
        for (uint i = 0; i < 3; i++) {
            mMatrices[i] = other.mMatrices[i];
        }

        createAxis();
    }

    /**
     * @brief Move constructor.
     *
     * Transfers ownership of resources from the specified DrawableAxis.
     *
     * @param[in] other: The DrawableAxis to move from.
     */
    DrawableAxis(DrawableAxis&& other) { swap(other); }

    ~DrawableAxis() = default;

    /**
     * @brief Copy assignment operator (implemented via copy-and-swap idiom).
     *
     * @param[in] other: The DrawableAxis to assign from.
     * @return Reference to this DrawableAxis.
     */
    DrawableAxis& operator=(DrawableAxis other)
    {
        swap(other);
        return *this;
    }

    /**
     * @brief Swaps the contents of this DrawableAxis with another.
     *
     * @param[in] other: The DrawableAxis to swap with.
     */
    void swap(DrawableAxis& other)
    {
        using std::swap;
        swap(mVisible, other.mVisible);
        for (uint i = 0; i < 3; i++) {
            swap(mMatrices[i], other.mMatrices[i]);
        }
        for (uint i = 0; i < 2; i++) {
            mArrowBuffers[i].swap(other.mArrowBuffers[i]);
        }
    }

    friend void swap(DrawableAxis& a, DrawableAxis& b) { a.swap(b); }

    void setSize(double size);

    // DrawableObject interface

    void draw(const DrawObjectSettings& settings) override;

    /**
     * @brief Returns the bounding box of the axis indicator.
     *
     * Always returns an empty bounding box, indicating that this object
     * should not influence camera positioning or framing.
     *
     * @return An empty Box3d.
     *
     * @see DrawableObject::boundingBox()
     */
    Box3d boundingBox() const override { return Box3d(); }

    /**
     * @brief Creates a deep copy of this DrawableAxis.
     *
     * @return A shared pointer to a new DrawableAxis that is a copy of this
     * one.
     *
     * @see DrawableObject::clone()
     */
    std::shared_ptr<DrawableObject> clone() const& override
    {
        return std::make_shared<DrawableAxis>(*this);
    }

    /**
     * @brief Creates a new DrawableAxis by moving this one.
     *
     * @return A shared pointer to a new DrawableAxis moved from this one.
     *
     * @see DrawableObject::clone()
     */
    std::shared_ptr<DrawableObject> clone() && override
    {
        return std::make_shared<DrawableAxis>(std::move(*this));
    }

    /**
     * @brief Checks whether the axis indicator is currently visible.
     *
     * @return `true` if the axis will be drawn, `false` otherwise.
     *
     * @see DrawableObject::isVisible()
     */
    bool isVisible() const override { return mVisible; }

    /**
     * @brief Sets the visibility of the axis indicator.
     *
     * @param[in] vis: `true` to enable rendering, `false` to hide the axis.
     *
     * @see DrawableObject::setVisibility()
     */
    void setVisibility(bool vis) override { mVisible = vis; }

private:
    void updateMatrices(double size);

    void createAxis();
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_AXIS_H
