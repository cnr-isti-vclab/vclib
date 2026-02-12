/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#include <vclib/base.h>

#include <bgfx/bgfx.h>

#include <string>

namespace vcl {

/**
 * @brief The Uniform class wraps a bgfx::UniformHandle and provides a simple
 * interface to set the uniform data.
 *
 * It manages the lifetime of the bgfx::UniformHandle: each instance of this
 * class creates a new bgfx::UniformHandle and destroys it when the instance
 * goes out of scope. This simplifies resource management for shader uniforms.
 */
class Uniform
{
    bgfx::UniformHandle     mUniformHandle = BGFX_INVALID_HANDLE;
    std::string             mUniformName;
    bgfx::UniformType::Enum mUniformType = bgfx::UniformType::Count;
    uint16_t mArraySize = 1; // for array uniforms, the number of elements

public:
    /**
     * @brief Default constructor. Creates an invalid Uniform.
     */
    Uniform() = default;

    /**
     * @brief Creates a new shader uniform.
     * @param[in] name: The name of the uniform. It must match the name used in
     * the shader.
     * @param[in] type: The data type of the uniform.
     * @param[in] size: The number of elements for array uniforms (default is 1
     * for non-array uniforms).
     */
    Uniform(
        const std::string&      name,
        bgfx::UniformType::Enum type,
        uint16_t                size = 1) :
            mUniformName(name), mUniformType(type), mArraySize(size)
    {
        mUniformHandle = bgfx::createUniform(name.c_str(), type, size);
    }

    // Copying a Uniform is not allowed
    Uniform(const Uniform& oth) = delete;

    /**
     * @brief Move constructor.
     * @param[in] oth: The Uniform to move from.
     */
    Uniform(Uniform&& oth) { swap(oth); }

    /**
     * @brief Destructor. Destroys the underlying bgfx::UniformHandle.
     */
    ~Uniform()
    {
        if (bgfx::isValid(mUniformHandle))
            bgfx::destroy(mUniformHandle);
    }

    // Copying a Uniform is not allowed
    Uniform& operator=(const Uniform& oth) = delete;

    /**
     * @brief Move assignment operator.
     * @param[in] oth: The Uniform to move from.
     * @return A reference to this Uniform.
     */
    Uniform& operator=(Uniform&& oth)
    {
        swap(oth);
        return *this;
    }

    /**
     * @brief Swaps the content of this Uniform with another.
     * @param[in] oth: The other Uniform to swap with.
     */
    void swap(Uniform& oth)
    {
        using std::swap;
        swap(mUniformHandle, oth.mUniformHandle);
        swap(mUniformName, oth.mUniformName);
        swap(mUniformType, oth.mUniformType);
        swap(mArraySize, oth.mArraySize);
    }

    /**
     * @brief Swaps two Uniform objects.
     * @param[in] a: The first Uniform.
     * @param[in] b: The second Uniform.
     */
    friend void swap(Uniform& a, Uniform& b) { a.swap(b); }

    /**
     * @brief Checks if the Uniform is valid (i.e., if it has a valid
     * bgfx::UniformHandle).
     * @return true if the Uniform is valid, false otherwise.
     */
    bool isValid() const { return bgfx::isValid(mUniformHandle); }

    /**
     * @brief Returns the underlying bgfx::UniformHandle.
     * @return The handle to the uniform.
     */
    bgfx::UniformHandle handle() const { return mUniformHandle; }

    /**
     * @brief Returns the name of the uniform.
     * @return The uniform's name.
     */
    const std::string& name() const { return mUniformName; }

    /**
     * @brief Returns the type of the uniform.
     * @return The uniform's type.
     */
    bgfx::UniformType::Enum type() const { return mUniformType; }

    /**
     * @brief Returns the array size of the uniform (number of elements for
     * array uniforms, 1 for non-array uniforms).
     * @return The uniform's array size.
     */
    uint16_t arraySize() const { return mArraySize; }

    /**
     * @brief Sets the uniform data for the current shader program.
     *
     * This function should be called after a shader program has been set with
     * bgfx::submit(). The data pointer must point to a memory block that
     * matches the uniform's type and size.
     *
     * @param[in] data: A pointer to the uniform data.
     */
    void bind(const void* data) const
    {
        bgfx::setUniform(mUniformHandle, data, mArraySize);
    }

    /**
     * @brief Sets the uniform data for the current shader program, specifying
     * the number of elements for array uniforms.
     *
     * This function should be called after a shader program has been set with
     * bgfx::submit(). The data pointer must point to a memory block that
     * matches the uniform's type and size. The numElements parameter specifies
     * how many elements of the array uniform to set (for non-array uniforms,
     * this should be 1).
     *
     * @param[in] data: A pointer to the uniform data.
     * @param[in] numElements: The number of elements to set for array uniforms.
     */
    void bind(const void* data, uint16_t numElements) const
    {
        assert(numElements <= mArraySize);
        bgfx::setUniform(mUniformHandle, data, numElements);
    }
};

} // namespace vcl

#endif // VCL_BGFX_UNIFORM_H
