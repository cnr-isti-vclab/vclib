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

#include <vclib/ext/glfw/bgfx/minimal_viewer_window.h>

#include <vclib/ext/glfw/gui/input.h>

namespace vcl::bglfwx {

MinimalViewerWindow::MinimalViewerWindow(
    std::shared_ptr<DrawableObjectVector> v,
    const std::string&                    windowTitle,
    uint                                  width,
    uint                                  height) :
        CanvasWindow(width, height),
        MinimalViewer(v, width, height)
{
    setCallbacks();
}

MinimalViewerWindow::MinimalViewerWindow(
    std::string& windowTitle,
    uint         width,
    uint         height) :
        MinimalViewerWindow(
            std::make_shared<DrawableObjectVector>(),
            windowTitle,
            width,
            height)
{
}

MinimalViewerWindow::MinimalViewerWindow(uint width, uint height) :
        MinimalViewerWindow(
            std::make_shared<DrawableObjectVector>(),
            "Minimal Viewer",
            width,
            height)
{
}

void MinimalViewerWindow::draw()
{
    MV::draw(viewId());
}

void MinimalViewerWindow::onResize(unsigned int width, unsigned int height)
{
    MV::resizeViewer(width, height);
}

void MinimalViewerWindow::onKeyPress(Key::Enum key, KeyModifiers modifiers)
{
    MV::setKeyModifiers(modifiers);

    switch (key) {
    case Key::C:
        std::cerr << "(" << MV::camera().eye() << ") "
                  << "(" << MV::camera().center() << ") "
                  << "(" << MV::camera().up() << ")\n";
        break;

    default:
        MV::keyPress(key);
        bgfx::setViewTransform(
            viewId(), MV::viewMatrix().data(), MV::projectionMatrix().data());
        break;
    }
}

void MinimalViewerWindow::onKeyRelease(Key::Enum key, KeyModifiers modifiers)
{
    MV::setKeyModifiers(modifiers);
}

void MinimalViewerWindow::onMouseMove(double x, double y)
{
    MV::moveMouse(x, y);

    bgfx::setViewTransform(
        viewId(), MV::viewMatrix().data(), MV::projectionMatrix().data());
}

void MinimalViewerWindow::onMousePress(MouseButton::Enum button)
{
    MV::pressMouse(button);
}

void MinimalViewerWindow::onMouseRelease(MouseButton::Enum button)
{
    MV::releaseMouse(button);
}

void MinimalViewerWindow::onMouseScroll(double dx, double dy)
{
    const int WHEEL_STEP = 120;
    float     notchY     = dy / float(WHEEL_STEP);

    MV::wheelMouse(notchY > 0);

    bgfx::setViewTransform(
        viewId(), MV::viewMatrix().data(), MV::projectionMatrix().data());
}

void MinimalViewerWindow::setCallbacks()
{
    // key callback lambda
    auto keyCB =
        [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            auto* self = static_cast<MinimalViewerWindow*>(
                glfwGetWindowUserPointer(window));
            self->glfwKeyCallback(window, key, scancode, action, mods);
        };

    glfwSetKeyCallback(window, keyCB);

    // mouse position callback
    glfwSetCursorPosCallback(
        window, [](GLFWwindow* window, double xpos, double ypos) {
            auto* self = static_cast<MinimalViewerWindow*>(
                glfwGetWindowUserPointer(window));
            self->glfwCursorPosCallback(window, xpos, ypos);
        });

    // mouse button callback
    glfwSetMouseButtonCallback(
        window, [](GLFWwindow* window, int button, int action, int mods) {
            auto* self = static_cast<MinimalViewerWindow*>(
                glfwGetWindowUserPointer(window));
            self->glfwMouseButtonCallback(window, button, action, mods);
        });

    // scroll callback
    glfwSetScrollCallback(
        window, [](GLFWwindow* window, double xoffset, double yoffset) {
            auto* self = static_cast<MinimalViewerWindow*>(
                glfwGetWindowUserPointer(window));
            self->glfwScrollCallback(window, xoffset, yoffset);
        });
}

void MinimalViewerWindow::glfwKeyCallback(
    GLFWwindow*,
    int key,
    int,
    int action,
    int mods)
{
    KeyModifiers modifiers = glfw::fromGLFW((glfw::KeyboardModifiers) mods);
    MV::setKeyModifiers(modifiers);

    if (action == GLFW_PRESS) {
        MV::keyPress(glfw::fromGLFW((glfw::Key) key));
    }
}

void MinimalViewerWindow::glfwMouseButtonCallback(
    GLFWwindow*,
    int button,
    int action,
    int mods)
{
    glfw::MouseButton btn = (glfw::MouseButton) button;

    KeyModifiers modifiers = glfw::fromGLFW((glfw::KeyboardModifiers) mods);
    MV::setKeyModifiers(modifiers);

    if (action == GLFW_PRESS) {
        MV::pressMouse(glfw::fromGLFW(btn));
    }
    if (action == GLFW_RELEASE) {
        MV::releaseMouse(glfw::fromGLFW(btn));
    }
}

void MinimalViewerWindow::glfwCursorPosCallback(
    GLFWwindow*,
    double xpos,
    double ypos)
{
    MV::moveMouse(xpos, ypos);
}

void MinimalViewerWindow::glfwScrollCallback(
    GLFWwindow*,
    double,
    double yoffset)
{
    const int WHEEL_STEP = 120;

    float notchY = yoffset / float(WHEEL_STEP);

    MV::wheelMouse(notchY > 0);
}

} // namespace vcl::bglfwx
