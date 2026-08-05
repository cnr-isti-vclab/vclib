// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "get_drawable_mesh.h"

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/editors/info_editor_bgfx.h>
#include <vclib/render/headless_mesh_viewer.h>

#include <vclib/io.h>
#include <vclib/meshes.h>

#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <functional>
#include <string>

void runRenderTest(
    const std::string&                            testName,
    std::function<void(vcl::HeadlessMeshViewer&)> setup,
    float                                         angleY = 0.0f)
{
    vcl::HeadlessMeshViewer mv("Headless Info Editor Test", 1920, 1080);

    // run custom setup
    setup(mv);

    // Apply fitscene to center everything
    mv.fitScene();

    // zoom in slightly to make sure the mesh is big
    mv.trackballZoom(-150.f);

    if (angleY != 0.0f) {
        mv.trackballRotate(vcl::Point3f(0.0f, 1.0f, 0.0f), angleY);
    }

    // push and activate the InfoEditor
    auto infoEditor = mv.pushEditor<vcl::InfoEditorBGFX>(true);

    // simulate a click at the center of the screen
    double cx = 1920 / 2.0;
    double cy = 1080 / 2.0;
    mv.simulateMousePress(vcl::MouseButton::LEFT, cx, cy);

    // Wait for the simulated mouse press (Target::ID) to be fully processed
    mv.show();

    vcl::Image renderedImage;
    // this auto concludes loop
    mv.screenshot(renderedImage);

    REQUIRE_FALSE(renderedImage.isNull());

    std::string groundTruthFilename = std::string(VCLIB_GROUND_TRUTH_PATH) +
                                      "/007-info-editor-headless_" + testName +
                                      "_gt.png";

    if (!std::filesystem::exists(groundTruthFilename)) {
        FAIL("Ground truth image not found.");
    }

    vcl::Image groundTruthImage = vcl::loadImage(groundTruthFilename);
    REQUIRE_FALSE(groundTruthImage.isNull());

    REQUIRE(renderedImage.width() == groundTruthImage.width());
    REQUIRE(renderedImage.height() == groundTruthImage.height());
    REQUIRE(renderedImage.colorSpace() == groundTruthImage.colorSpace());
    REQUIRE(renderedImage.sizeInBytes() == groundTruthImage.sizeInBytes());

    bool match = renderedImage.isAlmostEqual(groundTruthImage, 2, 0.005f);

    std::string resultFilename = std::string(VCLIB_RENDER_RESULTS_PATH) +
                                 "/007-info-editor-headless_" + testName +
                                 "_res.png";
    vcl::saveImage(renderedImage, resultFilename);

    //const bgfx::Caps& caps = vcl::Context::instance().capabilites();
    //bool isWARP            = caps.vendorId == 0x1414 && caps.deviceId == 0x008c;

    //if (!isWARP) {
        REQUIRE(match);
    //}
}

TEST_CASE("Info Editor Rendering")
{
    SECTION("face_info")
    {
        runRenderTest(
            "face_info", [](vcl::HeadlessMeshViewer& mv) {
                auto mesh = getDrawableMesh<vcl::TriMesh>("bunny_simplified.obj");
                mv.pushDrawableObject(std::move(mesh));
            });
    }
}
