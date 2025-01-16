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

#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <vclib/render/canvas.h>
#include <vclib/render/render_app.h>

#ifdef VCLIB_WITH_QT
#include <vclib/qt/widget_manager.h>
#endif
#ifdef VCLIB_WITH_GLFW
#include <vclib/glfw/window_manager.h>
#endif

void windowManagerStaticAsserts()
{
    using namespace vcl;

#ifdef VCLIB_WITH_QT
    using RenderAppQ = RenderApp<qt::WidgetManager, Canvas>;

    static_assert(
        WindowManagerConcept<qt::WidgetManager<RenderAppQ>>,
        "qt::WidgetManager does not satisfy the WindowManagerConcept");
    static_assert(
        WindowManagerConcept<const qt::WidgetManager<RenderAppQ>>,
        "const qt::WidgetManager does not satisfy the WindowManagerConcept");
    static_assert(
        WindowManagerConcept<qt::WidgetManager<RenderAppQ>&>,
        "qt::WidgetManager& does not satisfy the WindowManagerConcept");
    static_assert(
        WindowManagerConcept<const qt::WidgetManager<RenderAppQ>&>,
        "const qt::WidgetManager& does not satisfy the WindowManagerConcept");
    static_assert(
        WindowManagerConcept<qt::WidgetManager<RenderAppQ>&&>,
        "qt::WidgetManager&& does not satisfy the WindowManagerConcept");
#endif

#ifdef VCLIB_WITH_GLFW
    using RenderAppG = RenderApp<glfw::WindowManager, Canvas>;

    static_assert(
        WindowManagerConcept<glfw::WindowManager<RenderAppG>>,
        "glfw::WindowManager does not satisfy the WindowManagerConcept");
    static_assert(
        WindowManagerConcept<const glfw::WindowManager<RenderAppG>>,
        "const glfw::WindowManager does not satisfy the WindowManagerConcept");
    static_assert(
        WindowManagerConcept<glfw::WindowManager<RenderAppG>&>,
        "glfw::WindowManager& does not satisfy the WindowManagerConcept");
    static_assert(
        WindowManagerConcept<const glfw::WindowManager<RenderAppG>&>,
        "const glfw::WindowManager& does not satisfy the WindowManagerConcept");
    static_assert(
        WindowManagerConcept<glfw::WindowManager<RenderAppG>&&>,
        "glfw::WindowManager&& does not satisfy the WindowManagerConcept");
#endif
}

#endif // WINDOW_MANAGER_H
