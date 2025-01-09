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
#endif
#ifdef VCLIB_WITH_GLFW
#include <vclib/glfw/window_manager.h>
#endif

#include <vclib/render/drawers/plain_drawer.h>
#include <vclib/render/drawers/event_drawer.h>
#include <vclib/render/drawers/viewer_drawer.h>

#ifdef VCLIB_WITH_IMGUI
#include <vclib/imgui/imgui_drawer.h>
#endif

#ifdef VCLIB_RENDER_BACKEND_BGFX
#include <vclib/bgfx/drawers/text_drawer.h>
#endif

template<typename DR>
using BlockEventDrawer = vcl::EventDrawer<DR, true>;

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

    using RendererTypePD  = Renderer<WM, Canvas, PlainDrawer>;
    using RendererTypeED  = Renderer<WM, Canvas, EventDrawer>;
    using RendererTypeBED = Renderer<WM, Canvas, BlockEventDrawer>;
    using RendererTypeVD  = Renderer<WM, Canvas, ViewerDrawer>;

    // PlainDrawer
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
    static_assert(
        !EventDrawerConcept<const PlainDrawer<RendererTypePD>>,
        "const PlainDrawer does satisfy the EventDrawerConcept");
    static_assert(
        !EventDrawerConcept<PlainDrawer<RendererTypePD>&>,
        "PlainDrawer& does satisfy the EventDrawerConcept");
    static_assert(
        !EventDrawerConcept<const PlainDrawer<RendererTypePD>&>,
        "const PlainDrawer& does satisfy the EventDrawerConcept");
    static_assert(
        !EventDrawerConcept<PlainDrawer<RendererTypePD>&&>,
        "PlainDrawer&& does satisfy the EventDrawerConcept");

    // EventDrawer
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
        CantBlockEventDrawerConcept<EventDrawer<RendererTypeED>>,
        "EventDrawer does not satisfy the CantBlockEventDrawerConcept");
    static_assert(
        CantBlockEventDrawerConcept<const EventDrawer<RendererTypeED>>,
        "const EventDrawer does not satisfy the CantBlockEventDrawerConcept");
    static_assert(
        CantBlockEventDrawerConcept<EventDrawer<RendererTypeED>&>,
        "EventDrawer& does not satisfy the CantBlockEventDrawerConcept");
    static_assert(
        CantBlockEventDrawerConcept<const EventDrawer<RendererTypeED>&>,
        "const EventDrawer& does not satisfy the CantBlockEventDrawerConcept");
    static_assert(
        CantBlockEventDrawerConcept<EventDrawer<RendererTypeED>&&>,
        "EventDrawer&& does not satisfy the CantBlockEventDrawerConcept");

    static_assert(
        !CanBlockEventDrawerConcept<EventDrawer<RendererTypeED>>,
        "EventDrawer does satisfy the CanBlockEventDrawerConcept");
    static_assert(
        !CanBlockEventDrawerConcept<const EventDrawer<RendererTypeED>>,
        "const EventDrawer does satisfy the CanBlockEventDrawerConcept");
    static_assert(
        !CanBlockEventDrawerConcept<EventDrawer<RendererTypeED>&>,
        "EventDrawer& does satisfy the CanBlockEventDrawerConcept");
    static_assert(
        !CanBlockEventDrawerConcept<const EventDrawer<RendererTypeED>&>,
        "const EventDrawer& does satisfy the CanBlockEventDrawerConcept");
    static_assert(
        !CanBlockEventDrawerConcept<EventDrawer<RendererTypeED>&&>,
        "EventDrawer&& does satisfy the CanBlockEventDrawerConcept");

    // BlockEventDrawer
    static_assert(
        DrawerConcept<BlockEventDrawer<RendererTypeBED>>,
        "BlockEventDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const BlockEventDrawer<RendererTypeBED>>,
        "const BlockEventDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<BlockEventDrawer<RendererTypeBED>&>,
        "BlockEventDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const BlockEventDrawer<RendererTypeBED>&>,
        "const BlockEventDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<BlockEventDrawer<RendererTypeBED>&&>,
        "BlockEventDrawer&& does not satisfy the DrawerConcept");

    static_assert(
        EventDrawerConcept<BlockEventDrawer<RendererTypeBED>>,
        "BlockEventDrawer does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<const BlockEventDrawer<RendererTypeBED>>,
        "const BlockEventDrawer does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<BlockEventDrawer<RendererTypeBED>&>,
        "BlockEventDrawer& does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<const BlockEventDrawer<RendererTypeBED>&>,
        "const BlockEventDrawer& does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<BlockEventDrawer<RendererTypeBED>&&>,
        "BlockEventDrawer&& does not satisfy the EventDrawerConcept");

    static_assert(
        !CantBlockEventDrawerConcept<BlockEventDrawer<RendererTypeBED>>,
        "BlockEventDrawer does satisfy the CantBlockEventDrawerConcept");
    static_assert(
        !CantBlockEventDrawerConcept<const BlockEventDrawer<RendererTypeBED>>,
        "const BlockEventDrawer does satisfy the CantBlockEventDrawerConcept");
    static_assert(
        !CantBlockEventDrawerConcept<BlockEventDrawer<RendererTypeBED>&>,
        "BlockEventDrawer& does satisfy the CantBlockEventDrawerConcept");
    static_assert(
        !CantBlockEventDrawerConcept<const BlockEventDrawer<RendererTypeBED>&>,
        "const BlockEventDrawer& does satisfy the CantBlockEventDrawerConcept");
    static_assert(
        !CantBlockEventDrawerConcept<BlockEventDrawer<RendererTypeBED>&&>,
        "BlockEventDrawer&& does satisfy the CantBlockEventDrawerConcept");

    static_assert(
        CanBlockEventDrawerConcept<BlockEventDrawer<RendererTypeBED>>,
        "BlockEventDrawer does not satisfy the CanBlockEventDrawerConcept");
    static_assert(
        CanBlockEventDrawerConcept<const BlockEventDrawer<RendererTypeBED>>,
        "const BlockEventDrawer does not satisfy the CanBlockEventDrawerConcept");
    static_assert(
        CanBlockEventDrawerConcept<BlockEventDrawer<RendererTypeBED>&>,
        "BlockEventDrawer& does not satisfy the CanBlockEventDrawerConcept");
    static_assert(
        CanBlockEventDrawerConcept<const BlockEventDrawer<RendererTypeBED>&>,
        "const BlockEventDrawer& does not satisfy the CanBlockEventDrawerConcept");
    static_assert(
        CanBlockEventDrawerConcept<BlockEventDrawer<RendererTypeBED>&&>,
        "BlockEventDrawer&& does not satisfy the CanBlockEventDrawerConcept");

    // ViewerDrawer
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

    static_assert(
        CantBlockEventDrawerConcept<ViewerDrawer<RendererTypeVD>>,
        "ViewerDrawer does not satisfy the CantBlockEventDrawerConcept");
    static_assert(
        CantBlockEventDrawerConcept<const ViewerDrawer<RendererTypeVD>>,
        "const ViewerDrawer does not satisfy the CantBlockEventDrawerConcept");
    static_assert(
        CantBlockEventDrawerConcept<ViewerDrawer<RendererTypeVD>&>,
        "ViewerDrawer& does not satisfy the CantBlockEventDrawerConcept");
    static_assert(
        CantBlockEventDrawerConcept<const ViewerDrawer<RendererTypeVD>&>,
        "const ViewerDrawer& does not satisfy the CantBlockEventDrawerConcept");
    static_assert(
        CantBlockEventDrawerConcept<ViewerDrawer<RendererTypeVD>&&>,
        "ViewerDrawer&& does not satisfy the CantBlockEventDrawerConcept");

    static_assert(
        !CanBlockEventDrawerConcept<ViewerDrawer<RendererTypeVD>>,
        "ViewerDrawer does satisfy the CanBlockEventDrawerConcept");
    static_assert(
        !CanBlockEventDrawerConcept<const ViewerDrawer<RendererTypeVD>>,
        "const ViewerDrawer does satisfy the CanBlockEventDrawerConcept");
    static_assert(
        !CanBlockEventDrawerConcept<ViewerDrawer<RendererTypeVD>&>,
        "ViewerDrawer& does satisfy the CanBlockEventDrawerConcept");
    static_assert(
        !CanBlockEventDrawerConcept<const ViewerDrawer<RendererTypeVD>&>,
        "const ViewerDrawer& does satisfy the CanBlockEventDrawerConcept");
    static_assert(
        !CanBlockEventDrawerConcept<ViewerDrawer<RendererTypeVD>&&>,
        "ViewerDrawer&& does satisfy the CanBlockEventDrawerConcept");

#ifdef VCLIB_RENDER_BACKEND_BGFX
    using RendererTypeTD = Renderer<WMQ, Canvas, TextDrawer>;

    // TextDrawer
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
    static_assert(
        !EventDrawerConcept<const TextDrawer<RendererTypeTD>>,
        "const TextDrawer does satisfy the EventDrawerConcept");
    static_assert(
        !EventDrawerConcept<TextDrawer<RendererTypeTD>&>,
        "TextDrawer& does satisfy the EventDrawerConcept");
    static_assert(
        !EventDrawerConcept<const TextDrawer<RendererTypeTD>&>,
        "const TextDrawer& does satisfy the EventDrawerConcept");
    static_assert(
        !EventDrawerConcept<TextDrawer<RendererTypeTD>&&>,
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

    // static assers for drawers that work with specific window managers
#ifdef VCLIB_WITH_IMGUI
    using namespace vcl::imgui;

    using RendererTypeID = Renderer<WMG, Canvas, ImGuiDrawer>;

    // ImguiDrawer
    static_assert(
        DrawerConcept<ImGuiDrawer<RendererTypeID>>,
        "ImguiDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const ImGuiDrawer<RendererTypeID>>,
        "const ImguiDrawer does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<ImGuiDrawer<RendererTypeID>&>,
        "ImguiDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<const ImGuiDrawer<RendererTypeID>&>,
        "const ImguiDrawer& does not satisfy the DrawerConcept");
    static_assert(
        DrawerConcept<ImGuiDrawer<RendererTypeID>&&>,
        "ImguiDrawer&& does not satisfy the DrawerConcept");

    static_assert(
        EventDrawerConcept<ImGuiDrawer<RendererTypeID>>,
        "ImguiDrawer does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<const ImGuiDrawer<RendererTypeID>>,
        "const ImguiDrawer does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<ImGuiDrawer<RendererTypeID>&>,
        "ImguiDrawer& does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<const ImGuiDrawer<RendererTypeID>&>,
        "const ImguiDrawer& does not satisfy the EventDrawerConcept");
    static_assert(
        EventDrawerConcept<ImGuiDrawer<RendererTypeID>&&>,
        "ImguiDrawer&& does not satisfy the EventDrawerConcept");

    static_assert(
        CanBlockEventDrawerConcept<ImGuiDrawer<RendererTypeID>>,
        "ImguiDrawer does not satisfy the CanBlockEventDrawerConcept");
    static_assert(
        CanBlockEventDrawerConcept<const ImGuiDrawer<RendererTypeID>>,
        "const ImguiDrawer does not satisfy the CanBlockEventDrawerConcept");
    static_assert(
        CanBlockEventDrawerConcept<ImGuiDrawer<RendererTypeID>&>,
        "ImguiDrawer& does not satisfy the CanBlockEventDrawerConcept");
    static_assert(
        CanBlockEventDrawerConcept<const ImGuiDrawer<RendererTypeID>&>,
        "const ImguiDrawer& does not satisfy the CanBlockEventDrawerConcept");
    static_assert(
        CanBlockEventDrawerConcept<ImGuiDrawer<RendererTypeID>&&>,
        "ImguiDrawer&& does not satisfy the CanBlockEventDrawerConcept");

    static_assert(
        !CantBlockEventDrawerConcept<ImGuiDrawer<RendererTypeID>>,
        "ImguiDrawer does satisfy the CantBlockEventDrawerConcept");
    static_assert(
        !CantBlockEventDrawerConcept<const ImGuiDrawer<RendererTypeID>>,
        "const ImguiDrawer does satisfy the CantBlockEventDrawerConcept");
    static_assert(
        !CantBlockEventDrawerConcept<ImGuiDrawer<RendererTypeID>&>,
        "ImguiDrawer& does satisfy the CantBlockEventDrawerConcept");
    static_assert(
        !CantBlockEventDrawerConcept<const ImGuiDrawer<RendererTypeID>&>,
        "const ImguiDrawer& does satisfy the CantBlockEventDrawerConcept");
    static_assert(
        !CantBlockEventDrawerConcept<ImGuiDrawer<RendererTypeID>&&>,
        "ImguiDrawer&& does satisfy the CantBlockEventDrawerConcept");
#endif
}

#endif // DRAWERS_H
