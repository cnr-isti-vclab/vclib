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
#include <vclib/meshes.h>

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
    std::function<void(vcl::HeadlessMeshViewer&)> setup,
    float                                         angleY = 0.0f)
{
    vcl::HeadlessMeshViewer mv("Headless Mesh Viewer", 1920, 1080);

    // run custom setup
    setup(mv);

    // Apply fitscene to center everything
    mv.fitScene();

    if (angleY != 0.0f) {
        mv.trackballRotate(vcl::Point3f(0.0f, 1.0f, 0.0f), angleY);
    }

    vcl::Image renderedImage;
    // this auto concludes loop
    mv.screenshot(renderedImage);

    REQUIRE_FALSE(renderedImage.isNull());

    std::string groundTruthFilename = std::string(VCLIB_GROUND_TRUTH_PATH) +
                                      "/002-mesh-surface-headless_" + testName +
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
                                 "/002-mesh-surface-headless_" + testName +
                                 "_res.png";
    vcl::saveImage(renderedImage, resultFilename);

    const bgfx::Caps& caps = vcl::Context::instance().capabilites();
    bool isWARP            = caps.vendorId == 0x1414 && caps.deviceId == 0x008c;

    // WARP software rasterizer has a bug with SV_PrimitiveID on indexed
    // meshes without a Geometry Shader. It returns scrambled indices.
    if (!isWARP ||
        (testName != "color_face" && testName != "shading_flat" &&
         testName != "surface_selection" && testName != "polygon_selection")) {
        REQUIRE(match);
    }
}

TEST_CASE("Mesh Surface Shading Modes")
{
    SECTION("Shading None")
    {
        runRenderTest("shading_none", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::SHADING_NONE);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }

    SECTION("Shading Flat")
    {
        runRenderTest("shading_flat", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::SHADING_FLAT);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }

    SECTION("Shading Smooth")
    {
        runRenderTest("shading_smooth", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::SHADING_SMOOTH);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }

    SECTION("Shading Normal Map")
    {
        runRenderTest("shading_normal_map", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>(
                "gltf/NormalTangentMirrorTest/NormalTangentMirrorTest.gltf");

            auto settings = mesh.renderSettings();
            settings.setSurface(
                vcl::MeshRenderInfo::Surface::SHADING_NORMAL_MAP);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }
}

TEST_CASE("Mesh Surface Color Modes")
{
    SECTION("Color User")
    {
        runRenderTest("color_user", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

            auto settings = mesh.renderSettings();
            settings.setSurfaceUserColor(vcl::Color::Red);
            settings.setSurface(vcl::MeshRenderInfo::Surface::COLOR_USER);
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
            settings.setSurface(vcl::MeshRenderInfo::Surface::COLOR_MESH);
            settings.setSurfaceUserColor(
                vcl::Color::Green); // Different color to ensure it's not used
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
            settings.setSurface(vcl::MeshRenderInfo::Surface::COLOR_VERTEX);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }

    SECTION("Color Face")
    {
        runRenderTest("color_face", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bunny_simplified.obj");

            mesh.enablePerFaceColor();
            mesh.updateRenderSettingsCapabilities();

            // Deterministically assign colors to faces
            for (auto& f : mesh.faces()) {
                if (f.index() % 3 == 0)
                    f.color() = vcl::Color::Cyan;
                else if (f.index() % 3 == 1)
                    f.color() = vcl::Color::Magenta;
                else
                    f.color() = vcl::Color::Yellow;
            }
            mesh.updateBuffers({vcl::MeshRenderInfo::Buffers::TRI_COLORS});

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::COLOR_FACE);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }

    SECTION("Color Vertex Texcoord")
    {
        runRenderTest("color_vertex_texcoord", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>(
                "gltf/DamagedHelmet/DamagedHelmet.gltf");

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::COLOR_VERTEX_TEX);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }

    SECTION("Color Wedge Texcoord")
    {
        runRenderTest("color_wedge_texcoord", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh =
                getDrawableMesh<vcl::TriMesh>("spot/spot_triangulated.obj");

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::COLOR_WEDGE_TEX);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }
}

TEST_CASE("Mesh Surface Selection")
{
    SECTION("Face Selection")
    {
        runRenderTest("surface_selection", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

            mesh.updateRenderSettingsCapabilities();

            // Deterministically assign selection to faces
            for (auto& f : mesh.faces()) {
                if (f.index() % 5 == 0)
                    f.selected() = true;
            }
            mesh.updateBuffers();

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::SELECTION);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        });
    }

    SECTION("Polygon Selection")
    {
        runRenderTest("polygon_selection", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::PolyMesh>("spot/spot_quadrangulated.obj");

            mesh.updateRenderSettingsCapabilities();

            // Deterministically assign selection to faces
            for (auto& f : mesh.faces()) {
                if (f.index() % 3 == 0)
                    f.selected() = true;
            }
            mesh.updateBuffers();

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::SELECTION);
            settings.setWireframe(vcl::MeshRenderInfo::Wireframe::VISIBLE);
            mesh.setRenderSettings(settings);

            mv.pushDrawableObject(std::move(mesh));
        }, 1.57079632679f);
    }
}
