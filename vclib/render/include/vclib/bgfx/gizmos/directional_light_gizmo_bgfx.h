// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_GIZMOS_DIRECTIONAL_LIGHT_GIZMO_BGFX_H
#define VCL_BGFX_GIZMOS_DIRECTIONAL_LIGHT_GIZMO_BGFX_H

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>
#include <vclib/render/gizmos/abstract_gizmo.h>

#include <vclib/space/core.h>

namespace vcl {

/**
 * @brief The DirectionalLightGizmoBGFX class is a visual gizmo that
 * renders a directional light using the BGFX backend.
 *
 * It draws a set of lines representing the light rays.
 */
class DirectionalLightGizmoBGFX : public AbstractGizmo
{
    bool mVisible = false;

    vcl::Matrix44f mTransform = vcl::Matrix44f::Identity();

    std::vector<float> mVertices; // vertices of the drawn lines
    vcl::Color         mColor = vcl::Color::Yellow; // color of the lines

    VertexBuffer mVertexPosBuffer;

public:
    DirectionalLightGizmoBGFX();

    // default move constructor - buffers can be moved
    DirectionalLightGizmoBGFX(DirectionalLightGizmoBGFX&& other) = default;

    // default destructor - buffers are destroyed by their destructor
    ~DirectionalLightGizmoBGFX() = default;

    /**
     * @brief Swap the content of this object with another
     * DirectionalLightGizmoBGFX object.
     *
     * @param[in] other: the other DirectionalLightGizmoBGFX object.
     */
    void swap(DirectionalLightGizmoBGFX& other);

    friend void swap(DirectionalLightGizmoBGFX& a, DirectionalLightGizmoBGFX& b)
    {
        a.swap(b);
    }

    void updateRotation(const vcl::Matrix44f& rot);

    const vcl::Color& linesColor() const { return mColor; }

    void setLinesColor(const vcl::Color& c);

    // AbstractGizmo interface

    void init() override {}

    void draw(uint viewId = 0) override;

    bool isVisible() const override { return mVisible; }

    void setVisibility(bool vis) override { mVisible = vis; }

private:
    void createVertexBuffer();
};

} // namespace vcl

#endif // VCL_BGFX_GIZMOS_DIRECTIONAL_LIGHT_GIZMO_BGFX_H
