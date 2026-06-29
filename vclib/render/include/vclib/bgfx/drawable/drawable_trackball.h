// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_TRACKBALL_H
#define VCL_BGFX_DRAWABLE_DRAWABLE_TRACKBALL_H

#include <vclib/algorithms/core.h>
#include <vclib/bgfx/buffers.h>
#include <vclib/render/drawable/drawable_object.h>
#include <vclib/space/core.h>

namespace vcl {
class DrawableTrackBall : public DrawableObject
{
    bool mVisible = true;

    VertexBuffer mVertexPosColorBuffer;
    IndexBuffer  mEdgeIndexBuffer;

    vcl::Matrix44f mTransform  = vcl::Matrix44f::Identity();
    bool           mIsDragging = false;

public:
    DrawableTrackBall();

    DrawableTrackBall(const DrawableTrackBall& other);

    // default move constructor - buffers can be moved
    DrawableTrackBall(DrawableTrackBall&& other) = default;

    // default destructor - buffers are destroyed by their destructor
    ~DrawableTrackBall() = default;

    /**
     * @brief Swap the content of this object with another DrawableTrackBall
     * object.
     *
     * @param[in] other: the other DrawableTrackBall object.
     */
    void swap(DrawableTrackBall& other);

    friend void swap(DrawableTrackBall& a, DrawableTrackBall& b) { a.swap(b); }

    /**
     * @brief Update the dragging status of the trackball.
     * @param[in] isDragging: true if the trackball is being dragged, false
     * otherwise.
     */
    void updateDragging(bool isDragging);

    void setTransform(const vcl::Matrix44f& mtx);

    // copy and swap idiom
    DrawableTrackBall& operator=(DrawableTrackBall other);

    // DrawableObject interface

    void draw(const DrawObjectSettings& settings) override;

    Box3d boundingBox() const override;

    std::shared_ptr<DrawableObject> clone() const& override;

    std::shared_ptr<DrawableObject> clone() && override;

    bool isVisible() const override;

    void setVisibility(bool vis) override;

private:
    void createBuffers();
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_TRACKBALL_H
