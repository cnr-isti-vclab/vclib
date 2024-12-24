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

#ifndef RENDERER_H
#define RENDERER_H

#include "concepts/canvas.h"
#include "concepts/event_drawer.h"
#include "concepts/window_manager.h"
#include "input.h"
#include "read_buffer_types.h"

#include <vclib/space/core/point.h>

namespace vcl {

/**
 * @brief The Renderer class is a template class that combines a canvas, a
 * window manager, and a set of drawers, allowing them to work together and
 * communicate with each other.
 *
 * The Renderer class uses the Curiously Recurring Template Pattern (CRTP) to
 * allow the derived classes to access the member functions of the Renderer
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
class Renderer :
        public WindowManagerT<Renderer<WindowManagerT, CanvasT, Drawers...>>,
        public CanvasT<Renderer<WindowManagerT, CanvasT, Drawers...>>,
        public Drawers<Renderer<WindowManagerT, CanvasT, Drawers...>>...
{
    using WindowManagerType =
        WindowManagerT<Renderer<WindowManagerT, CanvasT, Drawers...>>;
    using CanvasType = CanvasT<Renderer<WindowManagerT, CanvasT, Drawers...>>;

    static_assert(
        WindowManagerConcept<WindowManagerType>,
        "The first template parameter type of the Renderer class must be a "
        "class that satisfies the WindowManagerConcept.");

    static_assert(
        CanvasConcept<CanvasType>,
        "The second template parameter type of the Renderer class must be a "
        "class that satisfies the CanvasConcept.");

    static_assert(
        (DrawerConcept<
             Drawers<Renderer<WindowManagerT, CanvasT, Drawers...>>> &&
         ...),
        "All the Drawer types must satisfy the DrawerConcept.");

    KeyModifiers mKeyModifiers = {KeyModifier::NO_MODIFIER};

    // hide CanvasType member functions
    using CanvasType::onInit;
    using CanvasType::size;
    using CanvasType::onResize;
    using CanvasType::onPaint;

    // hide WindowManagerType member functions
    using WindowManagerType::displayId;
    using WindowManagerType::winId;

public:
    using ParentType = WindowManagerType::ParentType;

    // Attorneys
    class CNV;
    class WM;
    class D;

    Renderer(ParentType* parent = nullptr) :
            Renderer("Renderer", 1024, 768, parent)
    {
    }

    Renderer(
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
            Drawers<Renderer<WindowManagerT, CanvasT, Drawers...>>(
                width * WindowManagerType::dpiScale().x(),
                height * WindowManagerType::dpiScale().y())...
    {
    }

protected:
    using CanvasType::viewId;

private:
    /***** Member functions called by CanvasType *****/

    /**
     * @brief The CanvasType is ready to draw, and asks the Renderer to call
     * the `onDraw(uint())` function for every Drawer object.
     */
    void cnvDraw()
    {
        // call the onDraw member function of each Drawer object.
        // NOTE: use static_cast<Drawers*>(this)->function() to call the
        // right VIRTUAL function of the Drawer object.
        (static_cast<Drawers<Renderer<WindowManagerT, CanvasT, Drawers...>>*>(this)->onDraw(CanvasType::viewId()), ...);
    }

    /**
     * @brief The CanvasType wants to draw only the content of the objects,
     * without any decorator (e.g. axis, trackball, grid, etc.).
     * This scenario is useful when the user wants to take a snapshot of the
     * scene without any decoration. It asks the Renderer to call the
     * `onDrawContent(uint())` function for every Drawer object.
     */
    void cnvDrawContent()
    {
        // call the onDrawContent member function of each Drawer object.
        // NOTE: use static_cast<Drawers*>(this)->function() to call the
        // right VIRTUAL function of the Drawer object.
        (static_cast<Drawers<Renderer<WindowManagerT, CanvasT, Drawers...>>*>(this)->onDrawContent(CanvasType::viewId()), ...);
    }

    /***** Member functions called by WindowManagerType *****/

    /**
     * @brief The WindowManagerType calls this member function when the window
     * render backend is initialized. The Renderer propagates the init event to
     * the CanvasType and to each Drawer object, by calling the `onInit()`
     * member function.
     */
    void wmInit()
    {
        CanvasType::onInit();
        (static_cast<Drawers<Renderer<WindowManagerT, CanvasT, Drawers...>>*>(this)->onInit(), ...);
    }

    /**
     * @brief The WindowManagerType calls this member function when the window
     * is resized, telling the new width and height. The Renderer propagates
     * the resize event to the CanvasType and to each Drawer object, by calling
     * the `onResize(uint, uint)` member function.
     *
     * @param width
     * @param height
     */
    void wmResize(uint width, uint height)
    {
        CanvasType::onResize(width, height);

        // call the onResize member function of each Drawer object.
        // NOTE: use static_cast<Drawers*>(this)->function() to call the
        // right VIRTUAL function of the Drawer object.
        (static_cast<Drawers<Renderer<WindowManagerT, CanvasT, Drawers...>>*>(this)->onResize(width, height), ...);
    }

    /**
     * @brief The WindowManagerType calls this member function when the window
     * triggers a paint event.
     */
    void wmPaint() { CanvasType::onPaint(); }

    /**
     * @brief The WindowManagerType calls this member function when the current
     * modifiers are updated.
     */
    void wmSetModifiers(const KeyModifiers& modifiers)
    {
        mKeyModifiers = modifiers;
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
    void wmKeyPress(Key::Enum key)
    {
        // call onKeyPress member function of each Drawer object, ONLY if the
        // object is an EventDrawer (it satisfies the EventDrawerConcept).
        // NOTE: use static_cast<Drawers*>(this)->function() to call the
        // right VIRTUAL function of the Drawer object.
        auto lambda = [&]<typename D>(auto* t){
            if constexpr(EventDrawerConcept<D>){
                static_cast<D*>(t)->onKeyPress(key, mKeyModifiers);
            }
        };

        (lambda.template
         operator()<Drawers<Renderer<WindowManagerT, CanvasT, Drawers...>>>(
             this),
         ...);
    }

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
    void wmKeyRelease(Key::Enum key)
    {
        // call onKeyRelease member function of each Drawer object, ONLY if the
        // object is an EventDrawer (it satisfies the EventDrawerConcept).
        // NOTE: use static_cast<Drawers*>(this)->function() to call the
        // right VIRTUAL function of the Drawer object.
        auto lambda = [&]<typename D>(auto* t){
            if constexpr(EventDrawerConcept<D>){
                static_cast<D*>(t)->onKeyRelease(key, mKeyModifiers);
            }
        };

        (lambda.template
         operator()<Drawers<Renderer<WindowManagerT, CanvasT, Drawers...>>>(
             this),
         ...);
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
    void wmMouseMove(double x, double y)
    {
        // call onMouseMove member function of each Drawer object, ONLY if the
        // object is an EventDrawer (it satisfies the EventDrawerConcept).
        // NOTE: use static_cast<Drawers*>(this)->function() to call the
        // right VIRTUAL function of the Drawer object.
        auto lambda = [&]<typename D>(auto* t){
            if constexpr(EventDrawerConcept<D>){
                static_cast<D*>(t)->onMouseMove(x, y, mKeyModifiers);
            }
        };

        (lambda.template
         operator()<Drawers<Renderer<WindowManagerT, CanvasT, Drawers...>>>(
             this),
         ...);
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
    void wmMousePress(MouseButton::Enum button, double x, double y)
    {
        // call onMousePress member function of each Drawer object, ONLY if the
        // object is an EventDrawer (it satisfies the EventDrawerConcept).
        // NOTE: use static_cast<Drawers*>(this)->function() to call the
        // right VIRTUAL function of the Drawer object.
        auto lambda = [&]<typename D>(auto* t){
            if constexpr(EventDrawerConcept<D>){
                static_cast<D*>(t)->onMousePress(button, x, y, mKeyModifiers);
            }
        };

        (lambda.template
         operator()<Drawers<Renderer<WindowManagerT, CanvasT, Drawers...>>>(
             this),
         ...);
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
    void wmMouseRelease(MouseButton::Enum button, double x, double y)
    {
        // call onMouseRelease member function of each Drawer object, ONLY if
        // the object is an EventDrawer (it satisfies the EventDrawerConcept).
        // NOTE: use static_cast<Drawers*>(this)->function() to call the
        // right VIRTUAL function of the Drawer object.
        auto lambda = [&]<typename D>(auto* t){
            if constexpr(EventDrawerConcept<D>){
                static_cast<D*>(t)->onMouseRelease(button, x, y, mKeyModifiers);
            }
        };

        (lambda.template
         operator()<Drawers<Renderer<WindowManagerT, CanvasT, Drawers...>>>(
             this),
         ...);
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
    void wmMouseDoubleClick(MouseButton::Enum button, double x, double y)
    {
        // call onMouseDoubleClick member function of each Drawer object, ONLY
        // if the object is an EventDrawer (it satisfies the
        // EventDrawerConcept).
        // NOTE: use static_cast<Drawers*>(this)->function() to call the right
        // VIRTUAL function of the Drawer object.
        auto lambda = [&]<typename D>(auto* t){
            if constexpr(EventDrawerConcept<D>){
                static_cast<D*>(t)->onMouseDoubleClick(
                    button, x, y, mKeyModifiers);
            }
        };

        (lambda.template
         operator()<Drawers<Renderer<WindowManagerT, CanvasT, Drawers...>>>(
             this),
         ...);
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
    void wmMouseScroll(double x, double y)
    {
        // call onMouseScroll member function of each Drawer object, ONLY if the
        // object is an EventDrawer (it satisfies the EventDrawerConcept).
        // NOTE: use static_cast<Drawers*>(this)->function() to call the right
        // VIRTUAL function of the Drawer object.
        auto lambda = [&]<typename D>(auto* t){
            if constexpr(EventDrawerConcept<D>){
                static_cast<D*>(t)->onMouseScroll(
                    x, y, mKeyModifiers);
            }
        };

        (lambda.template
         operator()<Drawers<Renderer<WindowManagerT, CanvasT, Drawers...>>>(
             this),
         ...);
    }

    /***** Member functions called by Drawer objects *****/

    /**
     * @brief A Drawer object can request the size of the canvas. This function
     * is called by the Drawer object to request the size of the canvas.
     *
     * @return The size of the canvas.
     */
    vcl::Point2<uint> dCanvasSize() const { return CanvasType::size(); }

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
    auto dCanvasFrameBuffer() const { return CanvasType::frameBuffer(); }

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
    [[nodiscard]] bool dReadDepth(
        const Point2i&     point,
        ReadBufferTypes::CallbackReadBuffer callback = nullptr)
    {
        return CanvasType::readDepth(point, callback);
    }
};

/*** Inner classes: Attorneys ***/
// (https://tinyurl.com/kp8m28je)

/**
 * @brief The Renderer::CNV inner class is an Attorney that allow access to some
 * private member functions of the Renderer class to the CanvasType class.
 * They can be called only by the CanvasType class in the following way:
 * Renderer::CNV::update(static_cast<Renderer*>(this));
 */
template<
    template<typename> typename WindowManagerT,
    template<typename> typename CanvasT,
    template<typename> typename... Drawers>
class Renderer<WindowManagerT, CanvasT, Drawers...>::CNV
{
    using CanvasType = CanvasT<Renderer<WindowManagerT, CanvasT, Drawers...>>;

    friend CanvasType;

    static void draw(Renderer* r) { r->cnvDraw(); }

    static void drawContent(Renderer* r) { r->cnvDrawContent(); }
};

/**
 * @brief The Renderer::WM inner class is an Attorney that allow access to some
 * private member functions of the Renderer class to the WindowManager
 * class.
 * They can be called only by the WindowManager class in the following way:
 * Renderer::WM::init(static_cast<Renderer*>(this));
 */
template<
    template<typename> typename WindowManagerT,
    template<typename> typename CanvasT,
    template<typename> typename... Drawers>
class Renderer<WindowManagerT, CanvasT, Drawers...>::WM
{
    using WindowManagerType =
        WindowManagerT<Renderer<WindowManagerT, CanvasT, Drawers...>>;

    friend WindowManagerType;

    static void init(Renderer* r) { r->wmInit(); }

    static void resize(Renderer* r, uint width, uint height)
    {
        r->wmResize(width, height);
    }

    static void paint(Renderer* r) { r->wmPaint(); }

    static void setModifiers(Renderer* r, const KeyModifiers& modifiers)
    {
        r->wmSetModifiers(modifiers);
    }

    static void keyPress(Renderer* r, Key::Enum key) { r->wmKeyPress(key); }

    static void keyRelease(Renderer* r, Key::Enum key)
    {
        r->wmKeyRelease(key);
    }

    static void mouseMove(Renderer* r, double x, double y)
    {
        r->wmMouseMove(x, y);
    }

    static void mousePress(
        Renderer*         r,
        MouseButton::Enum button,
        double            x,
        double            y)
    {
        r->wmMousePress(button, x, y);
    }

    static void mouseRelease(
        Renderer*         r,
        MouseButton::Enum button,
        double            x,
        double            y)
    {
        r->wmMouseRelease(button, x, y);
    }

    static void mouseDoubleClick(
        Renderer*         r,
        MouseButton::Enum button,
        double            x,
        double            y)
    {
        r->wmMouseDoubleClick(button, x, y);
    }

    static void mouseScroll(Renderer* r, double x, double y)
    {
        r->wmMouseScroll(x, y);
    }
};

/**
 * @brief The Renderer::D inner class is an Attorney that allow access to some
 * private member functions of the Renderer class to the Drawer
 * classes.
 * They can be called only by the Drawer classes in the following way:
 * Renderer::D::canvasSize(static_cast<Renderer*>(this));
 */
template<
    template<typename> typename WindowManagerT,
    template<typename> typename CanvasT,
    template<typename> typename... Drawers>
class Renderer<WindowManagerT, CanvasT, Drawers...>::D
{
    // TODO: right now all the function in this inner class are public,
    // because variadic friends are still not allowed in C++.
    // It will allowed in C++26: https://stackoverflow.com/a/78246001/5851101
    // As soon as this feature will be available on all the major compilers,
    // the functions will be made private.
    //friend Drawers<Renderer<WindowManagerT, CanvasT, Drawers...>>...;
public: // TODO - remove this when C++26 is supported
    static Point2<uint> canvasSize(const Renderer* r)
    {
        return r->dCanvasSize();
    }

    static auto canvasFrameBuffer(const Renderer* r)
    {
        return r->dCanvasFrameBuffer();
    }

    [[nodiscard]] static bool readDepth(
        Renderer* r,
        const Point2i&     point,
        ReadBufferTypes::CallbackReadBuffer callback = nullptr)
    {
        return r->dReadDepth(point, callback);
    }

};

} // namespace vcl

#endif // RENDERER_H
