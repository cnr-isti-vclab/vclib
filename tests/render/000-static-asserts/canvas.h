/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef CANVAS_H
#define CANVAS_H

#include <vclib/render/canvas.h>
#include <vclib/render/render_app.h>

#ifdef VCLIB_WITH_QT
#include <vclib/qt/widget_manager.h>
#elif VCLIB_WITH_GLFW
#include <vclib/glfw/window_manager.h>
#endif

#ifdef VCLIB_WITH_QT
template<typename DR>
using WM = vcl::qt::WidgetManager<DR>;
#elif VCLIB_WITH_GLFW
template<typename DR>
using WM = vcl::glfw::WindowManager<DR>;
#endif

void canvasStaticAsserts()
{
    using namespace vcl;

    using RenderAppType = RenderApp<WM, Canvas>;

    static_assert(
        CanvasConcept<Canvas<RenderAppType>>,
        "Canvas does not satisfy the CanvasConcept");
    static_assert(
        CanvasConcept<const Canvas<RenderAppType>>,
        "const Canvas does not satisfy the CanvasConcept");
    static_assert(
        CanvasConcept<Canvas<RenderAppType>&>,
        "Canvas& does not satisfy the CanvasConcept");
    static_assert(
        CanvasConcept<const Canvas<RenderAppType>&>,
        "const Canvas& does not satisfy the CanvasConcept");
    static_assert(
        CanvasConcept<Canvas<RenderAppType>&&>,
        "Canvas&& does not satisfy the CanvasConcept");
}

#endif // CANVAS_H