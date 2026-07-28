// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_HEADLESS_MANAGER_H
#define VCL_RENDER_HEADLESS_MANAGER_H

#include <vclib/render/concepts/render_app.h>
#include <vclib/render/window_managers.h>

#include <vclib/space/core.h>

#include <string>

namespace vcl {

template<typename DerivedRenderApp>
class HeadlessManager
{
    std::string mTitle;
    uint        mWidth;
    uint        mHeight;
    bool        mUpdateRequested = false;

public:
    /**
     * @brief The ParentType is the type of the parent class. It is used to
     * initialize the base class (if any). In the HeadlessManager, the
     * parent class is void and it is not managed.
     */
    using ParentType = void;

    /**
     * @brief The WINDOW_MANAGER_ID is the ID of the window manager. It is used
     * to identify the window manager implementation (if necessary) by the
     * DerivedRenderApp class.
     */
    static const uint WINDOW_MANAGER_ID = WindowManagerId::HEADLESS;

    HeadlessManager(ParentType* parent = nullptr) :
            HeadlessManager("VCLib Headless", 1024, 768, parent)
    {
    }

    HeadlessManager(
        const std::string& windowTitle,
        uint               width  = 1024,
        uint               height = 768,
        ParentType*               = nullptr) :
            mTitle(windowTitle),
            mWidth(width),
            mHeight(height)
    {
        static_assert(
            RenderAppConcept<DerivedRenderApp>,
            "The DerivedRenderApp must satisfy the RenderAppConcept.");

#ifndef VCLIB_RENDER_BACKEND_BGFX
        static_assert(
            sizeof(DerivedRenderApp) == 0,
            "HeadlessManager is currently only supported with the BGFX backend. "
            "OpenGL2 requires a context that must be explicitly created.");
#endif
    }

    virtual ~HeadlessManager() = default;

    const std::string& windowTitle() const { return mTitle; }

    void setWindowTitle(const std::string& title) { mTitle = title; }

    uint width() const { return mWidth; }

    uint height() const { return mHeight; }

    void resize(uint width, uint height)
    {
        mWidth  = width;
        mHeight = height;
        DerivedRenderApp::WM::resize(derived(), width, height);
    }

    void show()
    {
        DerivedRenderApp::WM::init(derived());
        
        mUpdateRequested = true;
        while (mUpdateRequested) {
            mUpdateRequested = false;
            DerivedRenderApp::WM::paint(derived());
        }
    }

    /**
     * @brief Returns true if the window is minimized (i.e. iconified), false
     * otherwise.
     * @return true if the window is minimized, false otherwise.
     */
    bool isMinimized() const { return false; }

    void update() { mUpdateRequested = true; }

    void setContinuousRedraw(bool enabled) { (void) enabled; }

    Point2f dpiScale() const { return Point2f(1.0f, 1.0f); }

    void* winId() const { return nullptr; }

    static void* displayId() { return nullptr; }

    static vcl::NativeWindowHandleType handleType()
    {
        return vcl::NativeWindowHandleType::DEFAULT;
    }

protected:
    void* windowPtr() { return nullptr; }

    auto* derived() { return static_cast<DerivedRenderApp*>(this); }

    const auto* derived() const
    {
        return static_cast<const DerivedRenderApp*>(this);
    }
};

} // namespace vcl

#endif // VCL_RENDER_HEADLESS_MANAGER_H
