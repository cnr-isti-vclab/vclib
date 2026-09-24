// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_GLFW_APPLICATION_H
#define VCL_GLFW_APPLICATION_H

#ifdef VCLIB_RENDER_BACKEND_BGFX
#include <vclib/bgfx/context.h>
#endif

#include <GLFW/glfw3.h>
#include <set>
#include <memory>

namespace vcl::glfw {

class WindowManagerBase {
public:
    virtual ~WindowManagerBase() = default;
    virtual void paint() = 0;
    virtual bool shouldClose() const = 0;
    virtual GLFWwindow* glfwWindow() const = 0;

    // GLFW callback handling methods (to be called by Application)
    virtual void onGlfwFramebufferSize(int width, int height) = 0;
    virtual void onGlfwContentScale(float xscale, float yscale) = 0;
    virtual void onGlfwKey(int key, int scancode, int action, int mods) = 0;
    virtual void onGlfwMouseButton(int button, int action, int mods) = 0;
    virtual void onGlfwCursorPos(double xpos, double ypos) = 0;
    virtual void onGlfwScroll(double xoffset, double yoffset) = 0;
};

/**
 * @brief Application class for GLFW backend.
 */
class Application
{
public:
    Application(int& argc, char** argv);
    ~Application();

    int exec();

    static Application* instance();
    void registerWindow(WindowManagerBase* win);
    void unregisterWindow(WindowManagerBase* win);

private:
    static Application* s_instance;
    std::set<WindowManagerBase*> mWindows;

    // Centralized static GLFW callbacks
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void contentScaleCallback(GLFWwindow* window, float xscale, float yscale);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

} // namespace vcl::glfw

#endif // VCL_GLFW_APPLICATION_H
