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

    m.enablePerVertexColor();

    // Deterministically assign colors to vertices
    for (auto& v : m.vertices()) {
        if (v.index() % 3 == 0)
            v.color() = vcl::Color::Red;
        else if (v.index() % 3 == 1)
            v.color() = vcl::Color::Green;
        else
            v.color() = vcl::Color::Blue;
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
    mv.trackballZoom(-150.0f);

    vcl::Image renderedImage;
    // this auto concludes loop
    mv.screenshot(renderedImage);

    REQUIRE_FALSE(renderedImage.isNull());

    std::string groundTruthFilename = std::string(VCLIB_GROUND_TRUTH_PATH) +
                                      "/005-mesh-wireframe-headless_" +
                                      testName + "_gt.png";

    if (!std::filesystem::exists(groundTruthFilename)) {
        std::string resultFilename = std::string(VCLIB_RENDER_RESULTS_PATH) +
                                     "/005-mesh-wireframe-headless_" +
                                     testName + "_res.png";
        vcl::saveImage(renderedImage, resultFilename);
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
                                 "/005-mesh-wireframe-headless_" + testName +
                                 "_res.png";
    vcl::saveImage(renderedImage, resultFilename);

    const bgfx::Caps& caps = vcl::Context::instance().capabilites();
    bool isWARP            = caps.vendorId == 0x1414 && caps.deviceId == 0x008c;

    if (!isWARP || testName != "selection") {
        REQUIRE(match);
    }
}

TEST_CASE("Wireframe Shading Modes")
{
    SECTION("Shading None")
    {
        runRenderTest("shading_none", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bunny_simplified.obj");

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::VISIBLE, false);
            settings.setWireframe(
                vcl::MeshRenderInfo::Wireframe::VISIBLE, true);
            settings.setWireframe(vcl::MeshRenderInfo::Wireframe::SHADING_NONE);
            settings.setWireframeUserColor(vcl::Color::Green);
            settings.setWireframe(vcl::MeshRenderInfo::Wireframe::COLOR_USER);
            settings.setWireframeWidth(3);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }

    SECTION("Shading Vert")
    {
        runRenderTest("shading_vert", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bunny_simplified.obj");

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::VISIBLE, false);
            settings.setWireframe(
                vcl::MeshRenderInfo::Wireframe::VISIBLE, true);
            settings.setWireframe(vcl::MeshRenderInfo::Wireframe::SHADING_VERT);
            settings.setWireframeUserColor(vcl::Color::Green);
            settings.setWireframe(vcl::MeshRenderInfo::Wireframe::COLOR_USER);
            settings.setWireframeWidth(3);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }
}

TEST_CASE("Wireframe Color Modes")
{
    SECTION("Color User")
    {
        runRenderTest("color_user", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bunny_simplified.obj");

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::VISIBLE, false);
            settings.setWireframe(
                vcl::MeshRenderInfo::Wireframe::VISIBLE, true);
            settings.setWireframeUserColor(vcl::Color::Red);
            settings.setWireframe(vcl::MeshRenderInfo::Wireframe::COLOR_USER);
            settings.setWireframeWidth(3);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }

    SECTION("Color Mesh")
    {
        runRenderTest("color_mesh", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bunny_simplified.obj");
            mesh.color() =
                vcl::Color::Blue; // Set a color different from default/user
            mesh.updateBuffers(
                {vcl::MeshRenderInfo::Buffers::MESH_ADDITIONAL_DATA});

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::VISIBLE, false);
            settings.setWireframe(
                vcl::MeshRenderInfo::Wireframe::VISIBLE, true);
            settings.setWireframe(vcl::MeshRenderInfo::Wireframe::COLOR_MESH);
            settings.setWireframeUserColor(
                vcl::Color::Green); // Different color to ensure it's not used
            settings.setWireframeWidth(3);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }

    SECTION("Color Vertex")
    {
        runRenderTest("color_vertex", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bunny_simplified.obj");

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::VISIBLE, false);
            settings.setWireframe(
                vcl::MeshRenderInfo::Wireframe::VISIBLE, true);
            settings.setWireframe(vcl::MeshRenderInfo::Wireframe::COLOR_VERTEX);
            settings.setWireframeWidth(3);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }
}

TEST_CASE("Wireframe Depth Offset")
{
    SECTION("Depth Offset")
    {
        runRenderTest("depth_offset", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bunny_simplified.obj");

            auto settings = mesh.renderSettings();
            // Surface visible, shading smooth
            settings.setSurface(vcl::MeshRenderInfo::Surface::VISIBLE, true);
            settings.setSurface(vcl::MeshRenderInfo::Surface::COLOR_USER);
            settings.setSurface(vcl::MeshRenderInfo::Surface::SHADING_SMOOTH);

            // Wireframe visible, should render on top because of depth offset
            settings.setWireframe(
                vcl::MeshRenderInfo::Wireframe::VISIBLE, true);
            settings.setWireframeWidth(3);
            settings.setWireframeUserColor(vcl::Color::Red);
            settings.setWireframe(vcl::MeshRenderInfo::Wireframe::COLOR_USER);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }
}
