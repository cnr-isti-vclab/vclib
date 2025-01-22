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

#ifndef VCL_BGFX_UNIFORM_H
#define VCL_BGFX_UNIFORM_H

#include <vclib/types.h>

#include <bgfx/bgfx.h>

#include <string>

namespace vcl {

/**
 * @brief The Uniform class wraps a bgfx::UniformHandle and provides a simple
 * interface to set the uniform data.
 *
 * It manages the lifetime of the bgfx::UniformHandle: each instance of this
 * class creates a new bgfx::UniformHandle and destroys it when the instance
 * goes out of scope.
 */
class Uniform
{
    bgfx::UniformHandle     mUniformHandle = BGFX_INVALID_HANDLE;
    std::string             mUniformName;
    bgfx::UniformType::Enum mUniformType = bgfx::UniformType::Count;

public:
    Uniform() = default;

    Uniform(const std::string& name, bgfx::UniformType::Enum type) :
            mUniformName(name), mUniformType(type)
    {
        mUniformHandle = bgfx::createUniform(name.c_str(), type);
    }

    Uniform(const Uniform& oth) :
            mUniformName(oth.mUniformName), mUniformType(oth.mUniformType)
    {
        mUniformHandle =
            bgfx::createUniform(mUniformName.c_str(), mUniformType);
    }

    Uniform(Uniform&& oth) { swap(oth); }

    ~Uniform()
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

    void swap(Uniform& oth)
    {
        using std::swap;
        swap(mUniformHandle, oth.mUniformHandle);
        swap(mUniformName, oth.mUniformName);
        swap(mUniformType, oth.mUniformType);
    }

    friend void swap(Uniform& a, Uniform& b) { a.swap(b); }

    Uniform& operator=(Uniform oth)
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

#endif // VCL_BGFX_UNIFORM_H
