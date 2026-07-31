// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../00-hello-triangle-bgfx-common/hello_triangle_drawer.h"

#include <vclib/render/canvas.h>
#include <vclib/render/headless_manager.h>
#include <vclib/render/render_app.h>

#include <vclib/io.h>

#include <iostream>

int main(int argc, char** argv)
{
    using WindowHeadless =
        vcl::RenderApp<vcl::HeadlessManager, vcl::Canvas, HelloTriangleDrawer>;

    WindowHeadless app("Hello Triangle Headless");

    // Request a screenshot
    std::string screenshotFilename = "screenshot.png";
    std::cout << "Requesting screenshot: " << screenshotFilename << std::endl;
    app.screenshot(screenshotFilename);

    // Run the headless app
    // It will run until the screenshot readback is complete
    std::cout << "Starting rendering loop..." << std::endl;
    app.show();
    std::cout << "Rendering loop finished. Check " << screenshotFilename
              << " for the output." << std::endl;

    // request screenshot and save it to vcl::Image
    vcl::Image screenshotImage;
    std::cout << "Requesting screenshot in memory..." << std::endl;
    app.screenshot(screenshotImage);
    app.show();

    std::cout << "Screenshot in memory requested. Image size: "
              << screenshotImage.width() << "x" << screenshotImage.height()
              << std::endl;

    vcl::saveImage(screenshotImage, "screenshot_in_memory.png");

    std::cout << "Screenshot saved to screenshot_in_memory.png" << std::endl;

    return 0;
}
