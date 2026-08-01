// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_DETAIL_WINDOW_MANAGER_NATIVE_H
#define VCL_QT_DETAIL_WINDOW_MANAGER_NATIVE_H

#include <vclib/render/window_managers.h>

#include <QGuiApplication>

namespace vcl::qt::detail {

class WindowManagerNative
{
public:
    static void* displayId()
    {
        void* displayID = nullptr;
#ifdef Q_OS_LINUX
        QNativeInterface::QX11Application* x11AppInfo =
            qApp->nativeInterface<QNativeInterface::QX11Application>();
        if (x11AppInfo) {
            displayID = x11AppInfo->display();
        }
        else {
            QNativeInterface::QWaylandApplication* wayAppInfo =
                qApp->nativeInterface<QNativeInterface::QWaylandApplication>();
            if (wayAppInfo) {
                displayID = wayAppInfo->display();
            }
            else {
                exit(-1);
            }
        }
#endif
        return displayID;
    }

    static vcl::NativeWindowHandleType handleType()
    {
#ifdef Q_OS_LINUX
        QNativeInterface::QX11Application* x11AppInfo =
            qApp->nativeInterface<QNativeInterface::QX11Application>();
        if (x11AppInfo) {
            return vcl::NativeWindowHandleType::DEFAULT;
        }

        QNativeInterface::QWaylandApplication* wayAppInfo =
            qApp->nativeInterface<QNativeInterface::QWaylandApplication>();
        if (wayAppInfo) {
            return vcl::NativeWindowHandleType::WAYLAND;
        }
#endif
        return vcl::NativeWindowHandleType::DEFAULT;
    }
};

} // namespace vcl::qt::detail

#endif // VCL_QT_DETAIL_WINDOW_MANAGER_NATIVE_H
