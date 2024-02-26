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

#ifndef VCL_EXT_BGFX_UNIFORMS_SHADER_UNIFORM_H
#define VCL_EXT_BGFX_UNIFORMS_SHADER_UNIFORM_H

#include <bgfx/bgfx.h>

#include <string>

namespace vcl::bgf {

class ShaderUniform
{
    bgfx::UniformHandle     uniformHandle = BGFX_INVALID_HANDLE;
    std::string             uniformName;
    bgfx::UniformType::Enum uniformType = bgfx::UniformType::Count;

public:
    ShaderUniform() = default;

    ShaderUniform(const std::string& name, bgfx::UniformType::Enum type) :
            uniformName(name), uniformType(type)
    {
        uniformHandle = bgfx::createUniform(name.c_str(), type);
    }

    ShaderUniform(const ShaderUniform& oth) :
            uniformName(oth.uniformName), uniformType(oth.uniformType)
    {
        uniformHandle = bgfx::createUniform(uniformName.c_str(), uniformType);
    }

    ShaderUniform(ShaderUniform&& oth) { swap(oth); }

    ~ShaderUniform()
    {
        if (bgfx::isValid(uniformHandle))
            bgfx::destroy(uniformHandle);
    }

    bgfx::UniformHandle handle() const { return uniformHandle; }

    const std::string& name() const { return uniformName; }

    bgfx::UniformType::Enum type() const { return uniformType; }

    void bind(const void* data) const { bgfx::setUniform(uniformHandle, data); }

    void swap(ShaderUniform& oth)
    {
        std::swap(uniformHandle, oth.uniformHandle);
        std::swap(uniformName, oth.uniformName);
        std::swap(uniformType, oth.uniformType);
    }

    ShaderUniform& operator=(ShaderUniform oth)
    {
        swap(oth);
        return *this;
    }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_UNIFORMS_SHADER_UNIFORM_H
