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

#ifndef VCL_BGFX_SCREENSPACE_OVERLAY_SCREENSPACE_BOX_H
#define VCL_BGFX_SCREENSPACE_OVERLAY_SCREENSPACE_BOX_H

#include "uniforms/screenspace_box_uniforms.h"

#include <vclib/bgfx/buffers/dynamic_vertex_buffer.h>
#include <vclib/bgfx/buffers/index_buffer.h>
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>
#include <vclib/bgfx/programs/embedded_vf_programs/screenspace_box.h>

#include <vclib/algorithms/core.h>
#include <vclib/space/core.h>

#include <array>

namespace vcl {

/**
 * @brief A screen-space overlay box drawn in pixel coordinates.
 *
 * ScreenSpaceBox renders a filled rectangle in screen space using bgfx.
 * The box is defined by a Box2d (min-max corners in pixel coordinates).
 * This class manages its own vertex and index buffers.
 */
class ScreenSpaceBox
{
    // ---- BGFX rendering resources ----
    DynamicVertexBuffer mPosBuf;
    IndexBuffer         mTriIndexBuf;
    bool                mInitialized = false;

    // ---- Box state ----
    Color mColor   = {27, 120, 249, 64};
    bool  mVisible = true;
    Box2d mBox;

public:
    ScreenSpaceBox() = default;

    /**
     * @brief Initializes BGFX buffers for the selection box.
     *
     * Creates a dynamic vertex buffer for 4 vertices (2D positions)
     * and an index buffer for 2 triangles forming a filled rectangle.
     */
    void init()
    {
        if (mInitialized)
            return;

        // Create position dynamic vertex buffer
        bgfx::VertexLayout vertexLayout;
        vertexLayout.begin()
            .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
            .end();
        mPosBuf.create(4, vertexLayout);

        // Create index buffer for the two box triangles
        std::array<uint, 6> indices {1, 2, 0, 2, 3, 0};
        mTriIndexBuf.create(bgfx::copy(indices.data(), 6 * sizeof(uint)));

        mInitialized = true;
    }

    /**
     * @brief Returns true if the box has been initialized.
     */
    bool isInitialized() const { return mInitialized; }

    /**
     * @brief Sets the box rectangle in screen (pixel) coordinates.
     */
    void setBox(const Box2d& box) { mBox = box; }

    /**
     * @brief Returns the current box rectangle.
     */
    const Box2d& box() const { return mBox; }

    /**
     * @brief Sets the fill color of the box.
     */
    void setColor(const Color& color) { mColor = color; }

    /**
     * @brief Returns the current fill color.
     */
    const Color& color() const { return mColor; }

    /**
     * @brief Sets whether the box is visible.
     */
    void setVisible(bool visible) { mVisible = visible; }

    /**
     * @brief Returns whether the box is visible.
     */
    bool isVisible() const { return mVisible; }

    /**
     * @brief Draws the box in the specified view if visible and initialized.
     *
     * @param[in] viewId: the bgfx view ID in which to render the overlay.
     */
    void draw(uint viewId)
    {
        if (!mInitialized || !mVisible || mBox.isNull())
            return;

        std::array<Point2f, 4> temp;
        temp[0] = vcl::boxVertex(mBox, 0).cast<float>();
        temp[1] = vcl::boxVertex(mBox, 1).cast<float>();
        temp[2] = vcl::boxVertex(mBox, 2).cast<float>();
        temp[3] = vcl::boxVertex(mBox, 3).cast<float>();

        bgfx::setState(
            0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_DEPTH_TEST_ALWAYS |
            BGFX_STATE_BLEND_ALPHA);
        mTriIndexBuf.bind();

        mPosBuf.update(0, bgfx::copy(&temp[0], 8 * sizeof(float)));
        mPosBuf.bind(VCL_MRB_VERTEX_POSITION_STREAM);

        ScreenSpaceBoxUniforms::setColor(mColor);
        ScreenSpaceBoxUniforms::bind();

        bgfx::submit(
            viewId,
            Context::instance()
                .programManager()
                .getProgram<VertFragProgram::SCREENSPACE_BOX>());
    }
};

} // namespace vcl

#endif // VCL_BGFX_SCREENSPACE_OVERLAY_SCREENSPACE_BOX_H
