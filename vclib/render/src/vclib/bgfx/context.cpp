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

#include <vclib/bgfx/context.h>

#include <vclib/bgfx/system/native_window_handle.h>
#include <vclib/types/base.h>

#include <iostream>

namespace vcl {

Context*   Context::sInstancePtr = nullptr;
std::mutex Context::sMutex;

void Context::init(void* windowHandle, void* displayHandle)
{
    std::lock_guard<std::mutex> lock(sMutex);
    if (sInstancePtr == nullptr) {
        sInstancePtr = new Context(windowHandle, displayHandle);
    }
}

void Context::initHeadless()
{
    init(nullptr, nullptr);
}

bool Context::isInitialized()
{
    return sInstancePtr != nullptr;
}

void Context::shutdown()
{
    std::lock_guard<std::mutex> lock(sMutex);
    if (sInstancePtr != nullptr) {
        delete sInstancePtr;
        sInstancePtr = nullptr;
    }
}

/**
 * @brief Return the backend renderer type used by bgfx.
 *
 * This function can be called before the context is initialized or after.
 * If called before, it returns the default renderer type (or the one set by
 * calling setRenderType). If called after, it returns the renderer type used
 * by bgfx.
 */
bgfx::RendererType::Enum Context::renderType()
{
    assert(
        !isInitialized() ||
        (instance().capabilites().rendererType == sRenderType));
    return sRenderType;
}

/**
 * @brief Set the backend renderer type used by bgfx.
 *
 * This function can be called before the context is initialized, to set the
 * type of renderer used by bgfx.
 *
 * Depending on the platform, some renderer types could not be available. In
 * such cases, the renderer type is set to the default one.
 *
 * @warning This function must be called before the context is initialized.
 * Otherwise, it throws an exception.
 *
 * @param[in] renderType: the renderer type to set.
 */
void Context::setRenderType(bgfx::RendererType::Enum renderType)
{
    if (isInitialized()) {
        throw std::runtime_error(
            "Cannot change render type after the context has been initialized");
    }
    sRenderType = renderType;
}

void Context::setDebugVerbosity(bool verbose)
{
    instance().mCallBack.setDebugVerbosity(verbose);
}

bool Context::isHeadless() const
{
    return mWindowHandle == nullptr;
}

/**
 * @brief Return the capabilities of the backend renderer.
 */
const bgfx::Caps& Context::capabilites() const
{
    return *bgfx::getCaps();
}

bool Context::supportsReadback() const
{
    return (capabilites().supported &
            (BGFX_CAPS_TEXTURE_BLIT | BGFX_CAPS_TEXTURE_READ_BACK)) ==
           (BGFX_CAPS_TEXTURE_BLIT | BGFX_CAPS_TEXTURE_READ_BACK);
}

bgfx::ViewId Context::requestViewId()
{
    std::lock_guard<std::mutex> lock(sMutex);
    bgfx::ViewId                viewId = mViewStack.top();
    mViewStack.pop();
    return viewId;
}

void Context::releaseViewId(bgfx::ViewId viewId)
{
    std::lock_guard<std::mutex> lock(sMutex);
    instance().mViewStack.push(viewId);
}

bool Context::isDefaultWindow(void* windowHandle) const
{
    return mWindowHandle == windowHandle;
}

bool Context::isValidViewId(bgfx::ViewId viewId) const
{
    return viewId <= capabilites().limits.maxViews;
}

static const uint64_t kRenderBufferflags =
    0 | BGFX_TEXTURE_RT | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT |
    BGFX_SAMPLER_MIP_POINT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP;

bgfx::FrameBufferHandle Context::createFramebuffer(
    uint16_t                  width,
    uint16_t                  height,
    bgfx::TextureFormat::Enum colorFormat,
    bgfx::TextureFormat::Enum depthFormat)
{
    bgfx::FrameBufferHandle fbh = BGFX_INVALID_HANDLE;

    // create offscreen framebuffer with explicit textures
    bgfx::TextureHandle fbtextures[2];
    fbtextures[0] = bgfx::createTexture2D(
        uint16_t(width),
        uint16_t(height),
        false,
        1,
        colorFormat,
        kRenderBufferflags);

    fbtextures[1] = bgfx::createTexture2D(
        uint16_t(width),
        uint16_t(height),
        false,
        1,
        depthFormat,
        kRenderBufferflags);

    assert(bgfx::isValid(fbtextures[0]));
    assert(bgfx::isValid(fbtextures[1]));
    // textures are automatically destroyed along the framebuffer
    fbh = bgfx::createFrameBuffer(2, fbtextures, true);
    assert(bgfx::isValid(fbh));

    return fbh;
}

void Context::resetDefaultFramebuffer(
    uint16_t                  width,
    uint16_t                  height,
    bgfx::TextureFormat::Enum colorFormat)
{
    // TODO: manage the reset flags elsewhere
    bgfx::reset(width, height, BGFX_RESET_VSYNC, colorFormat);
}

static const uint32_t kDefaultClearColor   = 0xffffffff;
static const float    kDefaultClearDepth   = 1.0f;
static const uint8_t  kDefaultClearStencil = 0;

bgfx::FrameBufferHandle Context::createFramebufferAndInitView(
    void*                     winId,
    bgfx::ViewId              view,
    uint16_t                  width,
    uint16_t                  height,
    bool                      clear,
    bgfx::TextureFormat::Enum colorFormat,
    bgfx::TextureFormat::Enum depthFormat)
{
    const bool offscreen = (winId == nullptr);
    const bool defaultWindow =
        (mWindowHandle != nullptr) && (winId == mWindowHandle);

    // if the context is headless, the framebuffer is always offscreen
    assert(!isHeadless() || offscreen);
    // TODO: eventually test the behavior with a headless context

    bgfx::FrameBufferHandle fbh = BGFX_INVALID_HANDLE;
    if (defaultWindow) {
        assert(!isHeadless());

        // create onscreen framebuffer
        resetDefaultFramebuffer(width, height, colorFormat);
    }
    else {
        // create framebuffer
        fbh = createFramebuffer(width, height, colorFormat, depthFormat);
    }
    // set view on framebuffer even if it must be done every frame
    bgfx::setViewFrameBuffer(view, fbh);

    if (clear) {
        bgfx::setViewClear(
            view,
            BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH | BGFX_CLEAR_STENCIL,
            kDefaultClearColor,
            kDefaultClearDepth,
            kDefaultClearStencil);
    }

    bgfx::setViewRect(view, 0, 0, width, height);
    bgfx::touch(view);

    return fbh;
}

bgfx::FrameBufferHandle Context::createOffscreenFramebufferAndInitView(
    bgfx::ViewId              view,
    uint16_t                  width,
    uint16_t                  height,
    bool                      clear,
    bgfx::TextureFormat::Enum colorFormat,
    bgfx::TextureFormat::Enum depthFormat)
{
    return createFramebufferAndInitView(
        nullptr, view, width, height, clear, colorFormat, depthFormat);
}

FontManager& Context::fontManager()
{
    return *mFontManager;
}

ProgramManager& Context::programManager()
{
    return *mProgramManager;
}

Context::Context(void* windowHandle, void* displayHandle)
{
    static const bool forceWindow = false;

    if (windowHandle == nullptr) {
        // Headless context
        if (forceWindow) {
            std::cerr << "WARNING: The first window used to create the bgfx "
                         "context is a dummy window. This is not recommended."
                      << std::endl;
            std::cerr
                << "Be sure to pass a valid window handle when requesting the "
                   "context instance for the first time."
                << std::endl;
            mWindowHandle = vcl::createWindow("", 1, 1, mDisplayHandle, true);
        }
    }
    else {
#ifdef __linux__
        assert(displayHandle != nullptr);
#endif
        mWindowHandle  = windowHandle;
        mDisplayHandle = displayHandle;
    }
#ifdef __APPLE__
    if (!isHeadless()) {
        bgfx::renderFrame(); // needed for macos
    }
#endif // __APPLE__

    bgfx::Init init;
    init.platformData.nwh  = mWindowHandle;
    init.type              = sRenderType;
    init.platformData.ndt  = mDisplayHandle;
    init.resolution.width  = 1;
    init.resolution.height = 1;
    init.resolution.reset  = BGFX_RESET_VSYNC;
    init.callback          = &mCallBack;
    bgfx::init(init);

    if (windowHandle == nullptr) {
        vcl::closeWindow(mWindowHandle, mDisplayHandle);
    }

    uint mv = bgfx::getCaps()->limits.maxViews;

    while (mv != 0) {
        mViewStack.push((bgfx::ViewId) mv--);
    }
    mViewStack.push((bgfx::ViewId) 0);

    // font manager must be created after bgfx::init
    mFontManager    = new FontManager();
    mProgramManager = new ProgramManager(bgfx::getCaps()->rendererType);

    sRenderType = bgfx::getCaps()->rendererType;
}

Context::~Context()
{
    delete mFontManager;
    delete mProgramManager;
    bgfx::shutdown();
}

Context& Context::instance(void* windowHandle, void* displayHandle)
{
    if (sInstancePtr == nullptr) {
        Context::init(windowHandle, displayHandle);
    }
    return *sInstancePtr;
}

} // namespace vcl
