// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "get_drawable_mesh.h"
#include "run_render_test.h"

static const std::string TEST_NAME = "002-mesh-surface-headless";

TEST_CASE("Mesh Surface Shading Modes")
{
    SECTION("Shading None")
    {
        runRenderTest(
            TEST_NAME, "shading_none", [](vcl::HeadlessMeshViewer& mv) {
                auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

                auto settings = mesh.renderSettings();
                settings.setSurface(vcl::MeshRenderInfo::Surface::SHADING_NONE);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            });
    }

    SECTION("Shading Flat")
    {
        runRenderTest(
            TEST_NAME,
            "shading_flat",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

                auto settings = mesh.renderSettings();
                settings.setSurface(vcl::MeshRenderInfo::Surface::SHADING_FLAT);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            },
            0.f);
    }

    SECTION("Shading Smooth")
    {
        runRenderTest(
            TEST_NAME, "shading_smooth", [](vcl::HeadlessMeshViewer& mv) {
                auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

                auto settings = mesh.renderSettings();
                settings.setSurface(
                    vcl::MeshRenderInfo::Surface::SHADING_SMOOTH);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            });
    }

    SECTION("Shading Normal Map")
    {
        runRenderTest(
            TEST_NAME, "shading_normal_map", [](vcl::HeadlessMeshViewer& mv) {
                auto mesh = getDrawableMesh<vcl::TriMesh>(
                    "gltf/NormalTangentMirrorTest/"
                    "NormalTangentMirrorTest.gltf");

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
        runRenderTest(TEST_NAME, "color_user", [](vcl::HeadlessMeshViewer& mv) {
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
        runRenderTest(TEST_NAME, "color_mesh", [](vcl::HeadlessMeshViewer& mv) {
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
        runRenderTest(
            TEST_NAME, "color_vertex", [](vcl::HeadlessMeshViewer& mv) {
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
        runRenderTest(
            TEST_NAME,
            "color_face",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh =
                    getDrawableMesh<vcl::TriMesh>("bunny_simplified.obj");

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
            },
            0.f);
    }

    SECTION("Color Vertex Texcoord")
    {
        runRenderTest(
            TEST_NAME,
            "color_vertex_texcoord",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh = getDrawableMesh<vcl::TriMesh>(
                    "gltf/DamagedHelmet/DamagedHelmet.gltf");

                auto settings = mesh.renderSettings();
                settings.setSurface(
                    vcl::MeshRenderInfo::Surface::COLOR_VERTEX_TEX);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            });
    }

    SECTION("Color Wedge Texcoord")
    {
        runRenderTest(
            TEST_NAME, "color_wedge_texcoord", [](vcl::HeadlessMeshViewer& mv) {
                auto mesh =
                    getDrawableMesh<vcl::TriMesh>("spot/spot_triangulated.obj");

                auto settings = mesh.renderSettings();
                settings.setSurface(
                    vcl::MeshRenderInfo::Surface::COLOR_WEDGE_TEX);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            });
    }
}

TEST_CASE("Mesh Surface Selection")
{
    SECTION("Face Selection")
    {
        runRenderTest(
            TEST_NAME,
            "surface_selection",
            [](vcl::HeadlessMeshViewer& mv) {
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
            },
            0.f);
    }

    SECTION("Polygon Selection")
    {
        runRenderTest(
            TEST_NAME,
            "polygon_selection",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh = getDrawableMesh<vcl::PolyMesh>(
                    "spot/spot_quadrangulated.obj");

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
            },
            1.57079632679f);
    }
}
