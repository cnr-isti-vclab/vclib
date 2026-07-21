// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
