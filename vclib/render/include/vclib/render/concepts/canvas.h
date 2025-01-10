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

#ifndef CONCEPTS_CANVAS_H
#define CONCEPTS_CANVAS_H

#include <vclib/concepts.h>
#include <vclib/space/core/color.h>
#include <vclib/space/core/point.h>

namespace vcl {

/**
 * @brief The CanvasConcept concept is used to check if a class satisfies the
 * requirements of the Canvas concept.
 *
 * Each class that satisfies this concept can be used as a canvas in the
 * @ref vcl::RenderApp class (second template parameter). The Canvas class is
 * responsible for managing the render backend and the surface where the Drawer
 * objects can draw.
 *
 * It is a class that is templated on the RenderApp class (using the CRTP
 * pattern). The class is then allowed to access the member functions of the
 * public members of the @ref vcl::RenderApp class and all the members of the
 * @ref vcl::RenderApp::CNV inner class.
 *
 * @par Constructors
 *
 * The class must have the following constructors:
 * - `CanvasType(void* winId, uint width, uint height)`: Constructor that
 * initializes the canvas with the window id (the platform dependent
 * identifier of the window where the canvas will be placed) and the initial
 * size of the canvas.
 * - `CanvasType(void* winId, uint width, uint height, void* displayId)`: Same
 * as the previous constructor, but also takes the display id (the platform
 * dependent identifier of the display where the window is placed - this
 * parameter is required only on linux platforms, it can be left nullptr in
 * other platforms).
 *
 * @par Inner types
 *
 * The class must have the following inner types:
 * - `CallbackReadBuffer`: A type that represents the signature of the callback
 * function that will be called when the depth value is read from the depth
 * buffer of the Canvas. See the `onReadDepth` member function for more details.
 *
 * @par Member functions
 *
 * The class must have the following member functions:
 * - `size() -> Point2Concept`: Returns the size of the canvas, as a Point2i
 * object having the width as x() and height as y().
 * - `viewId() -> uint`: Returns an unsigned integer that represents the view
 * id of the canvas. The view id is used to identify the canvas when drawing
 * content on it, and it is passed to the Drawer objects at each draw call.
 * - `onInit() -> void`: Called when the canvas is initialized. This function
 * is called by the RenderApp class when the WindowManager of the vcl::RenderApp
 * is initialized. This function should contain all the initialization calls
 * that cannot be done in the constructor (e.g. because the WindowManager
 * could not guarantee the initialization of the backend context in the
 * constructor).
 * - `onResize(uint width, uint height) -> void`: Called when the canvas is
 * resized. This function is called by the RenderApp class when the
 * WindowManager of the vcl::RenderApp is resized. The function should resize
 * the canvas to the new size.
 * - `onPaint() -> void`: Called when the canvas must be repainted. This
 * function is called by the RenderApp class when the WindowManager of the
 * vcl::RenderApp is repainted. The function should draw the content of the
 * canvas, by calling the vcl::RenderApp::CNV::draw() member function of the
 * RenderApp class.
 * - `onReadDepth(const vcl::Point2i& point, CallbackReadBuffer callback) ->
 * bool`: Called when a Drawer object asks to read the depth value at the
 * specified point on the canvas. The function should read the depth value at
 * the specified point and call the callback function with the depth value.
 * - `onScreenshot(const std::string& filename, uint width = 0, uint height = 0)
 * -> bool`: Called when a Drawer object asks for a screenshot of the canvas.
 * The function should take a screenshot of the canvas and save it to the
 * specified filename. If the width and height are specified, the screenshot
 * should be resized to the specified dimensions.
 */
template<typename T>
concept CanvasConcept = requires (
    T&&                                       obj,
    void*                                     vPtr,
    uint                                      u,
    vcl::Point2i                              p,
    vcl::Color                                c,
    typename RemoveRef<T>::CallbackReadBuffer cbrb,
    std::string                               str) {

    typename RemoveRef<T>::CallbackReadBuffer;

    RemoveRef<T>(vPtr, u, u);
    RemoveRef<T>(vPtr, u, u, c);
    RemoveRef<T>(vPtr, u, u, c, vPtr);

    { obj.size() } -> Point2Concept;
    { obj.viewId() } -> std::convertible_to<uint>;

    // non const requirements
    requires IsConst<T> || requires {
        { obj.onInit() } -> std::same_as<void>; // qt+opengl requires init
        { obj.onResize(u, u) } -> std::same_as<void>;
        { obj.onPaint() } -> std::same_as<void>;

        { obj.onReadDepth(p, cbrb) } -> std::same_as<bool>;
        { obj.onScreenshot(str) } -> std::same_as<bool>;
        { obj.onScreenshot(str, u, u) } -> std::same_as<bool>;
    };
};

} // namespace vcl

#endif // CONCEPTS_CANVAS_H
