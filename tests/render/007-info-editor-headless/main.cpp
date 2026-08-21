// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "get_drawable_mesh.h"
#include "run_render_test.h"

static const std::string TEST_NAME = "007-info-editor-headless";

TEST_CASE("Info Editor Rendering")
{
    SECTION("face_info")
    {
        runRenderTest(
            TEST_NAME,
            "face_info",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh =
                    getDrawableMesh<vcl::TriMesh>("bunny_simplified.obj");
                mv.pushDrawableObject(std::move(mesh));
            },
            0.0f,
            false,
            -150.0f,
            2,
            0.005f,
            [](vcl::HeadlessMeshViewer& mv) {
                mv.pushEditor<vcl::InfoEditorBGFX>(true);
                mv.simulateMousePress(
                    vcl::MouseButton::LEFT,
                    mv.width() / 2.0,
                    mv.height() / 2.0);
            });
    }

    SECTION("poly_info")
    {
        runRenderTest(
            TEST_NAME,
            "poly_info",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh = getDrawableMesh<vcl::PolyMesh>(
                    "spot/spot_quadrangulated.obj");
                mv.pushDrawableObject(std::move(mesh));
            },
            1.57079632679f,
            false,
            -150.0f,
            2,
            0.005f,
            [](vcl::HeadlessMeshViewer& mv) {
                mv.pushEditor<vcl::InfoEditorBGFX>(true);
                mv.simulateMousePress(
                    vcl::MouseButton::LEFT,
                    mv.width() / 2.0,
                    mv.height() / 2.0);
            });
    }
}
