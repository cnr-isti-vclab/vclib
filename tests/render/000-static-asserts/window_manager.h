// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
