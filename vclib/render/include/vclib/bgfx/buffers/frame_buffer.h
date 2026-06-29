// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_BUFFERS_FRAME_BUFFER_H
#define VCL_BGFX_BUFFERS_FRAME_BUFFER_H

#include "generic_buffer.h"

#include <vclib/bgfx/context.h>

#include <vector>

namespace vcl {

/**
 * @brief The FrameBuffer class manages the lifetime of a
 * bgfx::FrameBufferHandle.
 *
 * It provides an interface to create framebuffers for offscreen rendering,
 * either with simple color+depth attachments or with multiple render targets
 * (MRT).
 *
 * @note A FrameBuffer can be moved but not copied (a copy would require
 * creating a new bgfx::FrameBufferHandle, which can only be done with explicit
 * texture handles). Any class that contains a FrameBuffer should implement the
 * copy constructor and copy assignment operator if copying is needed.
 */
class FrameBuffer : public GenericBuffer<bgfx::FrameBufferHandle>
{
    using Base = GenericBuffer<bgfx::FrameBufferHandle>;

    std::vector<bgfx::TextureHandle> mOwnedTextures;

public:
    /**
     * @brief Empty constructor.
     *
     * It creates an invalid FrameBuffer object.
     */
    FrameBuffer() = default;

    /**
     * @brief Destructor.
     *
     * Destroys the framebuffer and any owned textures.
     */
    ~FrameBuffer() { destroy(); }

    /**
     * @brief Move constructor.
     *
     * The other FrameBuffer is left in an invalid state.
     *
     * @param[in] other: the other FrameBuffer object.
     */
    FrameBuffer(FrameBuffer&& other) noexcept { swap(other); }

    /**
     * @brief Move assignment operator.
     *
     * The other FrameBuffer is left in an invalid state.
     *
     * @param[in] other: the other FrameBuffer object.
     * @return a reference to this object.
     */
    FrameBuffer& operator=(FrameBuffer&& other) noexcept
    {
        swap(other);
        return *this;
    }

    /**
     * @brief Swap the content of this object with another FrameBuffer object.
     *
     * @param[in] other: the other FrameBuffer object.
     */
    void swap(FrameBuffer& other)
    {
        using std::swap;
        Base::swap(other);
        swap(mOwnedTextures, other.mOwnedTextures);
    }

    friend void swap(FrameBuffer& a, FrameBuffer& b) { a.swap(b); }

    /**
     * @brief Destroy the framebuffer and any owned textures.
     */
    void destroy()
    {
        // Destroy owned textures first
        for (auto& tex : mOwnedTextures) {
            if (bgfx::isValid(tex)) {
                bgfx::destroy(tex);
            }
        }
        mOwnedTextures.clear();

        // Destroy the framebuffer
        Base::destroy();
    }

    /**
     * @brief Creates a simple offscreen framebuffer with color and depth
     * attachments.
     *
     * If the framebuffer is already created (@ref isValid() returns `true`),
     * it is destroyed and a new one is created.
     *
     * @param[in] width: The width of the framebuffer.
     * @param[in] height: The height of the framebuffer.
     * @param[in] colorFormat: The format of the color attachment.
     * @param[in] depthFormat: The format of the depth attachment.
     */
    void create(
        uint16_t                  width,
        uint16_t                  height,
        bgfx::TextureFormat::Enum colorFormat = Context::DEFAULT_COLOR_FORMAT,
        bgfx::TextureFormat::Enum depthFormat = Context::DEFAULT_DEPTH_FORMAT)
    {
        destroy();

        if (width > 0 && height > 0) {
            mHandle = Context::instance().createOffscreenFramebuffer(
                width, height, colorFormat, depthFormat);
        }
    }

    /**
     * @brief Creates a framebuffer from an array of texture handles.
     *
     * If the framebuffer is already created (@ref isValid() returns `true`),
     * it is destroyed and a new one is created.
     *
     * This method is useful for creating framebuffers with multiple render
     * targets (MRT).
     *
     * @param[in] textureHandles: An array of texture handles to attach to the
     * framebuffer.
     * @param[in] count: The number of texture handles in the array.
     * @param[in] destroyTextures: If true, the textures will be owned by the
     * framebuffer and automatically destroyed when the framebuffer is
     * destroyed. If false, the caller retains ownership of the textures.
     */
    void create(
        bgfx::TextureHandle* textureHandles,
        uint8_t              count,
        bool                 destroyTextures = false)
    {
        destroy();

        if (count > 0) {
            mHandle =
                bgfx::createFrameBuffer(count, textureHandles, destroyTextures);

            // If we're not destroying textures via bgfx
            // (destroyTextures=false), but the caller wants us to manage them,
            // store them Note: bgfx's destroyTextures parameter handles
            // destruction automatically when set to true, so we only track if
            // bgfx will destroy them
            if (destroyTextures) {
                // bgfx will destroy the textures, so we don't need to track
                // them
                mOwnedTextures.clear();
            }
        }
    }

    /**
     * @brief Get the raw bgfx framebuffer handle.
     *
     * @return The bgfx::FrameBufferHandle.
     */
    bgfx::FrameBufferHandle handle() const { return mHandle; }

    /**
     * @brief Implicit conversion to bgfx::FrameBufferHandle for seamless
     * integration with bgfx API.
     *
     * @return The bgfx::FrameBufferHandle.
     */
    operator bgfx::FrameBufferHandle() const { return mHandle; }

    /**
     * @brief Get a texture handle for a specific attachment.
     *
     * @param[in] attachment: The attachment index (0 for first color
     * attachment, etc.).
     * @return The texture handle for the specified attachment.
     */
    bgfx::TextureHandle getTexture(uint8_t attachment = 0) const
    {
        if (bgfx::isValid(mHandle)) {
            return bgfx::getTexture(mHandle, attachment);
        }
        return BGFX_INVALID_HANDLE;
    }
};

} // namespace vcl

#endif // VCL_BGFX_BUFFERS_FRAME_BUFFER_H
