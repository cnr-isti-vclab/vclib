/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_RENDER_DRAWERS_CAMERA_DRAWER_H
#define VCL_RENDER_DRAWERS_CAMERA_DRAWER_H

#include "event_drawer.h"
#include <vclib/render/viewer/camera.h>
#include <vclib/render/viewer/lights.h>

namespace vcl {
template<typename Scalar, typename DerivedRenderApp>
class CameraDrawerT : public vcl::EventDrawer<DerivedRenderApp>
{
public:
    using ScalarType = Scalar;
    using CameraType = vcl::Camera<Scalar>;
    using PointType  = CameraType::PointType;
    using MatrixType = CameraType::MatrixType;
    using LightType  = vcl::DirectionalLight<Scalar>;

protected:
    CameraType mCamera;

public:
    using Base = vcl::EventDrawer<DerivedRenderApp>;

    CameraDrawerT(uint width = 100, uint height = 768) : Base(width, height)
    {
        onResize(width, height);
    }

    MatrixType viewMatrix() const { return mCamera.viewMatrix(); }

    MatrixType projectionMatrix() const { return mCamera.projectionMatrix(); }

    const CameraType& camera() const { return mCamera; }

    LightType light() const { return LightType(); }

    void reset() { mCamera.reset(); }

    void focus(const PointType& p) { mCamera.center() = p; }

    void fitScene(const PointType& p, Scalar s)
    {
        mCamera.center()         = p;
        mCamera.eye()            = p + PointType(0, 0, 1);
        mCamera.verticalHeight() = s;
        mCamera.setFieldOfViewAdaptingEyeDistance(mCamera.fieldOfView());
    }

    void onResize(uint width, uint height) override
    {
        mCamera.aspectRatio() = Scalar(double(width) / height);
    }
};

template<typename DerivedRenderApp>
using CameraDrawer = CameraDrawerT<float, DerivedRenderApp>;

} // namespace vcl

#endif // VCL_RENDER_DRAWERS_CAMERA_DRAWER_H
