// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_OPENGL2_DRAWABLE_DRAWABLE_TRACKBALL_OPENGL2_H
#define VCL_OPENGL2_DRAWABLE_DRAWABLE_TRACKBALL_OPENGL2_H

#include <vclib/algorithms/core.h>
#include <vclib/render/drawable/drawable_object.h>
#include <vclib/space/core.h>

namespace vcl {

class DrawableTrackBallOpenGL2 : public DrawableObject
{
    bool mVisible = true;

    vcl::Matrix44f mTransform  = vcl::Matrix44f::Identity();
    bool           mIsDragging = false;

public:
    DrawableTrackBallOpenGL2() = default;

    void updateDragging(bool isDragging);

    void setTransform(const vcl::Matrix44f& mtx);

    // DrawableObject interface

    void draw(const DrawObjectSettings& settings) override;

    Box3d boundingBox() const override;

    std::shared_ptr<DrawableObject> clone() const& override;

    std::shared_ptr<DrawableObject> clone() && override;

    bool isVisible() const override;

    void setVisibility(bool vis) override;
};

} // namespace vcl

#endif // VCL_OPENGL2_DRAWABLE_DRAWABLE_TRACKBALL_OPENGL2_H
