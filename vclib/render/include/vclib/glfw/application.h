/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_GLFW_APPLICATION_H
#define VCL_GLFW_APPLICATION_H

#include <set>
#include <memory>
#include <GLFW/glfw3.h>

namespace vcl::glfw {

class WindowManagerBase; // Forward declaration

class GLFWApplication {
    // Centralized static GLFW callbacks
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void contentScaleCallback(GLFWwindow* window, float xscale, float yscale);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    GLFWApplication();
    ~GLFWApplication();

public:
    // Singleton instance accessor (optional, can be regular class)
    static GLFWApplication& instance();

    // Register/unregister windows (called by WindowManager)
    void registerWindow(WindowManagerBase* win);
    void unregisterWindow(WindowManagerBase* win);

    // Main event/render loop
    int exec();

    // Number of open windows
    size_t windowCount() const;

private:
    std::set<WindowManagerBase*> mWindows;
    bool mInitialized = false;
};

// Base class for all window managers to allow registration
class WindowManagerBase {
public:
    virtual ~WindowManagerBase() = default;
    virtual void paint() = 0;
    virtual bool shouldClose() const = 0;
    virtual GLFWwindow* glfwWindow() const = 0;

    // GLFW callback handling methods (to be called by GLFWApplication)
    virtual void onGlfwFramebufferSize(int width, int height) = 0;
    virtual void onGlfwContentScale(float xscale, float yscale) = 0;
    virtual void onGlfwKey(int key, int scancode, int action, int mods) = 0;
    virtual void onGlfwMouseButton(int button, int action, int mods) = 0;
    virtual void onGlfwCursorPos(double xpos, double ypos) = 0;
    virtual void onGlfwScroll(double xoffset, double yoffset) = 0;
};

} // namespace vcl::glfw

#endif // VCL_GLFW_APPLICATION_H
