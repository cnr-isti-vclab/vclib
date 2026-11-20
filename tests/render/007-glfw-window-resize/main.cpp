// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/glfw/application.h>
#include <vclib/glfw/viewer_window.h>

#include <cstdlib>
#include <iostream>

/**
 * @brief Stress test for GLFW window resize handling.
 * Creates two glfw  windows, destroys the first, and repeatedlyresizes the
 * second to test framebuffer handling.
 */
int main(int argc, char** argv)
{
    if (!std::getenv("VCLIB_RUN_GLFW_WINDOW_TESTS")) {
        std::cerr << "Skipped: set VCLIB_RUN_GLFW_WINDOW_TESTS=1\n";
        return 77;
    }

    vcl::glfw::Application app(argc, argv);
    vcl::glfw::ViewerWindow first("First", 400, 300);
    vcl::glfw::ViewerWindow second("Second", 400, 300);
    first.show();
    second.show();
    glfwPollEvents();
    first.paint();
    second.paint();

    first.destroyWindow();
    app.unregisterWindow(&first);
    if (glfwGetWindowAttrib(first.glfwWindow(), GLFW_VISIBLE)) {
        std::cerr << "The closed default window is still visible\n";
        return 1;
    }

    const auto framebuffer = second.frameBuffer();
    for (int i = 0; i < 10; ++i) {
        second.resize(450 + 10 * i, 350 + 10 * i);
        glfwPollEvents();
        second.onGlfwFramebufferSize(900 + 20 * i, 700 + 20 * i);
        second.onGlfwFramebufferSize(910 + 20 * i, 710 + 20 * i);
        if (second.frameBuffer().idx != framebuffer.idx) {
            std::cerr << "Resizing replaced the window framebuffer\n";
            return 1;
        }
        second.paint();
    }

    second.destroyWindow();
    app.unregisterWindow(&second);
    return 0;
}
