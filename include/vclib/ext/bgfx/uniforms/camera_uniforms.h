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
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_EXT_BGFX_UNIFORMS_CAMERA_UNIFORMS_H
#define VCL_EXT_BGFX_UNIFORMS_CAMERA_UNIFORMS_H

#include <vclib/render/camera.h>

#include <bgfx/bgfx.h>

#include "shader_uniform.h"

namespace vcl::bgf {

class CameraUniforms
{
    float eye[4] = {0.0, 0.0, 0.0, 0.0};

    ShaderUniform cameraEyeUH = ShaderUniform("cameraEyePos", bgfx::UniformType::Vec4);

public:
    CameraUniforms() {}

    template<typename S>
    void updateCamera(const vcl::Camera<S>& camera)
    {
        eye[0] = camera.eye().x();
        eye[1] = camera.eye().y();
        eye[2] = camera.eye().z();
    }

    void setUniforms() const
    {
        bgfx::setUniform(cameraEyeUH.handle(), eye);
    }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_UNIFORMS_CAMERA_UNIFORMS_H
