// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_OPENGL2_GIZMOS_TRACKBALL_GIZMO_OPENGL2_H
#define VCL_OPENGL2_GIZMOS_TRACKBALL_GIZMO_OPENGL2_H

#include <vclib/render/gizmos/abstract_gizmo.h>

#include <vclib/algorithms/core.h>
#include <vclib/space/core.h>

namespace vcl {

/**
 * @brief The TrackballGizmoOpenGL2 class is a visual gizmo that
 * renders a trackball using the OpenGL2 backend.
 *
 * It draws a 3D gizmo composed of three circles.
 */
class TrackballGizmoOpenGL2 : public AbstractGizmo
{
    bool mVisible = true;

    vcl::Matrix44f mTransform  = vcl::Matrix44f::Identity();
    bool           mIsDragging = false;

public:
    TrackballGizmoOpenGL2() = default;

    void updateDragging(bool isDragging);

    void setTransform(const vcl::Matrix44f& mtx);

    // AbstractGizmo interface

    void init() override {}

    void draw(uint viewId = 0) override;

    bool isVisible() const override;

    void setVisibility(bool vis) override;
};

} // namespace vcl

#endif // VCL_OPENGL2_GIZMOS_TRACKBALL_GIZMO_OPENGL2_H
