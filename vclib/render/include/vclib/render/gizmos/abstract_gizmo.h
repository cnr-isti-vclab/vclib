// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_GIZMOS_ABSTRACT_GIZMO_H
#define VCL_RENDER_GIZMOS_ABSTRACT_GIZMO_H

#include <vclib/base.h>

namespace vcl {

/**
 * @brief The AbstractGizmo class provides the base interface for rendering
 * visual overlays (gizmos) in the scene.
 *
 * Gizmos are interactive or informative graphical elements (e.g., trackball
 * rings, directional light vectors) that are drawn on top of or within the 3D
 * scene, but do not represent physical geometry of the model.
 * Consequently, they do not provide bounding boxes and are not usually part
 * of the core object vectors.
 */
class AbstractGizmo
{
public:
    virtual ~AbstractGizmo() = default;

    /**
     * @brief Initializes the gizmo's graphic resources (e.g. GPU buffers).
     */
    virtual void init() = 0;

    /**
     * @brief Draws the gizmo.
     * @param viewId The ID of the view/pass to draw into (mainly for BGFX).
     */
    virtual void draw(uint viewId = 0) = 0;

    /**
     * @brief Checks if the gizmo is currently visible.
     */
    virtual bool isVisible() const = 0;

    /**
     * @brief Sets the visibility of the gizmo.
     */
    virtual void setVisibility(bool v) = 0;
};

} // namespace vcl

#endif // VCL_RENDER_GIZMOS_ABSTRACT_GIZMO_H
