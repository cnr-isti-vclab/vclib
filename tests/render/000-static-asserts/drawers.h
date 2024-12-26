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

#ifndef DRAWERS_H
#define DRAWERS_H

#include <vclib/render/canvas.h>
#include <vclib/render/renderer.h>

#ifdef VCLIB_WITH_QT
#include <vclib/qt/widget_manager.h>
#elif VCLIB_WITH_GLFW
#include <vclib/glfw/window_manager.h>
#endif

#include <vclib/render/drawers/plain_drawer.h>
#include <vclib/render/drawers/event_drawer.h>
#include <vclib/render/drawers/viewer_drawer.h>

#ifdef VCLIB_RENDER_BACKEND_BGFX
#include <vclib/bgfx/drawers/text_drawer.h>
#endif

#ifdef VCLIB_WITH_QT
template<typename DR>
using WM = vcl::qt::WidgetManager<DR>;
#elif VCLIB_WITH_GLFW
template<typename DR>
using WM = vcl::glfw::WindowManager<DR>;
#endif

void drawersStaticAsserts()
{
    using namespace vcl;

    using RendererTypePD = Renderer<WM, Canvas, PlainDrawer>;
    using RendererTypeED = Renderer<WM, Canvas, EventDrawer>;
    using RendererTypeVD = Renderer<WM, Canvas, ViewerDrawer>;

    static_assert(
        DrawerConcept<PlainDrawer<RendererTypePD>>,
        "PlainDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const PlainDrawer<RendererTypePD>>,
        "const PlainDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<PlainDrawer<RendererTypePD>&>,
        "PlainDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const PlainDrawer<RendererTypePD>&>,
        "const PlainDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<PlainDrawer<RendererTypePD>&&>,
        "PlainDrawer&& does not satisfy the DrawerConcept");

    static_assert(
        !EventDrawerConcept<PlainDrawer<RendererTypePD>>,
        "PlainDrawer does satisfy the EventDrawerConcept");
    // static_assert(
    //     !EventDrawerConcept<const PlainDrawer<RendererTypePD>>,
    //     "const PlainDrawer does satisfy the EventDrawerConcept");
    static_assert(
        !EventDrawerConcept<PlainDrawer<RendererTypePD>&>,
        "PlainDrawer& does satisfy the EventDrawerConcept");
    // static_assert(
    //     !EventDrawerConcept<const PlainDrawer<RendererTypePD>&>,
    //     "const PlainDrawer& does satisfy the EventDrawerConcept");
    static_assert(
        !EventDrawerConcept<PlainDrawer<RendererTypePD>&&>,
        "PlainDrawer&& does satisfy the EventDrawerConcept");

    static_assert(
        DrawerConcept<EventDrawer<RendererTypeED>>,
        "EventDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const EventDrawer<RendererTypeED>>,
        "const EventDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<EventDrawer<RendererTypeED>&>,
        "EventDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const EventDrawer<RendererTypeED>&>,
        "const EventDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<EventDrawer<RendererTypeED>&&>,
        "EventDrawer&& does not satisfy the DrawerConcept");

    static_assert(
        EventDrawerConcept<EventDrawer<RendererTypeED>>,
        "EventDrawer does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<const EventDrawer<RendererTypeED>>,
        "const EventDrawer does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<EventDrawer<RendererTypeED>&>,
        "EventDrawer& does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<const EventDrawer<RendererTypeED>&>,
        "const EventDrawer& does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<EventDrawer<RendererTypeED>&&>,
        "EventDrawer&& does not satisfy the EventDrawerConcept");

    static_assert(
        DrawerConcept<ViewerDrawer<RendererTypeVD>>,
        "ViewerDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const ViewerDrawer<RendererTypeVD>>,
        "const ViewerDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<ViewerDrawer<RendererTypeVD>&>,
        "ViewerDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const ViewerDrawer<RendererTypeVD>&>,
        "const ViewerDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<ViewerDrawer<RendererTypeVD>&&>,
        "ViewerDrawer&& does not satisfy the DrawerConcept");

    static_assert(
        EventDrawerConcept<ViewerDrawer<RendererTypeVD>>,
        "ViewerDrawer does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<const ViewerDrawer<RendererTypeVD>>,
        "const ViewerDrawer does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<ViewerDrawer<RendererTypeVD>&>,
        "ViewerDrawer& does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<const ViewerDrawer<RendererTypeVD>&>,
        "const ViewerDrawer& does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<ViewerDrawer<RendererTypeVD>&&>,
        "ViewerDrawer&& does not satisfy the EventDrawerConcept");

#ifdef VCLIB_RENDER_BACKEND_BGFX
    using RendererTypeTD = Renderer<WM, Canvas, TextDrawer>;

    static_assert(
        DrawerConcept<TextDrawer<RendererTypeTD>>,
        "TextDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const TextDrawer<RendererTypeTD>>,
        "const TextDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<TextDrawer<RendererTypeTD>&>,
        "TextDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const TextDrawer<RendererTypeTD>&>,
        "const TextDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<TextDrawer<RendererTypeTD>&&>,
        "TextDrawer&& does not satisfy the DrawerConcept");

    static_assert(
        !EventDrawerConcept<TextDrawer<RendererTypeTD>>,
        "TextDrawer does satisfy the EventDrawerConcept");
    // static_assert(
    //     !EventDrawerConcept<const TextDrawer<RendererTypeTD>>,
    //     "const TextDrawer does satisfy the EventDrawerConcept");
    static_assert(
        !EventDrawerConcept<TextDrawer<RendererTypeTD>&>,
        "TextDrawer& does satisfy the EventDrawerConcept");
    // static_assert(
    //     !EventDrawerConcept<const TextDrawer<RendererTypeTD>&>,
    //     "const TextDrawer& does satisfy the EventDrawerConcept");
    static_assert(
        !EventDrawerConcept<TextDrawer<RendererTypeTD>&&>,
        "TextDrawer&& does satisfy the EventDrawerConcept");
#endif
}

#endif // DRAWERS_H
