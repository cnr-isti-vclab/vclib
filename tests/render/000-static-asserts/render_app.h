// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef RENDER_APP_H
#define RENDER_APP_H

#include <vclib/render/canvas.h>
#include <vclib/render/render_app.h>

#ifdef VCLIB_WITH_QT
#include <vclib/qt/widget_manager.h>
#elif VCLIB_WITH_GLFW
#include <vclib/glfw/window_manager.h>
#endif

#ifdef VCLIB_WITH_QT
#include <vclib/qt/widget_manager.h>
#endif
#ifdef VCLIB_WITH_GLFW
#include <vclib/glfw/window_manager.h>
#endif

void renderAppStaticAsserts()
{
    using namespace vcl;

#ifdef VCLIB_WITH_QT
    using RenderAppQ = RenderApp<qt::WidgetManager, Canvas>;

    static_assert(
        RenderAppConcept<RenderAppQ>,
        "RenderApp does not satisfy the RenderAppConcept");
    static_assert(
        RenderAppConcept<const RenderAppQ>,
        "const RenderApp does not satisfy the RenderAppConcept");
    static_assert(
        RenderAppConcept<RenderAppQ&>,
        "RenderApp& does not satisfy the RenderAppConcept");
    static_assert(
        RenderAppConcept<const RenderAppQ&>,
        "const RenderApp& does not satisfy the RenderAppConcept");
    static_assert(
        RenderAppConcept<RenderAppQ&&>,
        "RenderApp&& does not satisfy the RenderAppConcept");
#endif

#ifdef VCLIB_WITH_GLFW
    using RenderAppG = RenderApp<glfw::WindowManager, Canvas>;

    static_assert(
        RenderAppConcept<RenderAppG>,
        "RenderApp does not satisfy the RenderAppConcept");
    static_assert(
        RenderAppConcept<const RenderAppG>,
        "const RenderApp does not satisfy the RenderAppConcept");
    static_assert(
        RenderAppConcept<RenderAppG&>,
        "RenderApp& does not satisfy the RenderAppConcept");
    static_assert(
        RenderAppConcept<const RenderAppG&>,
        "const RenderApp& does not satisfy the RenderAppConcept");
    static_assert(
        RenderAppConcept<RenderAppG&&>,
        "RenderApp&& does not satisfy the RenderAppConcept");
#endif
}

#endif // RENDER_APP_H
