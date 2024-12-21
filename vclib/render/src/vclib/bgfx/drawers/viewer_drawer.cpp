/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#include <vclib/bgfx/drawers/viewer_drawer.h>

#include <vclib/bgfx/read_framebuffer_request.h>
#include <vclib/math/min_max.h>
#include <vclib/render/drawable/drawable_mesh.h>
#include <vclib/space/core/box.h>

namespace vcl {

ViewerDrawerBGFX::ViewerDrawerBGFX(
    uint  width,
    uint  height) :
        AbstractViewer(width, height)
{
    mCameraUniforms.updateCamera(DTB::camera());
    mDirectionalLightUniforms.updateLight(DTB::light());
}

ViewerDrawerBGFX::ViewerDrawerBGFX(
    const std::shared_ptr<DrawableObjectVector>& v,
    uint                                         width,
    uint                                         height) : ViewerDrawerBGFX(width, height)
{
    setDrawableObjectVector(v);
}

void ViewerDrawerBGFX::onDraw(uint viewId)
{
    onDrawContent(viewId);

    if (mAxis.isVisible()) {
        mAxis.draw(viewId);
    }

    if (mDirectionalLight.isVisible()) {
        mDirectionalLight.draw(viewId);
    }

    if (mDrawTrackBall.isVisible()) {
        mDrawTrackBall.draw(viewId);
    }
}

void ViewerDrawerBGFX::onDrawContent(uint viewId)
{
    setDirectionalLightVisibility(
        currentMotion() == DTB::TrackBallType::DIR_LIGHT_ARC);
    updateDirectionalLight();
    updateDrawableTrackball();

    bgfx::setViewTransform(
        viewId, viewMatrix().data(), projectionMatrix().data());

    mCameraUniforms.updateCamera(DTB::camera());
    mCameraUniforms.bind();

    mDirectionalLightUniforms.bind();

    for (auto obj : drawableObjectVector())
        obj->draw(viewId);
}

// need access to canvas here
// void ViewerCanvasBGFX::onMouseDoubleClick(
//     MouseButton::Enum   button,
//     double              x,
//     double              y,
//     const KeyModifiers& modifiers)
// {
//     using ReadFramebufferRequest = detail::ReadFramebufferRequest;
//     using CallbackReadBuffer     = ReadFramebufferRequest::CallbackReadBuffer;
//     using ReadData               = ReadFramebufferRequest::ReadData;

//     // FIXME: code duplication for both OpenGL2 and BGFX
//     if (mReadRequested)
//         return;

//     // get point
//     const Point2d p(x, y);

//     // get the homogeneous NDC flag
//     const bool homogeneousNDC =
//         Context::instance().capabilites().homogeneousDepth;

//     // matrices
//     const auto proj = projectionMatrix();
//     const auto view = viewMatrix();
//     // viewport
//     const Point4f vp = {.0f, .0f, float(size().x()), float(size().y())};
//     // create the callback
//     auto callback = [=, this](const ReadData& data) {
//         assert(std::holds_alternative<std::vector<float>>(data));
//         const auto& d  = std::get<std::vector<float>>(data);
//         mReadRequested = false;

//         // if the depth is 1.0, the point is not in the scene
//         const float depth = d[0];
//         if (depth == 1.0f) {
//             return;
//         }

//         // unproject the point
//         const Point3f p2d(p.x(), vp[3] - p.y(), depth);
//         const auto    unproj = unproject(
//             p2d, Matrix44<ScalarType>(proj * view), vp, homogeneousNDC);

//         this->focus(unproj);
//         // this->update(); TODO
//     };

//     mReadRequested = this->readDepth(Point2i(p.x(), p.y()), callback);
//     if (mReadRequested)
//         update();
// }

} // namespace vcl
