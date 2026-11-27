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

#ifdef VCLIB_RENDER_BACKEND_BGFX
#include <vclib/bgfx/context.h>

#include <QPointer>
#include <QWindow>

#include <memory>
#include <stdexcept>
#endif

namespace vcl::qt::detail {

class WindowManagerNative
{
public:
#ifdef VCLIB_RENDER_BACKEND_BGFX
    /**
     * Initializes bgfx with an application-owned hidden window as its default
     * surface, so it remains valid after individual Qt windows are destroyed.
     * Visible Qt windows use separate swap chains, which the renderer must
     * support. An existing headless context is accepted, but a context using
     * an unrelated default window is rejected.
     */
    static void initBgfxContext()
    {
        // Reuse the application-owned window; QPointer clears when Qt deletes it.
        static QPointer<QWindow> defaultWindow;
        const bool initializing = !vcl::Context::isInitialized();
        if (initializing) {
            if (!defaultWindow) {
                auto window = std::make_unique<QWindow>();
                window->resize(1, 1);
                window->create();
                if (!window->winId())
                    throw std::runtime_error(
                        "Failed to create bgfx default Qt window");

                // The default swap chain must outlive every widget and bgfx.
                window->QObject::setParent(QCoreApplication::instance());
                defaultWindow = window.release();
            }
            vcl::Context::init(
                reinterpret_cast<void*>(defaultWindow->winId()),
                displayId(),
                handleType());
        }
        else if (
            !vcl::Context::instance().isHeadless() &&
            (!defaultWindow || !vcl::Context::instance().isDefaultWindow(
                                   reinterpret_cast<void*>(
                                       defaultWindow->winId())))) {
            throw std::runtime_error(
                "Qt windows require a bgfx context initialized by Qt or "
                "a headless bgfx context");
        }

        if (!(bgfx::getCaps()->supported & BGFX_CAPS_SWAP_CHAIN)) {
            if (initializing)
                vcl::Context::shutdown();
            throw std::runtime_error(
                "The bgfx renderer does not support Qt window swap chains");
        }
    }
#endif

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
