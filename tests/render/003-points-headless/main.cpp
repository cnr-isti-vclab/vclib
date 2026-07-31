// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/context.h>
#include <vclib/render/drawable/drawable_mesh.h>
#include <vclib/render/headless_mesh_viewer.h>

#include <vclib/algorithms/mesh.h>
#include <vclib/io.h>
#include <vclib/mesh.h>

#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <functional>
#include <string>

// We need a helper to load meshes and create drawable mesh
template<vcl::MeshConcept MeshType>
vcl::DrawableMesh<MeshType> getDrawableMesh(const std::string& filename)
{
    vcl::MeshInfo info;
    MeshType      m = vcl::loadMesh<MeshType>(
        std::string(VCLIB_EXAMPLE_MESHES_PATH) + "/" + filename, info);

    if constexpr (vcl::FaceMeshConcept<MeshType>) {
        if (!info.hasPerFaceNormal()) {
            vcl::updatePerFaceNormals(m);
        }
        if (!info.hasPerVertexNormal()) {
            vcl::updatePerVertexNormalsFromFaceNormals(m);
        }
    }

    return vcl::makeDrawable(std::move(m));
}

void runRenderTest(
    const std::string&                            testName,
    std::function<void(vcl::HeadlessMeshViewer&)> setup)
{
    vcl::HeadlessMeshViewer mv("Headless Mesh Viewer", 1920, 1080);

    // run custom setup
    setup(mv);

    // Apply fitscene to center everything
    mv.fitScene();

    // Zoom in a bit to make the mesh larger
    auto cam  = mv.camera();
    cam.eye() = cam.center() + (cam.eye() - cam.center()) * 0.6f;
    mv.setCamera(cam);

    vcl::Image renderedImage;
    // this auto concludes loop
    mv.screenshot(renderedImage);

    REQUIRE_FALSE(renderedImage.isNull());

    std::string groundTruthFilename = std::string(VCLIB_GROUND_TRUTH_PATH) +
                                      "/003-points-headless_" + testName +
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
                                 "/003-points-headless_" + testName +
                                 "_res.png";
    vcl::saveImage(renderedImage, resultFilename);

    const bgfx::Caps& caps = vcl::Context::instance().capabilites();
    bool isWARP            = caps.vendorId == 0x1414 && caps.deviceId == 0x008c;

    // WARP software rasterizer has a bug with SV_PrimitiveID on indexed
    // meshes without a Geometry Shader. It returns scrambled indices.
    // We check if it matches in the tests that are not affected or if we are
    // not on WARP. For now we assume points are fine on WARP, but we keep the
    // structure just in case.
    if (!isWARP || testName != "selection") {
        REQUIRE(match);
    }
}

TEST_CASE("Points Shading Modes")
{
    SECTION("Shading None")
    {
        runRenderTest("shading_none", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::VISIBLE, false);
            settings.setPoints(vcl::MeshRenderInfo::Points::VISIBLE, true);
            settings.setPoints(vcl::MeshRenderInfo::Points::SHADING_NONE);
            settings.setPointsWidth(4.0f);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }

    SECTION("Shading Vert")
    {
        runRenderTest("shading_vert", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::VISIBLE, false);
            settings.setPoints(vcl::MeshRenderInfo::Points::VISIBLE, true);
            settings.setPoints(vcl::MeshRenderInfo::Points::SHADING_VERT);
            settings.setPointsWidth(4.0f);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }
}

TEST_CASE("Points Color Modes")
{
    SECTION("Color User")
    {
        runRenderTest("color_user", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::VISIBLE, false);
            settings.setPoints(vcl::MeshRenderInfo::Points::VISIBLE, true);
            settings.setPointsUserColor(vcl::Color::Red);
            settings.setPoints(vcl::MeshRenderInfo::Points::COLOR_USER);
            settings.setPointsWidth(4.0f);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }

    SECTION("Color Mesh")
    {
        runRenderTest("color_mesh", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");
            mesh.color() =
                vcl::Color::Blue; // Set a color different from default/user

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::VISIBLE, false);
            settings.setPoints(vcl::MeshRenderInfo::Points::VISIBLE, true);
            settings.setPoints(vcl::MeshRenderInfo::Points::COLOR_MESH);
            settings.setPointsUserColor(
                vcl::Color::Green); // Different color to ensure it's not used
            settings.setPointsWidth(4.0f);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }

    SECTION("Color Vertex")
    {
        runRenderTest("color_vertex", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

            mesh.enablePerVertexColor(); // Enable per vertex color
            mesh.updateRenderSettingsCapabilities();
            // Deterministically assign colors to vertices
            for (auto& v : mesh.vertices()) {
                if (v.index() % 3 == 0)
                    v.color() = vcl::Color::Red;
                else if (v.index() % 3 == 1)
                    v.color() = vcl::Color::Green;
                else
                    v.color() = vcl::Color::Blue;
            }
            mesh.updateBuffers({vcl::MeshRenderInfo::Buffers::VERT_COLORS});

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::VISIBLE, false);
            settings.setPoints(vcl::MeshRenderInfo::Points::VISIBLE, true);
            settings.setPoints(vcl::MeshRenderInfo::Points::COLOR_VERTEX);
            settings.setPointsWidth(4.0f);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }
}

TEST_CASE("Points Shape Modes")
{
    SECTION("Shape Pixel")
    {
        runRenderTest("shape_pixel", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::VISIBLE, false);
            settings.setPoints(vcl::MeshRenderInfo::Points::VISIBLE, true);
            settings.setPoints(vcl::MeshRenderInfo::Points::SHAPE_PIXEL);
            settings.setPointsWidth(4.0f);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }

    SECTION("Shape Circle")
    {
        runRenderTest("shape_circle", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::VISIBLE, false);
            settings.setPoints(vcl::MeshRenderInfo::Points::VISIBLE, true);
            settings.setPoints(vcl::MeshRenderInfo::Points::SHAPE_CIRCLE);
            settings.setPointsWidth(4.0f);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }
}

TEST_CASE("Points Selection")
{
    SECTION("Vertex Selection")
    {
        runRenderTest("selection", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

            mesh.updateRenderSettingsCapabilities();

            // Deterministically assign selection to vertices
            for (auto& v : mesh.vertices()) {
                if (v.index() % 5 == 0)
                    v.selected() = true;
            }
            mesh.updateBuffers();

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::VISIBLE, false);
            settings.setPoints(vcl::MeshRenderInfo::Points::VISIBLE, true);
            settings.setPoints(vcl::MeshRenderInfo::Points::SELECTION);
            settings.setPointsWidth(4.0f);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }
}

TEST_CASE("Points Depth Offset")
{
    SECTION("Depth Offset")
    {
        runRenderTest("depth_offset", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

            auto settings = mesh.renderSettings();
            // Surface visible, shading smooth
            settings.setSurface(vcl::MeshRenderInfo::Surface::VISIBLE, true);
            settings.setSurface(vcl::MeshRenderInfo::Surface::SHADING_SMOOTH);

            // Points visible, should render on top because of depth offset
            settings.setPoints(vcl::MeshRenderInfo::Points::VISIBLE, true);
            settings.setPointsWidth(4.0f);
            settings.setPointsUserColor(vcl::Color::Red);
            settings.setPoints(vcl::MeshRenderInfo::Points::COLOR_USER);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }
}
