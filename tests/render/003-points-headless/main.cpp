// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "get_drawable_mesh.h"
#include "run_render_test.h"

static const std::string TEST_NAME = "003-points-headless";

TEST_CASE("Points Shading Modes")
{
    SECTION("Shading None")
    {
        runRenderTest(
            TEST_NAME,
            "shading_none",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

                auto settings = mesh.renderSettings();
                settings.setSurface(
                    vcl::MeshRenderInfo::Surface::VISIBLE, false);
                settings.setPoints(vcl::MeshRenderInfo::Points::VISIBLE, true);
                settings.setPoints(vcl::MeshRenderInfo::Points::SHADING_NONE);
                settings.setPointsWidth(4.0f);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            },
            0.0f,
            false,
            -150.0f);
    }

    SECTION("Shading Vert")
    {
        runRenderTest(
            TEST_NAME,
            "shading_vert",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

                auto settings = mesh.renderSettings();
                settings.setSurface(
                    vcl::MeshRenderInfo::Surface::VISIBLE, false);
                settings.setPoints(vcl::MeshRenderInfo::Points::VISIBLE, true);
                settings.setPoints(vcl::MeshRenderInfo::Points::SHADING_VERT);
                settings.setPointsWidth(4.0f);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            },
            0.0f,
            false,
            -150.0f);
    }
}

TEST_CASE("Points Color Modes")
{
    SECTION("Color User")
    {
        runRenderTest(
            TEST_NAME,
            "color_user",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

                auto settings = mesh.renderSettings();
                settings.setSurface(
                    vcl::MeshRenderInfo::Surface::VISIBLE, false);
                settings.setPoints(vcl::MeshRenderInfo::Points::VISIBLE, true);
                settings.setPointsUserColor(vcl::Color::Red);
                settings.setPoints(vcl::MeshRenderInfo::Points::COLOR_USER);
                settings.setPointsWidth(4.0f);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            },
            0.0f,
            false,
            -150.0f);
    }

    SECTION("Color Mesh")
    {
        runRenderTest(
            TEST_NAME,
            "color_mesh",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");
                mesh.color() =
                    vcl::Color::Blue; // Set a color different from default/user

                auto settings = mesh.renderSettings();
                settings.setSurface(
                    vcl::MeshRenderInfo::Surface::VISIBLE, false);
                settings.setPoints(vcl::MeshRenderInfo::Points::VISIBLE, true);
                settings.setPoints(vcl::MeshRenderInfo::Points::COLOR_MESH);
                settings.setPointsUserColor(
                    vcl::Color::Green); // Different color to ensure it's not
                                        // used
                settings.setPointsWidth(4.0f);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            },
            0.0f,
            false,
            -150.0f);
    }

    SECTION("Color Vertex")
    {
        runRenderTest(
            TEST_NAME,
            "color_vertex",
            [](vcl::HeadlessMeshViewer& mv) {
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
                settings.setSurface(
                    vcl::MeshRenderInfo::Surface::VISIBLE, false);
                settings.setPoints(vcl::MeshRenderInfo::Points::VISIBLE, true);
                settings.setPoints(vcl::MeshRenderInfo::Points::COLOR_VERTEX);
                settings.setPointsWidth(4.0f);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            },
            0.0f,
            false,
            -150.0f);
    }
}

TEST_CASE("Points Shape Modes")
{
    SECTION("Shape Pixel")
    {
        runRenderTest(
            TEST_NAME,
            "shape_pixel",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

                auto settings = mesh.renderSettings();
                settings.setSurface(
                    vcl::MeshRenderInfo::Surface::VISIBLE, false);
                settings.setPoints(vcl::MeshRenderInfo::Points::VISIBLE, true);
                settings.setPoints(vcl::MeshRenderInfo::Points::SHAPE_PIXEL);
                settings.setPointsWidth(4.0f);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            },
            0.0f,
            false,
            -150.0f);
    }

    SECTION("Shape Circle")
    {
        runRenderTest(
            TEST_NAME,
            "shape_circle",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

                auto settings = mesh.renderSettings();
                settings.setSurface(
                    vcl::MeshRenderInfo::Surface::VISIBLE, false);
                settings.setPoints(vcl::MeshRenderInfo::Points::VISIBLE, true);
                settings.setPoints(vcl::MeshRenderInfo::Points::SHAPE_CIRCLE);
                settings.setPointsWidth(4.0f);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            },
            0.0f,
            false,
            -150.0f);
    }
}

TEST_CASE("Points Selection")
{
    SECTION("Vertex Selection")
    {
        runRenderTest(
            TEST_NAME,
            "selection",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

                mesh.updateRenderSettingsCapabilities();

                // Deterministically assign selection to vertices
                for (auto& v : mesh.vertices()) {
                    if (v.index() % 5 == 0)
                        v.selected() = true;
                }
                mesh.updateBuffers();

                auto settings = mesh.renderSettings();
                settings.setSurface(
                    vcl::MeshRenderInfo::Surface::VISIBLE, false);
                settings.setPoints(vcl::MeshRenderInfo::Points::VISIBLE, true);
                settings.setPoints(vcl::MeshRenderInfo::Points::SELECTION);
                settings.setPointsWidth(4.0f);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            },
            0.0f,
            true,
            -150.0f);
    }
}

TEST_CASE("Points Depth Offset")
{
    SECTION("Depth Offset")
    {
        runRenderTest(
            TEST_NAME,
            "depth_offset",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh = getDrawableMesh<vcl::TriMesh>("bimba.obj");

                auto settings = mesh.renderSettings();
                // Surface visible, shading smooth
                settings.setSurface(
                    vcl::MeshRenderInfo::Surface::VISIBLE, true);
                settings.setSurface(
                    vcl::MeshRenderInfo::Surface::SHADING_SMOOTH);

                // Points visible, should render on top because of depth offset
                settings.setPoints(vcl::MeshRenderInfo::Points::VISIBLE, true);
                settings.setPointsWidth(4.0f);
                settings.setPointsUserColor(vcl::Color::Red);
                settings.setPoints(vcl::MeshRenderInfo::Points::COLOR_USER);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            },
            0.0f,
            false,
            -150.0f);
    }
}
