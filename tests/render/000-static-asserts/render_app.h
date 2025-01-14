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
