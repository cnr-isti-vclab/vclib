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

#include <vclib/render/canvas.h>
#include <vclib/render/renderer.h>

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

void rendererStaticAsserts()
{
    using namespace vcl;

#ifdef VCLIB_WITH_QT
    using RendererTypeQ = Renderer<qt::WidgetManager, Canvas>;

    static_assert(
        RendererConcept<RendererTypeQ>,
        "Renderer does not satisfy the RendererConcept");
    static_assert(
        RendererConcept<const RendererTypeQ>,
        "const Renderer does not satisfy the RendererConcept");
    static_assert(
        RendererConcept<RendererTypeQ&>,
        "Renderer& does not satisfy the RendererConcept");
    static_assert(
        RendererConcept<const RendererTypeQ&>,
        "const Renderer& does not satisfy the RendererConcept");
    static_assert(
        RendererConcept<RendererTypeQ&&>,
        "Renderer&& does not satisfy the RendererConcept");
#endif

#ifdef VCLIB_WITH_GLFW
    using RendererTypeG = Renderer<glfw::WindowManager, Canvas>;

    static_assert(
        RendererConcept<RendererTypeG>,
        "Renderer does not satisfy the RendererConcept");
    static_assert(
        RendererConcept<const RendererTypeG>,
        "const Renderer does not satisfy the RendererConcept");
    static_assert(
        RendererConcept<RendererTypeG&>,
        "Renderer& does not satisfy the RendererConcept");
    static_assert(
        RendererConcept<const RendererTypeG&>,
        "const Renderer& does not satisfy the RendererConcept");
    static_assert(
        RendererConcept<RendererTypeG&&>,
        "Renderer&& does not satisfy the RendererConcept");
#endif
}

#endif // RENDERER_H
