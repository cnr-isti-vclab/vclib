// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/application.h>
#include <vclib/qt/widget_manager.h>
#include <vclib/qt/window_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/drawers/plain_drawer.h>
#include <vclib/render/render_app.h>

#ifdef VCLIB_WITH_IMGUI
#include <vclib/imgui/imgui_drawer.h>
#endif

#include <QCoreApplication>
#include <QThread>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

template<typename App>
class PaintCounter : public vcl::PlainDrawer<App>
{
public:
    using vcl::PlainDrawer<App>::PlainDrawer;
    unsigned paints = 0;

    void onDraw(uint) override { ++paints; }
};

void repaint(QWidget& widget)
{
    widget.repaint();
}

void repaint(QWindow& window)
{
    QEvent event(QEvent::UpdateRequest);
    QCoreApplication::sendEvent(&window, &event);
}

template<typename Window>
bool waitForPaint(Window& window)
{
    const unsigned before = window.paints;
    for (int i = 0; i < 100; ++i) {
        QCoreApplication::processEvents();
        repaint(window);
        if (window.paints > before)
            return true;
        QThread::msleep(10);
    }
    return false;
}

template<template<typename> typename Manager>
bool testWindowOrder(bool firstDestroyedFirst, bool closeBeforeDestroy)
{
    using Window = vcl::RenderApp<Manager, vcl::Canvas, PaintCounter>;
    auto first   = std::make_unique<Window>("First", 400, 300);
    auto second  = std::make_unique<Window>("Second", 400, 300);
    first->show();
    second->show();
    QCoreApplication::processEvents();
    repaint(*first);
    repaint(*second);
    if (!bgfx::isValid(first->frameBuffer()) ||
        !bgfx::isValid(second->frameBuffer())) {
        std::cerr << "A window does not have its own swap chain\n";
        return false;
    }

    auto& closed   = firstDestroyedFirst ? first : second;
    auto& survivor = firstDestroyedFirst ? second : first;
    if (closeBeforeDestroy)
        closed->close();
    closed.reset();
    survivor->close();
    QCoreApplication::processEvents();
    survivor->show();
    if (!waitForPaint(*survivor)) {
        std::cerr << "The surviving window was not painted\n";
        return false;
    }
    survivor.reset();
    return true;
}

bool testForeignDefaultWindow()
{
    QWindow window;
    window.create();
    vcl::Context::init(
        reinterpret_cast<void*>(window.winId()),
        vcl::qt::detail::WindowManagerNative::displayId(),
        vcl::qt::detail::WindowManagerNative::handleType());
    bool rejected = false;
    try {
        vcl::RenderApp<vcl::qt::WidgetManager, vcl::Canvas> other;
    }
    catch (const std::runtime_error& error) {
        rejected =
            std::string(error.what())
                .find("Qt windows require a bgfx context") != std::string::npos;
    }
    vcl::Context::shutdown();
    if (!rejected)
        std::cerr << "A foreign default window was not rejected\n";
    return rejected;
}

bool testParentDestruction()
{
    using Window =
        vcl::RenderApp<vcl::qt::WidgetManager, vcl::Canvas, PaintCounter>;
    auto  survivor = std::make_unique<Window>("Survivor", 400, 300);
    auto  parent   = std::make_unique<QWidget>();
    auto* child    = new Window("Child", 400, 300, parent.get());
    survivor->show();
    parent->show();
    child->show();
    QCoreApplication::processEvents();
    repaint(*survivor);
    repaint(*child);
    parent->hide();
    parent->show();
    QCoreApplication::processEvents();
    repaint(*child);
    parent.reset();

    if (!waitForPaint(*survivor)) {
        std::cerr << "The survivor was not painted after parent destruction\n";
        return false;
    }
    return true;
}

bool testDeleteOnClose()
{
    using Window =
        vcl::RenderApp<vcl::qt::WidgetManager, vcl::Canvas, PaintCounter>;
    Window           survivor("Survivor", 400, 300);
    QPointer<Window> closed = new Window("Delete on close", 400, 300);
    closed->setAttribute(Qt::WA_DeleteOnClose);
    survivor.show();
    closed->show();
    QCoreApplication::processEvents();
    repaint(survivor);
    repaint(*closed);
    closed->close();
    QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
    if (closed) {
        std::cerr << "The closed window was not deleted\n";
        return false;
    }

    if (!waitForPaint(survivor)) {
        std::cerr << "The survivor was not painted after window deletion\n";
        return false;
    }
    return true;
}

#ifdef VCLIB_WITH_IMGUI
void testImGuiWindow()
{
    using Window = vcl::
        RenderApp<vcl::qt::WidgetManager, vcl::Canvas, vcl::imgui::ImGuiDrawer>;
    Window window("ImGui", 400, 300);
    ImGui::GetIO().IniFilename = nullptr;
    window.show();
    QCoreApplication::processEvents();
    repaint(window);
    window.close();
    window.show();
    QCoreApplication::processEvents();
    repaint(window);
}
#endif

int main(int argc, char** argv)
{
    if (!std::getenv("VCLIB_RUN_QT_WINDOW_TESTS")) {
        std::cerr << "Skipped: set VCLIB_RUN_QT_WINDOW_TESTS=1\n";
        return 77;
    }

    vcl::qt::Application app(argc, argv);
    const bool           foreign = testForeignDefaultWindow();
    const bool           widgets =
        testWindowOrder<vcl::qt::WidgetManager>(true, true) &&
        testWindowOrder<vcl::qt::WidgetManager>(true, false) &&
        testWindowOrder<vcl::qt::WidgetManager>(false, true) &&
        testWindowOrder<vcl::qt::WidgetManager>(false, false) &&
        testParentDestruction() && testDeleteOnClose();
#ifdef VCLIB_WITH_IMGUI
    testImGuiWindow();
#endif
    vcl::Context::shutdown();
    const bool windows = testWindowOrder<vcl::qt::WindowManager>(true, true) &&
                         testWindowOrder<vcl::qt::WindowManager>(true, false) &&
                         testWindowOrder<vcl::qt::WindowManager>(false, true) &&
                         testWindowOrder<vcl::qt::WindowManager>(false, false);
    return foreign && widgets && windows ? 0 : 1;
}
