// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VIEW_PROJECTION_H
#define VIEW_PROJECTION_H

#include <vclib/render/canvas.h>
#include <vclib/render/concepts/view_projection.h>
#include <vclib/render/drawers/trackball_event_drawer.h>
#include <vclib/render/render_app.h>

#ifdef VCLIB_WITH_QT
#include <vclib/qt/widget_manager.h>
#endif
#ifdef VCLIB_WITH_GLFW
#include <vclib/glfw/window_manager.h>
#endif // VCLIB_WITH_GLFW

#ifdef VCLIB_WITH_QT
template<typename DR>
using WMQ = vcl::qt::WidgetManager<DR>;
#endif

#ifdef VCLIB_WITH_GLFW
template<typename DR>
using WMG = vcl::glfw::WindowManager<DR>;
#endif // VCLIB_WITH_GLFW

template<template<typename> typename WM>
void viewProjectionStaticAssertsWM()
{
    using namespace vcl;

    using RenderAppT = RenderApp<WM, Canvas, TrackBallEventDrawer>;

    static_assert(
        ViewProjectionConcept<TrackBallEventDrawer<RenderAppT>>,
        "TrackBallEventDrawer does not satisfy the ViewProjectionConcept");
    static_assert(
        ViewProjectionConcept<const TrackBallEventDrawer<RenderAppT>>,
        "const TrackBallEventDrawer does not satisfy the "
        "ViewProjectionConcept");
    static_assert(
        ViewProjectionConcept<TrackBallEventDrawer<RenderAppT>&>,
        "TrackBallEventDrawer& does not satisfy the ViewProjectionConcept");
    static_assert(
        ViewProjectionConcept<const TrackBallEventDrawer<RenderAppT>&>,
        "const TrackBallEventDrawer& does not satisfy the "
        "ViewProjectionConcept");
    static_assert(
        ViewProjectionConcept<TrackBallEventDrawer<RenderAppT>&&>,
        "TrackBallEventDrawer&& does not satisfy the ViewProjectionConcept");
}

void viewProjectionStaticAsserts()
{
    using namespace vcl;

    // static asserts for each window manager
#ifdef VCLIB_WITH_QT
    viewProjectionStaticAssertsWM<WMQ>();
#endif

#if VCLIB_WITH_GLFW
    viewProjectionStaticAssertsWM<WMG>();
#endif
}

#endif // VIEW_PROJECTION_H
