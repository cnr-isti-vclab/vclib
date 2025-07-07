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

#ifndef VCL_BGFX_CONTEXT_H
#define VCL_BGFX_CONTEXT_H

#include "context/callback.h"
#include "context/font_manager.h"
#include "context/program_manager.h"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <mutex>
#include <stack>

#define BGFX_INVALID_VIEW 65535

namespace vcl {

class Context
{
    // when initialization is done with a window handle
    // this will become the main window handle (withe the *special* framebuffer)
    // otherwise (null window handle) means that the context is headless (no UI)
    void* mWindowHandle  = nullptr;
    void* mDisplayHandle = nullptr;

    std::stack<bgfx::ViewId> mViewStack;

    Callback        mCallBack;
    FontManager*    mFontManager    = nullptr;
    ProgramManager* mProgramManager = nullptr;

    inline static bgfx::RendererType::Enum sRenderType =
        bgfx::RendererType::Vulkan;

    inline static uint sResetFlags = BGFX_RESET_VSYNC;

    // singleton
    inline static Context*   sInstancePtr = nullptr;
    inline static std::mutex sMutex;

public:
    // default values, used for optional parameters
    static constexpr uint32_t                  DEFAULT_CLEAR_COLOR = 0x000000ff;
    static constexpr float                     DEFAULT_CLEAR_DEPTH = 1.0f;
    static constexpr uint8_t                   DEFAULT_CLEAR_STENCIL = 0;
    static constexpr bgfx::TextureFormat::Enum DEFAULT_COLOR_FORMAT =
        bgfx::TextureFormat::RGBA8;
    static constexpr bgfx::TextureFormat::Enum DEFAULT_DEPTH_FORMAT =
        bgfx::TextureFormat::D24S8;

    /**
     * @brief Return the context instance.
     *
     * @param[in] windowHandle: The window handle.
     * @param[in] displayHandle: The display handle.
     *
     * @return The context instance.
     */
    static Context& instance(
        void* windowHandle  = nullptr,
        void* displayHandle = nullptr);

    static void init(
        void* windowHandle  = nullptr,
        void* displayHandle = nullptr);

    static void initHeadless();

    static bool isInitialized();

    static void shutdown();

    static bgfx::RendererType::Enum renderType();

    static void setRenderType(bgfx::RendererType::Enum renderType);

    static void setResetFlags(uint flags);

    static void setDebugVerbosity(bool verbose);

    bool isHeadless() const;

    const bgfx::Caps& capabilites() const;

    bool supportsReadback() const;

    bool supportsCompute() const;

    /**
     * @brief Checks whether the context is initialized with the provided
     *  window handle.
     *
     * @param[in] windowHandle: The window handle to check.
     */
    bool isDefaultWindow(void* windowHandle) const;

    bool isValidViewId(bgfx::ViewId viewId) const;

    bgfx::ViewId requestViewId();

    void releaseViewId(bgfx::ViewId viewId);

    /**
     * @brief Create a framebuffer with with 2 attachments (color and depth)
     *
     * @param[in] width: The width of the framebuffer.
     * @param[in] height: The height of the framebuffer.
     * @param[in] colorFormat: The format of the color attachment.
     * @param[in] depthFormat: The format of the depth attachment.
     *
     * @return The handle of the created framebuffer.
     */
    bgfx::FrameBufferHandle createOffscreenFramebuffer(
        uint16_t                  width,
        uint16_t                  height,
        bgfx::TextureFormat::Enum colorFormat = DEFAULT_COLOR_FORMAT,
        bgfx::TextureFormat::Enum depthFormat = DEFAULT_DEPTH_FORMAT);

    void resetDefaultFramebuffer(
        uint16_t                  width,
        uint16_t                  height,
        bgfx::TextureFormat::Enum colorFormat = DEFAULT_COLOR_FORMAT);

    bgfx::FrameBufferHandle createFramebufferAndInitView(
        void*                     winId,
        bgfx::ViewId              view,
        uint16_t                  width,
        uint16_t                  height,
        bool                      clear        = false,
        uint32_t                  clearColor   = DEFAULT_CLEAR_COLOR,
        float                     clearDepth   = DEFAULT_CLEAR_DEPTH,
        uint8_t                   clearStencil = DEFAULT_CLEAR_STENCIL,
        bgfx::TextureFormat::Enum colorFormat  = DEFAULT_COLOR_FORMAT,
        bgfx::TextureFormat::Enum depthFormat  = DEFAULT_DEPTH_FORMAT);

    bgfx::FrameBufferHandle createOffscreenFramebufferAndInitView(
        bgfx::ViewId              view,
        uint16_t                  width,
        uint16_t                  height,
        bool                      clear        = false,
        uint32_t                  clearColor   = DEFAULT_CLEAR_COLOR,
        float                     clearDepth   = DEFAULT_CLEAR_DEPTH,
        uint8_t                   clearStencil = DEFAULT_CLEAR_STENCIL,
        bgfx::TextureFormat::Enum colorFormat  = DEFAULT_COLOR_FORMAT,
        bgfx::TextureFormat::Enum depthFormat  = DEFAULT_DEPTH_FORMAT);

    FontManager& fontManager();

    ProgramManager& programManager();

private:
    Context(void* windowHandle, void* displayHandle);

    ~Context();

public:
    Context(const Context&)            = delete;
    Context& operator=(const Context&) = delete;
    Context(Context&&)                 = delete;
    Context& operator=(Context&&)      = delete;
};

} // namespace vcl

#endif // VCL_BGFX_CONTEXT_H
