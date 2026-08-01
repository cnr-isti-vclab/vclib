// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_OPENGL2_GIZMOS_DIRECTIONAL_LIGHT_GIZMO_OPENGL2_H
#define VCL_OPENGL2_GIZMOS_DIRECTIONAL_LIGHT_GIZMO_OPENGL2_H

#include <vclib/render/gizmos/abstract_gizmo.h>
#include <vclib/render/viewer/lights/directional_light.h>

#include <vclib/space/core.h>

namespace vcl {

/**
 * @brief The DirectionalLightGizmoOpenGL2 class is a visual gizmo that
 * renders a directional light using the OpenGL2 backend.
 *
 * It draws a set of lines representing the light rays.
 */
class DirectionalLightGizmoOpenGL2 : public AbstractGizmo
{
    bool mVisible = false;

    Matrix44f mTransform = vcl::Matrix44f::Identity();

    std::vector<float> mVertices;
    vcl::Color         mColor = vcl::Color::Yellow;

public:
    DirectionalLightGizmoOpenGL2();

    void updateRotation(const vcl::Matrix44f& rot);

    const vcl::Color& linesColor() const { return mColor; }

    void setLinesColor(const vcl::Color& c);

    // AbstractGizmo interface
    void init() override {}

    void draw(uint viewId = 0) override;

    bool isVisible() const override;

    void setVisibility(bool vis) override;
};

} // namespace vcl

#endif // VCL_OPENGL2_GIZMOS_DIRECTIONAL_LIGHT_GIZMO_OPENGL2_H
