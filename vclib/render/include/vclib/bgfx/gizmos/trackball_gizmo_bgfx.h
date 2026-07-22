// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_GIZMOS_TRACKBALL_GIZMO_BGFX_H
#define VCL_BGFX_GIZMOS_TRACKBALL_GIZMO_BGFX_H

#include <vclib/bgfx/buffers.h>
#include <vclib/render/gizmos/abstract_gizmo.h>

#include <vclib/algorithms/core.h>
#include <vclib/space/core.h>

namespace vcl {
/**
 * @brief The TrackballGizmoBGFX class is a visual gizmo that
 * renders a trackball using the BGFX backend.
 *
 * It draws a 3D gizmo composed of three circles.
 */
class TrackballGizmoBGFX : public AbstractGizmo
{
    bool mVisible = true;

    VertexBuffer mVertexPosColorBuffer;
    IndexBuffer  mEdgeIndexBuffer;

    vcl::Matrix44f mTransform  = vcl::Matrix44f::Identity();
    bool           mIsDragging = false;

public:
    TrackballGizmoBGFX();

    // default move constructor - buffers can be moved
    TrackballGizmoBGFX(TrackballGizmoBGFX&& other) = default;

    // default destructor - buffers are destroyed by their destructor
    ~TrackballGizmoBGFX() = default;

    /**
     * @brief Swap the content of this object with another TrackballGizmoBGFX
     * object.
     *
     * @param[in] other: the other TrackballGizmoBGFX object.
     */
    void swap(TrackballGizmoBGFX& other);

    friend void swap(TrackballGizmoBGFX& a, TrackballGizmoBGFX& b)
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

    // AbstractGizmo interface

    void init() override;

    void draw(uint viewId = 0) override;

    bool isVisible() const override;

    void setVisibility(bool vis) override;

private:
    void createBuffers();
};

} // namespace vcl

#endif // VCL_BGFX_GIZMOS_TRACKBALL_GIZMO_BGFX_H
