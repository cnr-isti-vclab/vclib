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
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_EXT_BGFX_CONTEXT_H
#define VCL_EXT_BGFX_CONTEXT_H

#include <stack>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#ifdef QT_WIDGETS_LIB
#include <QApplication>
#include <QWidget>
#endif

namespace vcl::bgf {

class Context
{
    void* windowHandle = nullptr;
    void* displayHandle = nullptr;

    std::stack<bgfx::ViewId> viewStack;

#ifdef QT_WIDGETS_LIB
    QWidget* widget = nullptr;
#endif

public:
    static Context& instance()
    {
        static Context ctx;
        return ctx;
    }

    static bgfx::ViewId requestViewId()
    {
        bgfx::ViewId viewId = instance().viewStack.top();
        instance().viewStack.pop();
        return viewId;
    }

    static void releaseViewId(bgfx::ViewId viewId)
    {
        instance().viewStack.push(viewId);
    }

private:
    Context()
    {
#ifdef QT_WIDGETS_LIB
        widget = new QWidget();
        widget->resize(1,1);
        windowHandle = (void*)widget->winId();
#ifdef Q_OS_LINUX
        /// THIS WORKS ONLY IF QT_QPA_PLATFORM = xcb
        QNativeInterface::QX11Application* x11AppInfo =
            qApp->nativeInterface<QNativeInterface::QX11Application>();
        if (x11AppInfo) {
            displayHandle = x11AppInfo->display();
        }
        else {
            QNativeInterface::QWaylandApplication* wayAppInfo =
                qApp->nativeInterface<QNativeInterface::QWaylandApplication>();
            if (wayAppInfo) {
                displayHandle = wayAppInfo->display();
            }
            else {
                exit(-1);
            }
        }
#endif // Q_OS_LINUX

#else // QT_WIDGETS_LIB
        static_assert(
            0, "No available library that allows to create a window.");
#endif // QT_WIDGETS_LIB

#ifdef __APPLE__
        bgfx::renderFrame(); // needed for macos
#endif // __APPLE__

        bgfx::Init init;
        init.platformData.nwh = windowHandle;
        init.type             = bgfx::RendererType::Count;
        init.platformData.ndt = displayHandle;
        init.resolution.width  = 1;
        init.resolution.height = 1;
        init.resolution.reset  = BGFX_RESET_NONE;
        bgfx::init(init);

        uint mv = bgfx::getCaps()->limits.maxViews;

        while (mv != 0) {
            viewStack.push((bgfx::ViewId) mv--);
        }
    };

    ~Context()
    {
#ifdef QT_WIDGETS_LIB
        delete widget;
#endif
        bgfx::shutdown();
    };

public:
    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;
    Context(Context&&) = delete;
    Context& operator=(Context&&) = delete;
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_CONTEXT_H
