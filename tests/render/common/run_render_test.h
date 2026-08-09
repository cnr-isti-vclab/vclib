// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCLIB_RENDER_TESTS_COMMON_RUN_RENDER_TEST_H
#define VCLIB_RENDER_TESTS_COMMON_RUN_RENDER_TEST_H

#include <vclib/bgfx/context.h>
#include <vclib/render/drawable/drawable_mesh.h>
#include <vclib/render/headless_mesh_viewer.h>

#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <functional>
#include <string>

void runRenderTest(
    const std::string&                            testName,
    const std::string&                            innerTestName,
    std::function<void(vcl::HeadlessMeshViewer&)> setup,
    float                                         angleY         = 0.0f,
    bool                                          warpIgnore     = false,
    float                                         zoom           = 0.0f,
    uint32_t                                      maxDiff        = 2,
    float                                         failPercentage = 0.005f,
    std::function<void(vcl::HeadlessMeshViewer&)> postSetup      = nullptr)
{
    vcl::HeadlessMeshViewer mv("Headless Mesh Viewer", 1920, 1080);

    // run custom setup
    setup(mv);

    // Apply fitscene to center everything
    mv.fitScene();

    if (zoom != 0.0f) {
        mv.trackballZoom(zoom);
    }

    if (angleY != 0.0f) {
        mv.trackballRotate(vcl::Point3f(0.0f, 1.0f, 0.0f), angleY);
    }

    if (postSetup) {
        postSetup(mv);
    }

    vcl::Image renderedImage;
    // this auto concludes loop
    mv.screenshot(renderedImage);

    REQUIRE_FALSE(renderedImage.isNull());

    std::string groundTruthFilename = std::string(VCLIB_GROUND_TRUTH_PATH) +
                                      "/" + testName + "_" + innerTestName +
                                      "_gt.png";
    std::string resultFilename = std::string(VCLIB_RENDER_RESULTS_PATH) + "/" +
                                 testName + "_" + innerTestName + "_res.png";

    if (!std::filesystem::exists(groundTruthFilename)) {
        vcl::saveImage(renderedImage, resultFilename);
        FAIL("Ground truth image not found.");
    }

    vcl::Image groundTruthImage = vcl::loadImage(groundTruthFilename);
    REQUIRE_FALSE(groundTruthImage.isNull());

    REQUIRE(renderedImage.width() == groundTruthImage.width());
    REQUIRE(renderedImage.height() == groundTruthImage.height());
    REQUIRE(renderedImage.colorSpace() == groundTruthImage.colorSpace());
    REQUIRE(renderedImage.sizeInBytes() == groundTruthImage.sizeInBytes());

    bool match =
        renderedImage.isAlmostEqual(groundTruthImage, maxDiff, failPercentage);

    vcl::saveImage(renderedImage, resultFilename);

    const bgfx::Caps& caps = vcl::Context::instance().capabilites();
    bool isWARP            = caps.vendorId == 0x1414 && caps.deviceId == 0x008c;

    // WARP software rasterizer has a bug with SV_PrimitiveID on indexed
    // meshes without a Geometry Shader. It returns scrambled indices.
    if (!isWARP || !warpIgnore) {
        REQUIRE(match);
    }
}

#endif // VCLIB_RENDER_TESTS_COMMON_RUN_RENDER_TEST_H
