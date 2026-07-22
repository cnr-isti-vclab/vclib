// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_TRACKBALL_BGFX_H
#define VCL_BGFX_DRAWABLE_DRAWABLE_TRACKBALL_BGFX_H

#include <vclib/algorithms/core.h>
#include <vclib/bgfx/buffers.h>
#include <vclib/render/drawable/drawable_object.h>
#include <vclib/space/core.h>

namespace vcl {
/**
 * @brief The DrawableTrackBallBGFX class is a drawable object that
 * renders a trackball using the BGFX backend.
 *
 * It draws a 3D gizmo composed of three circles.
 */
class DrawableTrackBallBGFX : public DrawableObject
{
    bool mVisible = true;

    VertexBuffer mVertexPosColorBuffer;
    IndexBuffer  mEdgeIndexBuffer;

    vcl::Matrix44f mTransform  = vcl::Matrix44f::Identity();
    bool           mIsDragging = false;

public:
    DrawableTrackBallBGFX();

    // default move constructor - buffers can be moved
    DrawableTrackBallBGFX(DrawableTrackBallBGFX&& other) = default;

    // default destructor - buffers are destroyed by their destructor
    ~DrawableTrackBallBGFX() = default;

    /**
     * @brief Swap the content of this object with another DrawableTrackBallBGFX
     * object.
     *
     * @param[in] other: the other DrawableTrackBallBGFX object.
     */
    void swap(DrawableTrackBallBGFX& other);

    friend void swap(DrawableTrackBallBGFX& a, DrawableTrackBallBGFX& b)
    {
        a.swap(b);
    }

    /**
     * @brief Update the dragging status of the trackball.
     * @param[in] isDragging: true if the trackball is being dragged, false
     * otherwise.
     */
    void updateDragging(bool isDragging);

    void setTransform(const vcl::Matrix44f& mtx);

    // DrawableObject interface

    void draw(const DrawObjectSettings& settings) override;

    Box3d boundingBox() const override;

    bool isVisible() const override;

    void setVisibility(bool vis) override;

private:
    void createBuffers();
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_TRACKBALL_BGFX_H
