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

#ifndef VCL_RENDER_RENDER_APP_H
#define VCL_RENDER_RENDER_APP_H

#include "concepts/blocker_event_drawer.h"
#include "concepts/canvas.h"
#include "concepts/event_drawer.h"
#include "concepts/window_manager.h"
#include "input.h"
#include "read_buffer_types.h"

#include <vclib/space/core/point.h>

namespace vcl {

/**
 * @brief The RenderApp class is a template class that combines a canvas, a
 * window manager, and a set of drawers, allowing them to work together and
 * communicate with each other.
 *
 * The RenderApp class uses the Curiously Recurring Template Pattern (CRTP) to
 * allow the derived classes to access the member functions of the RenderApp
 * class that propagate events from one derived class (e.g. the WindowManager)
 * to another (e.g. the Canvas).
 *
 * Each derived class has a role:
 * - The WindowManager class manages the window and the window events.
 * - The Canvas class manages the render backend and the surface where the
 *   drawers can draw.
 * - The Drawer classes draws the content on the canvas.
 *
 * @tparam WindowManagerT The type of the window manager class. It must satisfy
 * the WindowManagerConcept.
 */
template<
    template<typename> typename WindowManagerT,
    template<typename> typename CanvasT,
    template<typename> typename... Drawers>
class RenderApp :
        public WindowManagerT<RenderApp<WindowManagerT, CanvasT, Drawers...>>,
        public CanvasT<RenderApp<WindowManagerT, CanvasT, Drawers...>>,
        public Drawers<RenderApp<WindowManagerT, CanvasT, Drawers...>>...
{
    using WindowManagerType = WindowManagerT<RenderApp>;
    using CanvasType = CanvasT<RenderApp>;

    static_assert(
        WindowManagerConcept<WindowManagerType>,
        "The first template parameter type of the RenderApp class must be a "
        "class that satisfies the WindowManagerConcept.");

    static_assert(
        CanvasConcept<CanvasType>,
        "The second template parameter type of the RenderApp class must be a "
        "class that satisfies the CanvasConcept.");

    static_assert(
        (DrawerConcept<Drawers<RenderApp>> && ...),
        "All the Drawer types must satisfy the DrawerConcept.");

    KeyModifiers mKeyModifiers = {KeyModifier::NO_MODIFIER};

    // hide CanvasType member functions
    using CanvasType::size;
    using CanvasType::setDefaultClearColor;
    using CanvasType::onInit;
    using CanvasType::onResize;
    using CanvasType::onPaint;
    using CanvasType::onReadDepth;
    using CanvasType::onScreenshot;

    // hide WindowManagerType member functions
    using WindowManagerType::displayId;
    using WindowManagerType::winId;

protected:
    using CanvasType::viewId;

    // TODO: find a way to hide the Drawer member functions here

public:
    using ParentType = WindowManagerType::ParentType;

    // Attorneys
    class CNV;
    class WM;
    class DRW;

    RenderApp(ParentType* parent = nullptr) :
            RenderApp("RenderApp", 1024, 768, parent)
    {
    }

    RenderApp(
        const std::string& windowTitle,
        uint               width  = 1024,
        uint               height = 768,
        ParentType*        parent = nullptr) :
            WindowManagerType(windowTitle, width, height, parent),
            CanvasType(
                (void*) WindowManagerType::winId(),
                width * WindowManagerType::dpiScale().x(),
                height * WindowManagerType::dpiScale().y(),
                WindowManagerType::displayId()),
            Drawers<RenderApp>(
                width * WindowManagerType::dpiScale().x(),
                height * WindowManagerType::dpiScale().y())...
    {
    }

private:
    /**
     * @brief Calls a lambda function - that represents an event - for every
     * Drawer object listed in the template parameter pack.
     *
     * This function checks if the lambda function can be called for the D
     * type, and if it can, it calls it. Then, it calls itself recursively for
     * the other Drawer types in the list.
     *
     * This function will call the lambda function for each Drawer object in the
     * template parameter pack, in the order they are listed, only if several
     * conditions are satisfied:
     * - The D type must satisfy the EventDrawerConcept: if it does not, the
     *   lambda will not be called for that type.
     * - If the D type satisfies the CanBlockEventDrawerConcept, the lambda
     *   function will return a boolean value that will be used to block the
     *   event propagation to the other drawers.
     *
     * @param lambda
     */
    template<typename D, typename ...Others>
    void callEventFunForDrawers(auto lambda)
    {
        bool block = false;
        if constexpr(EventDrawerConcept<D>){
            if constexpr (BlockerEventDrawerConcept<D>) {
                block = lambda.template operator()<D>(this);
            } else {
                lambda.template operator()<D>(this);
            }
        }
        if constexpr(sizeof...(Others) > 0){
            if (!block) {
                callEventFunForDrawers<Others...>(lambda);
            }
        }
    }

    // Base case when there are no drawers
    void callEventFunForDrawers(auto lambda)
    {
    }

    /***** Member functions called by WindowManagerType *****/
    // Documentation is in the RenderApp::WM inner class

    void wmInit()
    {
        CanvasType::onInit();
        (static_cast<Drawers<RenderApp>*>(this)->onInit(CanvasType::viewId()),
         ...);
    }

    void wmResize(uint width, uint height)
    {
        CanvasType::onResize(width, height);

        // call the onResize member function of each Drawer object.
        // NOTE: use static_cast<Drawers*>(this)->function() to call the
        // right VIRTUAL function of the Drawer object.
        (static_cast<Drawers<RenderApp>*>(this)->onResize(width, height), ...);
    }

    void wmPaint() { CanvasType::onPaint(); }

    void wmSetModifiers(const KeyModifiers& modifiers)
    {
        mKeyModifiers = modifiers;
    }

    void wmKeyPress(Key::Enum key)
    {
        // given this object t and the type of a Drawer D, call the event member
        // function of the Drawer object
        auto lambda = [&]<typename D>(auto* t){
            // NOTE: use static_cast<Drawers*>(this)->function() to call the
            // right VIRTUAL function of the Drawer object.
            return static_cast<D*>(t)->onKeyPress(key, mKeyModifiers);
        };

               // call the lambda for all the drawers
        callEventFunForDrawers<Drawers<RenderApp>...>(lambda);
    }

    void wmKeyRelease(Key::Enum key)
    {
        // given this object t and the type of a Drawer D, call the event member
        // function of the Drawer object
        auto lambda = [&]<typename D>(auto* t){
            // NOTE: use static_cast<Drawers*>(this)->function() to call the
            // right VIRTUAL function of the Drawer object.
            return static_cast<D*>(t)->onKeyRelease(key, mKeyModifiers);
        };

               // call the lambda for all the drawers
        callEventFunForDrawers<Drawers<RenderApp>...>(lambda);
    }

    void wmMouseMove(double x, double y)
    {
        // given this object t and the type of a Drawer D, call the event member
        // function of the Drawer object
        auto lambda = [&]<typename D>(auto* t){
            // NOTE: use static_cast<Drawers*>(this)->function() to call the
            // right VIRTUAL function of the Drawer object.
            return static_cast<D*>(t)->onMouseMove(x, y, mKeyModifiers);
        };

        // call the lambda for all the drawers
        callEventFunForDrawers<Drawers<RenderApp>...>(lambda);
    }

    void wmMousePress(MouseButton::Enum button, double x, double y)
    {
        // given this object t and the type of a Drawer D, call the event member
        // function of the Drawer object
        auto lambda = [&]<typename D>(auto* t){
            // NOTE: use static_cast<Drawers*>(this)->function() to call the
            // right VIRTUAL function of the Drawer object.
            return static_cast<D*>(t)->onMousePress(
                button, x, y, mKeyModifiers);
        };

        // call the lambda for all the drawers
        callEventFunForDrawers<Drawers<RenderApp>...>(lambda);
    }

    void wmMouseRelease(MouseButton::Enum button, double x, double y)
    {
        // given this object t and the type of a Drawer D, call the event member
        // function of the Drawer object
        auto lambda = [&]<typename D>(auto* t){
            // NOTE: use static_cast<Drawers*>(this)->function() to call the
            // right VIRTUAL function of the Drawer object.
            return static_cast<D*>(t)->onMouseRelease(
                button, x, y, mKeyModifiers);
        };

        // call the lambda for all the drawers
        callEventFunForDrawers<Drawers<RenderApp>...>(lambda);
    }

    void wmMouseDoubleClick(MouseButton::Enum button, double x, double y)
    {
        // given this object t and the type of a Drawer D, call the event member
        // function of the Drawer object
        auto lambda = [&]<typename D>(auto* t) {
            // NOTE: use static_cast<Drawers*>(this)->function() to call the
            // right VIRTUAL function of the Drawer object.
            return static_cast<D*>(t)->onMouseDoubleClick(
                button, x, y, mKeyModifiers);
        };

        // call the lambda for all the drawers
        callEventFunForDrawers<Drawers<RenderApp>...>(lambda);
    }

    void wmMouseScroll(double x, double y)
    {
        // given this object t and the type of a Drawer D, call the event member
        // function of the Drawer object
        auto lambda = [&]<typename D>(auto* t) {
            // NOTE: use static_cast<Drawers*>(this)->function() to call the
            // right VIRTUAL function of the Drawer object.
            return static_cast<D*>(t)->onMouseScroll(x, y, mKeyModifiers);
        };

        // call the lambda for all the drawers
        callEventFunForDrawers<Drawers<RenderApp>...>(lambda);
    }

    /***** Member functions called by CanvasType *****/
    // Documentation is in the RenderApp::CNV inner class

    void cnvDraw()
    {
        // call the onDraw member function of each Drawer object.
        // NOTE: use static_cast<Drawers*>(this)->function() to call the
        // right VIRTUAL function of the Drawer object.
        (static_cast<Drawers<RenderApp>*>(this)->onDraw(CanvasType::viewId()),
         ...);
    }

    void cnvDrawContent()
    {
        // call the onDrawContent member function of each Drawer object.
        // NOTE: use static_cast<Drawers*>(this)->function() to call the
        // right VIRTUAL function of the Drawer object.
        (static_cast<Drawers<RenderApp>*>(this)->onDrawContent(
             CanvasType::viewId()),
         ...);
    }

    void cnvPostDraw()
    {
        // call the onPostDraw member function of each Drawer object.
        // NOTE: use static_cast<Drawers*>(this)->function() to call the
        // right VIRTUAL function of the Drawer object.
        (static_cast<Drawers<RenderApp>*>(this)->onPostDraw(), ...);
    }

    /***** Member functions called by Drawer objects *****/
    // Documentation is in the RenderApp::DRW inner class

    void* dWindowPtr() { return WindowManagerType::windowPtr(); }

    vcl::Point2<uint> dCanvasSize() const { return CanvasType::size(); }

    void dSetCanvasDefaultClearColor(const Color& color)
    {
        CanvasType::setDefaultClearColor(color);
    }

    auto dCanvasFrameBuffer() const { return CanvasType::frameBuffer(); }

    [[nodiscard]] bool dReadDepth(
        const Point2i&     point,
        ReadBufferTypes::CallbackReadBuffer callback = nullptr)
    {
        return CanvasType::onReadDepth(point, callback);
    }

    void dScreenshot(const std::string& filename, uint width, uint height)
    {
        CanvasType::onScreenshot(filename, width, height);
    }
};

/*** Inner classes: Attorneys ***/
// (https://tinyurl.com/kp8m28je)

/**
 * @brief The RenderApp::WM inner class is an Attorney that allow access to some
 * private member functions of the RenderApp class to the WindowManagerType
 * class.
 *
 * The member functions of the RenderApp::WM inner class can be called only by
 * the WindowManagerType class. For example, to call the init member function,
 * the WindowManagerType can call it in the following way:
 *
 * @code{.cpp}
 * RenderApp::WM::init(static_cast<RenderApp*>(this));
 * @endcode
 */
template<
    template<typename> typename WindowManagerT,
    template<typename> typename CanvasT,
    template<typename> typename... Drawers>
class RenderApp<WindowManagerT, CanvasT, Drawers...>::WM
{
    using WindowManagerType = WindowManagerT<RenderApp>;

    friend WindowManagerType;

    /**
     * @brief The WindowManagerType calls this member function when the window
     * render backend is initialized. The RenderApp propagates the init event to
     * the CanvasType and to each Drawer object, by calling the `onInit(uint)`
     * member function.
     */
    static void init(RenderApp* r) { r->wmInit(); }

    /**
     * @brief The WindowManagerType calls this member function when the window
     * is resized, telling the new width and height. The RenderApp propagates
     * the resize event to the CanvasType and to each Drawer object, by calling
     * the `onResize(uint, uint)` member function.
     *
     * @param width
     * @param height
     */
    static void resize(RenderApp* r, uint width, uint height)
    {
        r->wmResize(width, height);
    }

    /**
     * @brief The WindowManagerType calls this member function when the window
     * triggers a paint event.
     */
    static void paint(RenderApp* r) { r->wmPaint(); }

    /**
     * @brief The WindowManagerType calls this member function when the current
     * modifiers are updated.
     */
    static void setModifiers(RenderApp* r, const KeyModifiers& modifiers)
    {
        r->wmSetModifiers(modifiers);
    }

    /**
     * @brief The WindowManagerType calls this member function when a key is
     * pressed.
     *
     * The event (along with the current key modifiers) is propagated to each
     * Drawer object that is a EventDrawer object, by calling the
     * `onKeyPress(Key::Enum, KeyModifiers)` member function.
     *
     * @param key
     */
    static void keyPress(RenderApp* r, Key::Enum key) { r->wmKeyPress(key); }

    /**
     * @brief The WindowManagerType calls this member function when a key is
     * released.
     *
     * The event (along with the current key modifiers) is propagated to each
     * Drawer object that is a EventDrawer object, by calling the
     * `onKeyRelease(Key::Enum, KeyModifiers)` member function.
     *
     * @param key
     */
    static void keyRelease(RenderApp* r, Key::Enum key)
    {
        r->wmKeyRelease(key);
    }

    /**
     * @brief The WindowManagerType calls this member function when the mouse
     * cursor is moved.
     *
     * The event (along with the current key modifiers) is propagated to each
     * Drawer object that is a EventDrawer object, by calling the
     * `onMouseMove(double, double, KeyModifiers)` member function.
     *
     * @param x
     * @param y
     */
    static void mouseMove(RenderApp* r, double x, double y)
    {
        r->wmMouseMove(x, y);
    }

    /**
     * @brief The WindowManagerType calls this member function when a mouse
     * button is pressed.
     *
     * The event (along with the current key modifiers) is propagated to each
     * Drawer object that is a EventDrawer object, by calling the
     * `onMousePress(MouseButton::Enum, double, double, KeyModifiers)` member
     * function.
     *
     * @param button
     * @param x
     * @param y
     */
    static void mousePress(
        RenderApp*         r,
        MouseButton::Enum button,
        double            x,
        double            y)
    {
        r->wmMousePress(button, x, y);
    }

    /**
     * @brief The WindowManagerType calls this member function when a mouse
     * button is released.
     *
     * The event (along with the current key modifiers) is propagated to each
     * Drawer object that is a EventDrawer object, by calling the
     * `onMouseRelease(MouseButton::Enum, double, double, KeyModifiers)` member
     * function.
     *
     * @param button
     * @param x
     * @param y
     */
    static void mouseRelease(
        RenderApp*         r,
        MouseButton::Enum button,
        double            x,
        double            y)
    {
        r->wmMouseRelease(button, x, y);
    }

    /**
     * @brief The WindowManagerType calls this member function when a mouse
     * button is double clicked.
     *
     * The event (along with the current key modifiers) is propagated to each
     * Drawer object that is a EventDrawer object, by calling the
     * `onMouseDoubleClick(MouseButton::Enum, double, double, KeyModifiers)`
     * member function.
     *
     * @param button
     * @param x
     * @param y
     */
    static void mouseDoubleClick(
        RenderApp*         r,
        MouseButton::Enum button,
        double            x,
        double            y)
    {
        r->wmMouseDoubleClick(button, x, y);
    }

    /**
     * @brief The WindowManagerType calls this member function when the mouse
     * wheel is scrolled.
     *
     * The event (along with the current key modifiers) is propagated to each
     * Drawer object that is a EventDrawer object, by calling the
     * `onMouseScroll(double, double, KeyModifiers)` member function.
     *
     * @param button
     * @param x
     * @param y
     */
    static void mouseScroll(RenderApp* r, double x, double y)
    {
        r->wmMouseScroll(x, y);
    }
};

/**
 * @brief The RenderApp::CNV inner class is an Attorney that allow access to some
 * private member functions of the RenderApp class to the CanvasType class.
 *
 * The member functions of the RenderApp::CNV inner class can be called only by
 * the CanvasType class. For example, to call the update member function,
 * the CanvasType can call it in the following way:
 *
 * @code{.cpp}
 * RenderApp::CNV::update(static_cast<RenderApp*>(this));
 * @endcode
 */
template<
    template<typename> typename WindowManagerT,
    template<typename> typename CanvasT,
    template<typename> typename... Drawers>
class RenderApp<WindowManagerT, CanvasT, Drawers...>::CNV
{
    using CanvasType = CanvasT<RenderApp>;

    friend CanvasType;

    /**
     * @brief The CanvasType is ready to draw, and asks the RenderApp to call
     * the `onDraw(uint())` function for every Drawer object.
     */
    static void draw(RenderApp* r) { r->cnvDraw(); }

    /**
     * @brief The CanvasType wants to draw only the content of the objects,
     * without any decorator (e.g. axis, trackball, grid, etc.).
     * This scenario is useful when the user wants to take a snapshot of the
     * scene without any decoration. It asks the RenderApp to call the
     * `onDrawContent(uint())` function for every Drawer object.
     */
    static void drawContent(RenderApp* r) { r->cnvDrawContent(); }

    /**
     * @brief The CanvasType has finished drawing and has submitted the new
     * frame, and asks the RenderApp to call the `onPostDraw()` function for
     * every Drawer object.
     */
    static void postDraw(RenderApp* r) { r->cnvPostDraw(); }
};

/**
 * @brief The RenderApp::DRW inner class is an Attorney that allow access to some
 * private member functions of the RenderApp class to the Drawer
 * classes.
 *
 * The member functions of the RenderApp::DRW inner class can be called only by
 * the Drawer classes. For example, to call the canvasFrameBuffer member
 * function, the Drawer can call it in the following way:
 *
 * @code{.cpp}
 * RenderApp::DRW::canvasFrameBuffer(static_cast<RenderApp*>(this));
 * @endcode
 */
template<
    template<typename> typename WindowManagerT,
    template<typename> typename CanvasT,
    template<typename> typename... Drawers>
class RenderApp<WindowManagerT, CanvasT, Drawers...>::DRW
{
    // TODO: right now all the function in this inner class are public,
    // because variadic friends are still not allowed in C++.
    // It will allowed in C++26: https://stackoverflow.com/a/78246001/5851101
    // As soon as this feature will be available on all the major compilers,
    // the functions will be made private.
    //friend Drawers<RenderApp<WindowManagerT, CanvasT, Drawers...>>...;
public: // TODO - remove this when C++26 is supported

    /**
     * @brief A Drawer object can request the window pointer of the window
     * manager (the exact meaning of the window pointer depends on the window
     * manager implementation).
     *
     * To get the window pointer, the Drawer object can call the `windowPtr()`
     * function. To know the exact meaning of the window pointer, the Drawer
     * object should refer to the documentation of the specific window manager
     * implementation, and check the WINDOW_MANAGER_ID of the window manager,
     * that is a static constant of the WindowManager class.
     *
     * @param r
     * @return
     */
    static void* windowPtr(RenderApp* r) { return r->dWindowPtr(); }

    /**
     * @brief A Drawer object can request the size of the canvas. This function
     * is called by the Drawer object to request the size of the canvas.
     *
     * @return The size of the canvas.
     *
     * // TODO is this actually necessary???
     */
    static Point2<uint> canvasSize(const RenderApp* r)
    {
        return r->dCanvasSize();
    }

    /**
     * @brief A Drawer object can set the default clear color of the canvas.
     *
     * This function is called by the Drawer object to set the default clear
     * color of the canvas.
     *
     * @param color: The new default clear color of the canvas.
     */
    static void setCanvasDefaultClearColor(RenderApp* r, const Color& color)
    {
        r->dSetCanvasDefaultClearColor(color);
    }

    /**
     * @brief A Drawer object can request the frame buffer of the canvas.
     *
     * This function is called by the Drawer object to request the frame buffer
     * of the canvas, that can be used to draw on it.
     *
     * @warning This function is supported only if the backend is bgfx.
     *
     * @return The frame buffer of the canvas.
     */
    static auto canvasFrameBuffer(const RenderApp* r)
    {
        return r->dCanvasFrameBuffer();
    }

    /**
     * @brief A Drawer object can request the depth value at a specific point
     * on the canvas. This function is called by the Drawer object to request
     * the depth value at the specified point.
     *
     * @param[in] point: The point on the canvas where the depth value must be
     * read.
     * @param[in] callback: The callback function that will be called when the
     * depth value is read. TODO: explain the callback function signature.
     *
     * @return true if the depth value is successfully read, false otherwise.
     */
    [[nodiscard]] static bool readDepth(
        RenderApp* r,
        const Point2i&     point,
        ReadBufferTypes::CallbackReadBuffer callback = nullptr)
    {
        return r->dReadDepth(point, callback);
    }

    /**
     * @brief A Drawer object can request a screenshot of the canvas. This
     * function is called by the Drawer object to request a screenshot of the
     * canvas.
     *
     * @param[in] filename: The filename where the screenshot will be saved.
     * @param[in] width: The width of the screenshot. If 0, the width of the
     * canvas will be used.
     * @param[in] height: The height of the screenshot. If 0, the height of the
     * canvas will be used.
     */
    static void screenshot(
        RenderApp* r,
        const std::string& filename,
        uint width = 0,
        uint height = 0)
    {
        r->dScreenshot(filename, width, height);
    }
};

} // namespace vcl

#endif // VCL_RENDER_RENDER_APP_H
