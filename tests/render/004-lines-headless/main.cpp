// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/context.h>
#include <vclib/render/drawable/drawable_mesh.h>
#include <vclib/render/headless_mesh_viewer.h>

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <functional>
#include <string>

// We need a helper to create the edge mesh and wrap it in a drawable mesh
vcl::DrawableMesh<vcl::EdgeMesh> getDrawableEdgeMesh()
{
    // 1. Create Icosahedron
    vcl::TriMesh icosahedron = vcl::createIcosahedron<vcl::TriMesh>();

    // 2. Compute normals on faces and vertices
    vcl::updatePerFaceNormals(icosahedron);
    vcl::updatePerVertexNormalsFromFaceNormals(icosahedron);

    // 3. Compute adjacency
    icosahedron.enablePerFaceAdjacentFaces();
    vcl::updatePerFaceAdjacentFaces(icosahedron);

    // 4. Select crease edges (all edges of icosahedron since it's angular)
    vcl::selectCreaseFaceEdges(icosahedron, -vcl::toRad(1.f), vcl::toRad(1.f));

    // 5. Extract edge mesh
    vcl::EdgeMesh em =
        vcl::perFaceEdgeSelectionMeshFilter<vcl::EdgeMesh>(icosahedron);

    // Ensure the edge mesh has the required components
    em.enablePerVertexColor();
    em.enablePerEdgeColor();

    // Deterministically assign colors
    for (auto& v : em.vertices()) {
        if (v.index() % 3 == 0)
            v.color() = vcl::Color::Red;
        else if (v.index() % 3 == 1)
            v.color() = vcl::Color::Green;
        else
            v.color() = vcl::Color::Blue;
    }

    for (auto& e : em.edges()) {
        if (e.index() % 3 == 0)
            e.color() = vcl::Color::Yellow;
        else if (e.index() % 3 == 1)
            e.color() = vcl::Color::Cyan;
        else
            e.color() = vcl::Color::Magenta;
    }

    // Set mesh color
    em.color() = vcl::Color::Magenta;

    return vcl::makeDrawable(std::move(em));
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
                                      "/004-lines-headless_" + testName +
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
                                 "/004-lines-headless_" + testName + "_res.png";
    vcl::saveImage(renderedImage, resultFilename);

    const bgfx::Caps& caps = vcl::Context::instance().capabilites();
    bool isWARP            = caps.vendorId == 0x1414 && caps.deviceId == 0x008c;

    if (!isWARP || testName != "color_edge") {
        REQUIRE(match);
    }
}

TEST_CASE("Lines Shading Modes")
{
    SECTION("Shading None")
    {
        runRenderTest("shading_none", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableEdgeMesh();

            auto settings = mesh.renderSettings();
            settings.setEdges(vcl::MeshRenderInfo::Edges::VISIBLE, true);
            settings.setEdges(vcl::MeshRenderInfo::Edges::SHADING_NONE);
            settings.setEdgesWidth(4);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }

    SECTION("Shading Smooth")
    {
        runRenderTest("shading_smooth", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableEdgeMesh();

            auto settings = mesh.renderSettings();
            settings.setEdges(vcl::MeshRenderInfo::Edges::VISIBLE, true);
            settings.setEdges(vcl::MeshRenderInfo::Edges::SHADING_SMOOTH);
            settings.setEdgesWidth(4);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }
}

TEST_CASE("Lines Color Modes")
{
    SECTION("Color User")
    {
        runRenderTest("color_user", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableEdgeMesh();

            auto settings = mesh.renderSettings();
            settings.setEdges(vcl::MeshRenderInfo::Edges::VISIBLE, true);
            settings.setEdgesUserColor(vcl::Color::DarkYellow);
            settings.setEdges(vcl::MeshRenderInfo::Edges::COLOR_USER);
            settings.setEdgesWidth(4);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }

    SECTION("Color Mesh")
    {
        runRenderTest("color_mesh", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableEdgeMesh();

            auto settings = mesh.renderSettings();
            settings.setEdges(vcl::MeshRenderInfo::Edges::VISIBLE, true);
            settings.setEdges(vcl::MeshRenderInfo::Edges::COLOR_MESH);
            settings.setEdgesUserColor(
                vcl::Color::Green); // Different color to ensure it's not used
            settings.setEdgesWidth(4);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }

    SECTION("Color Vertex")
    {
        runRenderTest("color_vertex", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableEdgeMesh();

            auto settings = mesh.renderSettings();
            settings.setEdges(vcl::MeshRenderInfo::Edges::VISIBLE, true);
            settings.setEdges(vcl::MeshRenderInfo::Edges::COLOR_VERTEX);
            settings.setEdgesWidth(4);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }

    SECTION("Color Edge")
    {
        runRenderTest("color_edge", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableEdgeMesh();

            auto settings = mesh.renderSettings();
            settings.setEdges(vcl::MeshRenderInfo::Edges::VISIBLE, true);
            settings.setEdges(vcl::MeshRenderInfo::Edges::COLOR_EDGE);
            settings.setEdgesWidth(4);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }
}
