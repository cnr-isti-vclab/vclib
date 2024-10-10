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

#ifndef VCL_BGFX_RENDER_DRAWABLE_UNIFORMS_SHADER_UNIFORM_H
#define VCL_BGFX_RENDER_DRAWABLE_UNIFORMS_SHADER_UNIFORM_H

#include <bgfx/bgfx.h>

#include <string>

namespace vcl {

class ShaderUniform
{
    bgfx::UniformHandle     mUniformHandle = BGFX_INVALID_HANDLE;
    std::string             mUniformName;
    bgfx::UniformType::Enum mUniformType = bgfx::UniformType::Count;

public:
    ShaderUniform() = default;

    ShaderUniform(const std::string& name, bgfx::UniformType::Enum type) :
            mUniformName(name), mUniformType(type)
    {
        mUniformHandle = bgfx::createUniform(name.c_str(), type);
    }

    ShaderUniform(const ShaderUniform& oth) :
            mUniformName(oth.mUniformName), mUniformType(oth.mUniformType)
    {
        mUniformHandle =
            bgfx::createUniform(mUniformName.c_str(), mUniformType);
    }

    ShaderUniform(ShaderUniform&& oth) { swap(oth); }

    ~ShaderUniform()
    {
        if (bgfx::isValid(mUniformHandle))
            bgfx::destroy(mUniformHandle);
    }

    bgfx::UniformHandle handle() const { return mUniformHandle; }

    const std::string& name() const { return mUniformName; }

    bgfx::UniformType::Enum type() const { return mUniformType; }

    void bind(const void* data) const
    {
        bgfx::setUniform(mUniformHandle, data);
    }

    void swap(ShaderUniform& oth)
    {
        std::swap(mUniformHandle, oth.mUniformHandle);
        std::swap(mUniformName, oth.mUniformName);
        std::swap(mUniformType, oth.mUniformType);
    }

    ShaderUniform& operator=(ShaderUniform oth)
    {
        swap(oth);
        return *this;
    }

    static float uintBitsToFloat(uint bits)
    {
        union
        {
            uint  i;
            float f;
        } u;

        u.i = bits;
        return u.f;
    }
};

} // namespace vcl

#endif // VCL_BGFX_RENDER_DRAWABLE_UNIFORMS_SHADER_UNIFORM_H
