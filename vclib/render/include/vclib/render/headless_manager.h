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

/**
 * @brief The HeadlessManager class provides a window manager for headless
 * rendering.
 *
 * It simulates a window without actually displaying it, which is useful for
 * off-screen rendering, automated testing, or server-side rendering where a
 * physical display is not available.
 *
 * @tparam DerivedRenderApp: The derived render application class.
 */
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

    /**
     * @brief Constructs a HeadlessManager with default dimensions.
     * @param[in] parent Optional parent object (not managed in headless).
     */
    HeadlessManager(ParentType* parent = nullptr) :
            HeadlessManager("VCLib Headless", 1024, 768, parent)
    {
    }

    /**
     * @brief Constructs a HeadlessManager with a specific title and dimensions.
     * @param[in] windowTitle: The title of the headless window.
     * @param[in] width: The initial width of the canvas.
     * @param[in] height: The initial height of the canvas.
     * @param[in] parent: Optional parent object.
     */
    HeadlessManager(
        const std::string& windowTitle,
        uint               width  = 1024,
        uint               height = 768,
        ParentType*               = nullptr) :
            mTitle(windowTitle), mWidth(width), mHeight(height)
    {
        static_assert(
            RenderAppConcept<DerivedRenderApp>,
            "The DerivedRenderApp must satisfy the RenderAppConcept.");
    }

    virtual ~HeadlessManager() = default;

    /**
     * @brief Retrieves the window title.
     * @return The title of the headless window.
     */
    const std::string& windowTitle() const { return mTitle; }

    /**
     * @brief Sets the window title.
     * @param[in] title: The new title.
     */
    void setWindowTitle(const std::string& title) { mTitle = title; }

    /**
     * @brief Retrieves the width of the canvas.
     * @return The width in pixels.
     */
    uint width() const { return mWidth; }

    /**
     * @brief Retrieves the height of the canvas.
     * @return The height in pixels.
     */
    uint height() const { return mHeight; }

    /**
     * @brief Resizes the canvas.
     * @param[in] width: The new width in pixels.
     * @param[in] height: The new height in pixels.
     */
    void resize(uint width, uint height)
    {
        mWidth  = width;
        mHeight = height;
        DerivedRenderApp::WM::resize(derived(), width, height);
    }

    /**
     * @brief Runs the headless rendering loop.
     *
     * It initializes the render application and continuously calls paint()
     * as long as an update is requested. This allows rendering to continue
     * until the application finishes its tasks (e.g., taking a screenshot).
     */
    void show()
    {
        DerivedRenderApp::WM::init(derived());

        mUpdateRequested = true;

        // Loop runs as long as an update is requested by the application.
        // In headless mode, this allows fast consecutive renders (no swap
        // buffer blocking), but can become a busy loop if continuous redraw is
        // active.
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

    /**
     * @brief Requests an update, queuing a redraw for the next iteration of the
     * loop.
     */
    void update() { mUpdateRequested = true; }

    /**
     * @brief Toggles continuous redraw mode.
     * @param[in] enabled: Whether to enable continuous redraw.
     * @note In a headless environment, continuous redraw might lead to an
     * infinite busy loop.
     */
    void setContinuousRedraw(bool enabled) { (void) enabled; }

    /**
     * @brief Retrieves the DPI scale of the screen.
     * @return The DPI scale (1.0 in headless).
     */
    Point2f dpiScale() const { return Point2f(1.0f, 1.0f); }

    /**
     * @brief Retrieves the window ID (unsupported in headless).
     * @return nullptr.
     */
    void* winId() const { return nullptr; }

    /**
     * @brief Retrieves the display ID (unsupported in headless).
     * @return nullptr.
     */
    static void* displayId() { return nullptr; }

    /**
     * @brief Retrieves the native window handle type.
     * @return The default handle type.
     */
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
