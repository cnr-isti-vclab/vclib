// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_STATIC_UNIFORM_H
#define VCL_BGFX_STATIC_UNIFORM_H

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/uniform.h>

#include <string>

namespace vcl {

/**
 * @brief The StaticUniform class wraps a vcl::Uniform for static storage.
 *
 * It manages lazy initialization and automatic registration with the Context.
 * This guarantees that the uniform is destroyed safely before bgfx::shutdown()
 * is called, preventing leaks or crashes associated with static resource destruction.
 *
 * @warning This class should ONLY be instantiated as a static or global variable.
 *          If instantiated locally on the stack, it will lead to dangling references
 *          in the Context since it registers a reference to itself.
 *          Additionally, the get() initialization is not thread-safe and should
 *          be called from the main thread (or externally synchronized).
 */
class StaticUniform
{
    vcl::Uniform            mUniform;
    std::string             mName;
    bgfx::UniformType::Enum mType;
    uint16_t                mSize;

public:
    /**
     * @brief Creates a static uniform wrapper.
     * @param[in] name: The name of the uniform.
     * @param[in] type: The data type of the uniform.
     * @param[in] size: The number of elements for array uniforms.
     */
    StaticUniform(
        std::string             name,
        bgfx::UniformType::Enum type,
        uint16_t                size = 1) :
            mName(std::move(name)), mType(type), mSize(size)
    {
    }

    // Copying and moving a StaticUniform is not allowed to ensure that the
    // reference held by Context is always valid.
    StaticUniform(const StaticUniform&)            = delete;
    StaticUniform& operator=(const StaticUniform&) = delete;

    /**
     * @brief Returns the underlying vcl::Uniform, initializing it if necessary.
     * @return A reference to the initialized uniform.
     */
    const vcl::Uniform& get()
    {
        if (!mUniform.isValid()) {
            mUniform = vcl::Uniform(mName, mType, mSize);
            Context::instance().registerStaticUniform(mUniform);
        }
        return mUniform;
    }

    /**
     * @brief Checks if the underlying uniform has been initialized.
     * @return true if initialized, false otherwise.
     */
    bool isValid() const { return mUniform.isValid(); }

    /**
     * @brief Sets the uniform data for the current shader program.
     * @param[in] data: A pointer to the uniform data.
     */
    void bind(const void* data)
    {
        get().bind(data);
    }

    /**
     * @brief Sets the uniform data for the current shader program, specifying
     * the number of elements for array uniforms.
     * @param[in] data: A pointer to the uniform data.
     * @param[in] numElements: The number of elements to set.
     */
    void bind(const void* data, uint16_t numElements)
    {
        get().bind(data, numElements);
    }
};

} // namespace vcl

#endif // VCL_BGFX_STATIC_UNIFORM_H
