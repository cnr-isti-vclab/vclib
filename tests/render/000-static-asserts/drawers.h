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

#ifndef DRAWERS_H
#define DRAWERS_H

#include <vclib/render/canvas.h>
#include <vclib/render/render_app.h>

#ifdef VCLIB_WITH_QT
#include <vclib/qt/widget_manager.h>
#endif
#ifdef VCLIB_WITH_GLFW
#include <vclib/glfw/window_manager.h>
#endif

#include <vclib/render/drawers/event_drawer.h>
#include <vclib/render/drawers/plain_drawer.h>
#include <vclib/render/drawers/viewer_drawer.h>

#ifdef VCLIB_WITH_IMGUI
#include <vclib/imgui/imgui_drawer.h>
#endif

#ifdef VCLIB_RENDER_BACKEND_BGFX
#include <vclib/bgfx/drawers/text_drawer.h>
#endif

#ifdef VCLIB_WITH_QT
template<typename DR>
using WMQ = vcl::qt::WidgetManager<DR>;
#endif
#if VCLIB_WITH_GLFW
template<typename DR>
using WMG = vcl::glfw::WindowManager<DR>;
#endif

// generic static asserts for drawers that should work with any window manager
template<template<typename> typename WM>
void drawersStaticAssertsWM()
{
    using namespace vcl;

    using RenderAppPD  = RenderApp<WM, Canvas, PlainDrawer>;
    using RenderAppED  = RenderApp<WM, Canvas, EventDrawer>;
    using RenderAppBED = RenderApp<WM, Canvas, BlockerEventDrawer>;
    using RenderAppVD  = RenderApp<WM, Canvas, ViewerDrawer>;

    // PlainDrawer
    static_assert(
        DrawerConcept<PlainDrawer<RenderAppPD>>,
        "PlainDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const PlainDrawer<RenderAppPD>>,
        "const PlainDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<PlainDrawer<RenderAppPD>&>,
        "PlainDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const PlainDrawer<RenderAppPD>&>,
        "const PlainDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<PlainDrawer<RenderAppPD>&&>,
        "PlainDrawer&& does not satisfy the DrawerConcept");

    static_assert(
        !EventDrawerConcept<PlainDrawer<RenderAppPD>>,
        "PlainDrawer does satisfy the EventDrawerConcept");
    static_assert(
        !EventDrawerConcept<const PlainDrawer<RenderAppPD>>,
        "const PlainDrawer does satisfy the EventDrawerConcept");
    static_assert(
        !EventDrawerConcept<PlainDrawer<RenderAppPD>&>,
        "PlainDrawer& does satisfy the EventDrawerConcept");
    static_assert(
        !EventDrawerConcept<const PlainDrawer<RenderAppPD>&>,
        "const PlainDrawer& does satisfy the EventDrawerConcept");
    static_assert(
        !EventDrawerConcept<PlainDrawer<RenderAppPD>&&>,
        "PlainDrawer&& does satisfy the EventDrawerConcept");

    // EventDrawer
    static_assert(
        DrawerConcept<EventDrawer<RenderAppED>>,
        "EventDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const EventDrawer<RenderAppED>>,
        "const EventDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<EventDrawer<RenderAppED>&>,
        "EventDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const EventDrawer<RenderAppED>&>,
        "const EventDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<EventDrawer<RenderAppED>&&>,
        "EventDrawer&& does not satisfy the DrawerConcept");

    static_assert(
        EventDrawerConcept<EventDrawer<RenderAppED>>,
        "EventDrawer does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<const EventDrawer<RenderAppED>>,
        "const EventDrawer does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<EventDrawer<RenderAppED>&>,
        "EventDrawer& does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<const EventDrawer<RenderAppED>&>,
        "const EventDrawer& does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<EventDrawer<RenderAppED>&&>,
        "EventDrawer&& does not satisfy the EventDrawerConcept");

    static_assert(
        !BlockerEventDrawerConcept<EventDrawer<RenderAppED>>,
        "EventDrawer does satisfy the BlockerEventDrawerConcept");
    static_assert(
        !BlockerEventDrawerConcept<const EventDrawer<RenderAppED>>,
        "const EventDrawer does satisfy the BlockerEventDrawerConcept");
    static_assert(
        !BlockerEventDrawerConcept<EventDrawer<RenderAppED>&>,
        "EventDrawer& does satisfy the BlockerEventDrawerConcept");
    static_assert(
        !BlockerEventDrawerConcept<const EventDrawer<RenderAppED>&>,
        "const EventDrawer& does satisfy the BlockerEventDrawerConcept");
    static_assert(
        !BlockerEventDrawerConcept<EventDrawer<RenderAppED>&&>,
        "EventDrawer&& does satisfy the BlockerEventDrawerConcept");

    // BlockerEventDrawer
    static_assert(
        DrawerConcept<BlockerEventDrawer<RenderAppBED>>,
        "BlockerEventDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const BlockerEventDrawer<RenderAppBED>>,
        "const BlockerEventDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<BlockerEventDrawer<RenderAppBED>&>,
        "BlockerEventDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const BlockerEventDrawer<RenderAppBED>&>,
        "const BlockerEventDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<BlockerEventDrawer<RenderAppBED>&&>,
        "BlockerEventDrawer&& does not satisfy the DrawerConcept");

    static_assert(
        EventDrawerConcept<BlockerEventDrawer<RenderAppBED>>,
        "BlockerEventDrawer does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<const BlockerEventDrawer<RenderAppBED>>,
        "const BlockerEventDrawer does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<BlockerEventDrawer<RenderAppBED>&>,
        "BlockerEventDrawer& does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<const BlockerEventDrawer<RenderAppBED>&>,
        "const BlockEventDrawer& does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<BlockerEventDrawer<RenderAppBED>&&>,
        "BlockerEventDrawer&& does not satisfy the EventDrawerConcept");

    static_assert(
        BlockerEventDrawerConcept<BlockerEventDrawer<RenderAppBED>>,
        "BlockerEventDrawer does not satisfy the BlockerEventDrawerConcept");
    static_assert(
        BlockerEventDrawerConcept<const BlockerEventDrawer<RenderAppBED>>,
        "const BlockerEventDrawer does not satisfy the "
        "BlockerEventDrawerConcept");
    static_assert(
        BlockerEventDrawerConcept<BlockerEventDrawer<RenderAppBED>&>,
        "BlockerEventDrawer& does not satisfy the BlockerEventDrawerConcept");
    static_assert(
        BlockerEventDrawerConcept<const BlockerEventDrawer<RenderAppBED>&>,
        "const BlockerEventDrawer& does not satisfy the "
        "BlockerEventDrawerConcept");
    static_assert(
        BlockerEventDrawerConcept<BlockerEventDrawer<RenderAppBED>&&>,
        "BlockerEventDrawer&& does not satisfy the BlockerEventDrawerConcept");

    // ViewerDrawer
    static_assert(
        DrawerConcept<ViewerDrawer<RenderAppVD>>,
        "ViewerDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const ViewerDrawer<RenderAppVD>>,
        "const ViewerDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<ViewerDrawer<RenderAppVD>&>,
        "ViewerDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const ViewerDrawer<RenderAppVD>&>,
        "const ViewerDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<ViewerDrawer<RenderAppVD>&&>,
        "ViewerDrawer&& does not satisfy the DrawerConcept");

    static_assert(
        EventDrawerConcept<ViewerDrawer<RenderAppVD>>,
        "ViewerDrawer does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<const ViewerDrawer<RenderAppVD>>,
        "const ViewerDrawer does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<ViewerDrawer<RenderAppVD>&>,
        "ViewerDrawer& does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<const ViewerDrawer<RenderAppVD>&>,
        "const ViewerDrawer& does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<ViewerDrawer<RenderAppVD>&&>,
        "ViewerDrawer&& does not satisfy the EventDrawerConcept");

    static_assert(
        !BlockerEventDrawerConcept<ViewerDrawer<RenderAppVD>>,
        "ViewerDrawer does satisfy the BlockerEventDrawerConcept");
    static_assert(
        !BlockerEventDrawerConcept<const ViewerDrawer<RenderAppVD>>,
        "const ViewerDrawer does satisfy the BlockerEventDrawerConcept");
    static_assert(
        !BlockerEventDrawerConcept<ViewerDrawer<RenderAppVD>&>,
        "ViewerDrawer& does satisfy the BlockerEventDrawerConcept");
    static_assert(
        !BlockerEventDrawerConcept<const ViewerDrawer<RenderAppVD>&>,
        "const ViewerDrawer& does satisfy the BlockerEventDrawerConcept");
    static_assert(
        !BlockerEventDrawerConcept<ViewerDrawer<RenderAppVD>&&>,
        "ViewerDrawer&& does satisfy the BlockerEventDrawerConcept");

#ifdef VCLIB_WITH_IMGUI
    using namespace vcl::imgui;

    using RenderAppID = RenderApp<WM, Canvas, ImGuiDrawer>;

    // ImguiDrawer
    static_assert(
        DrawerConcept<ImGuiDrawer<RenderAppID>>,
        "ImguiDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const ImGuiDrawer<RenderAppID>>,
        "const ImguiDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<ImGuiDrawer<RenderAppID>&>,
        "ImguiDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const ImGuiDrawer<RenderAppID>&>,
        "const ImguiDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<ImGuiDrawer<RenderAppID>&&>,
        "ImguiDrawer&& does not satisfy the DrawerConcept");

    static_assert(
        EventDrawerConcept<ImGuiDrawer<RenderAppID>>,
        "ImguiDrawer does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<const ImGuiDrawer<RenderAppID>>,
        "const ImguiDrawer does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<ImGuiDrawer<RenderAppID>&>,
        "ImguiDrawer& does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<const ImGuiDrawer<RenderAppID>&>,
        "const ImguiDrawer& does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<ImGuiDrawer<RenderAppID>&&>,
        "ImguiDrawer&& does not satisfy the EventDrawerConcept");

    static_assert(
        BlockerEventDrawerConcept<ImGuiDrawer<RenderAppID>>,
        "ImguiDrawer does not satisfy the BlockerEventDrawerConcept");
    static_assert(
        BlockerEventDrawerConcept<const ImGuiDrawer<RenderAppID>>,
        "const ImguiDrawer does not satisfy the BlockerEventDrawerConcept");
    static_assert(
        BlockerEventDrawerConcept<ImGuiDrawer<RenderAppID>&>,
        "ImguiDrawer& does not satisfy the BlockerEventDrawerConcept");
    static_assert(
        BlockerEventDrawerConcept<const ImGuiDrawer<RenderAppID>&>,
        "const ImguiDrawer& does not satisfy the BlockerEventDrawerConcept");
    static_assert(
        BlockerEventDrawerConcept<ImGuiDrawer<RenderAppID>&&>,
        "ImguiDrawer&& does not satisfy the BlockerEventDrawerConcept");
#endif

#ifdef VCLIB_RENDER_BACKEND_BGFX
    using RenderAppTD = RenderApp<WM, Canvas, TextDrawer>;

    // TextDrawer
    static_assert(
        DrawerConcept<TextDrawer<RenderAppTD>>,
        "TextDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const TextDrawer<RenderAppTD>>,
        "const TextDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<TextDrawer<RenderAppTD>&>,
        "TextDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const TextDrawer<RenderAppTD>&>,
        "const TextDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<TextDrawer<RenderAppTD>&&>,
        "TextDrawer&& does not satisfy the DrawerConcept");

    static_assert(
        !EventDrawerConcept<TextDrawer<RenderAppTD>>,
        "TextDrawer does satisfy the EventDrawerConcept");
    static_assert(
        !EventDrawerConcept<const TextDrawer<RenderAppTD>>,
        "const TextDrawer does satisfy the EventDrawerConcept");
    static_assert(
        !EventDrawerConcept<TextDrawer<RenderAppTD>&>,
        "TextDrawer& does satisfy the EventDrawerConcept");
    static_assert(
        !EventDrawerConcept<const TextDrawer<RenderAppTD>&>,
        "const TextDrawer& does satisfy the EventDrawerConcept");
    static_assert(
        !EventDrawerConcept<TextDrawer<RenderAppTD>&&>,
        "TextDrawer&& does satisfy the EventDrawerConcept");
#endif
}

void drawersStaticAsserts()
{
    using namespace vcl;

    // static asserts for each window manager
#ifdef VCLIB_WITH_QT
    drawersStaticAssertsWM<WMQ>();
#endif

#if VCLIB_WITH_GLFW
    drawersStaticAssertsWM<WMG>();
#endif
}

#endif // DRAWERS_H
