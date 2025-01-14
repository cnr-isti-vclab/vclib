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

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_CAMERA_UNIFORMS_H
#define VCL_BGFX_DRAWABLE_UNIFORMS_CAMERA_UNIFORMS_H

#include <vclib/bgfx/uniform.h>
#include <vclib/render/viewer/camera.h>

#include <bgfx/bgfx.h>

namespace vcl {

class CameraUniforms
{
    float mEye[4]     = {0.0, 0.0, 0.0, 0.0}; // eye[3] not used
    float mNearFar[4] = {0.0, 0.0, 0.0, 0.0}; // nearFar[2] and [3] not used

    Uniform mCameraEyeUniform =
        Uniform("u_cameraEyePosPack", bgfx::UniformType::Vec4);

    Uniform mCameraNearFarUniform =
        Uniform("u_cameraNearFarPack", bgfx::UniformType::Vec4);

public:
    CameraUniforms() {}

    template<typename S>
    void updateCamera(const vcl::Camera<S>& camera)
    {
        mEye[0] = camera.eye().x();
        mEye[1] = camera.eye().y();
        mEye[2] = camera.eye().z();

        mNearFar[0] = camera.nearPlane();
        mNearFar[1] = camera.farPlane();
    }

    void bind() const
    {
        mCameraEyeUniform.bind(mEye);
        mCameraNearFarUniform.bind(mNearFar);
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_CAMERA_UNIFORMS_H
