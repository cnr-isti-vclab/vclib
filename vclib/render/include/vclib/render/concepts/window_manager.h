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

#ifndef CONCEPTS_WINDOW_MANAGER_H
#define CONCEPTS_WINDOW_MANAGER_H

#include <vclib/concepts/space/point.h>

#include <concepts>

namespace vcl {
/**
 * @brief The WindowManagerConcept concept is used to check if a class satisfies
 * the requirements of the WindowManager concept.
 *
 * Each class that satisfies this concept can be used as a window manager in the
 * @ref vcl::Renderer class (first template parameter). The WindowManager class
 * is responsible for managing the window and the window events.
 *
 * It is a class that is templated on the Renderer class (using the CRTP
 * pattern). The class is then allowed to access the member functions of the
 * public members of the @ref vcl::Renderer class and all the members of the
 * @ref vcl::Renderer::WM inner class.
 *
 * Moreoever, to work correctly with the Renderer class, the Canvas class and
 * the Drawer classes, **the WindowManager class should call for each event**
 * (e.g. init, resize, mouseMove, ...) **the corresponding member function of
 * the vcl::Renderer::WM inner class**. This is necessary to propagate the event
 * to the Canvas and to the Drawer objects. This requirement is not modeled in
 * this concept definition because any platform can have different event
 * handling.
 *
 * @par Constructors
 *
 * The class must have the following constructors:
 * - `WindowManagerType(ParentType* parent = nullptr)`: Default constructor that
 * initializes the window manager with a parent object (if available).
 * - `WindowManagerType(const std::string& windowTitle, uint width, uint height,
 * ParentType* parent = nullptr)`: Constructor that initializes the window
 * manager with the window title, the initial width and height of the window,
 * and a parent object (if available).
 *
 * @par Inner types
 *
 * The class must have the following inner types:
 * - `ParentType`: The type of the parent object of the window manager. This
 * object is used to propagate events from the window manager to the parent
 * object, if available. The parent object is necessary only on some platforms
 * (e.g. Qt). If the parent object is not available, the type should be set to
 * `void`.
 *
 * @par Member functions
 *
 * The class must have the following member functions:
 * - `windowTitle() -> const std::string&`: Returns the title of the window.
 * - `setWindowTitle(const std::string&) -> void`: Sets the title of the window.
 * - `width() -> uint`: Returns the width of the window.
 * - `height() -> uint`: Returns the height of the window.
 * - `dpiScale() -> Point2Concept`: Returns the DPI scale of the window.
 * - `winId() -> void*`: Returns the platform dependent identifier of the
 * window.
 * - `displayId() -> void*`: Returns the platform dependent identifier of the
 * display where the window is placed. This parameter is required only on linux
 * platforms, it can be left nullptr in other platforms.
 * - `update() -> void`: Updates the window.
 */
template<typename T>
concept WindowManagerConcept =
    requires (
        T&& obj,
        typename RemoveRef<T>::ParentType* pPtr,
        std::string s,
        uint u) {
        typename RemoveRef<T>::ParentType;

        RemoveRef<T>();
        RemoveRef<T>(pPtr);
        RemoveRef<T>(s, u, u);
        RemoveRef<T>(s, u, u, pPtr);

        { obj.windowTitle() } -> std::same_as<const std::string&>;


        { obj.width() } -> std::convertible_to<uint>;
        { obj.height() } -> std::convertible_to<uint>;

        { obj.dpiScale() } -> Point2Concept;

        obj.winId(); // todo: try to check return type, should be void*
        { obj.displayId() } -> std::same_as<void*>;

        // non const requirements
        requires IsConst<T> || requires {
            { obj.setWindowTitle(std::string()) } -> std::same_as<void>;

            { obj.update() } -> std::same_as<void>;
        };
    };

} // namespace vcl

#endif // CONCEPTS_WINDOW_MANAGER_H
