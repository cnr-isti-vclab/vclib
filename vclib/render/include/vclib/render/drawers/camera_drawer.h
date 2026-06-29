// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_DRAWERS_CAMERA_DRAWER_H
#define VCL_RENDER_DRAWERS_CAMERA_DRAWER_H

#include "event_drawer.h"

#include <vclib/render/viewer/lights.h>

#include <vclib/algorithms/core.h>
#include <vclib/space/core.h>

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
