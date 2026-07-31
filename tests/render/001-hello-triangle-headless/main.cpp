// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../../../examples/render/00-hello-triangle-bgfx-common/hello_triangle_drawer.h"
#include <catch2/catch_test_macros.hpp>
#include <vclib/io/image/load.h>
#include <vclib/io/image/save.h>
#include <vclib/render/canvas.h>
#include <vclib/render/headless_manager.h>
#include <vclib/render/render_app.h>

#include <filesystem>

// We inherit WindowHeadless just to make the test simpler and have access to
// app configuration if necessary, though it works perfectly with the default
using WindowHeadless =
    vcl::RenderApp<vcl::HeadlessManager, vcl::Canvas, HelloTriangleDrawer>;

TEST_CASE("Headless Triangle Rendering")
{
    WindowHeadless app("Hello Triangle Headless Test");

    // Capture the screenshot
    vcl::Image renderedImage;
    app.screenshot(renderedImage);

    // Run the app loop until readback is completed
    app.show();

    REQUIRE_FALSE(renderedImage.isNull());

    std::string groundTruthFilename = std::string(VCLIB_GROUND_TRUTH_PATH) +
                                      "/001-hello-triangle-headless_gt.png";

    if (!std::filesystem::exists(groundTruthFilename)) {
        // First run: save the rendered image as ground truth and fail the test
        // so the user can verify it.
        vcl::saveImage(renderedImage, groundTruthFilename);
        FAIL(
            "Ground truth image not found. Saved the current render to: " +
            groundTruthFilename +
            "\nPlease verify the image visually. If it is correct, commit it "
            "and re-run the test.");
    }

    // Load ground truth
    vcl::Image groundTruthImage = vcl::loadImage(groundTruthFilename);
    REQUIRE_FALSE(groundTruthImage.isNull());

    REQUIRE(renderedImage.width() == groundTruthImage.width());
    REQUIRE(renderedImage.height() == groundTruthImage.height());
    REQUIRE(renderedImage.colorSpace() == groundTruthImage.colorSpace());
    REQUIRE(renderedImage.sizeInBytes() == groundTruthImage.sizeInBytes());

    // Compare using isAlmostEqual with tolerance:
    // - max 2 units of difference per color channel
    // - max 0.5% of pixels failing this tolerance
    bool match = renderedImage.isAlmostEqual(groundTruthImage, 2, 0.005f);

    // Always save the result image for inspection
    std::string resultFilename = std::string(VCLIB_RENDER_RESULTS_PATH) +
                                 "/001-hello-triangle-headless_res.png";
    vcl::saveImage(renderedImage, resultFilename);

    REQUIRE(match);
}
