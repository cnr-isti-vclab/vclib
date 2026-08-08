// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "get_drawable_mesh.h"
#include "run_render_test.h"

static const std::string TEST_NAME = "005-mesh-wireframe-headless";

void addPerVertexColor(auto& m)
{
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

    m.updateBuffers();
}

TEST_CASE("Wireframe Shading Modes")
{
    SECTION("Shading None")
    {
        runRenderTest(
            TEST_NAME,
            "shading_none",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh =
                    getDrawableMesh<vcl::TriMesh>("bunny_simplified.obj");

                auto settings = mesh.renderSettings();
                settings.setSurface(
                    vcl::MeshRenderInfo::Surface::VISIBLE, false);
                settings.setWireframe(
                    vcl::MeshRenderInfo::Wireframe::VISIBLE, true);
                settings.setWireframe(
                    vcl::MeshRenderInfo::Wireframe::SHADING_NONE);
                settings.setWireframeUserColor(vcl::Color::Green);
                settings.setWireframe(
                    vcl::MeshRenderInfo::Wireframe::COLOR_USER);
                settings.setWireframeWidth(3);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            },
            0.f,
            false,
            -150.f);
    }

    SECTION("Shading Vert")
    {
        runRenderTest(
            TEST_NAME,
            "shading_vert",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh =
                    getDrawableMesh<vcl::TriMesh>("bunny_simplified.obj");

                auto settings = mesh.renderSettings();
                settings.setSurface(
                    vcl::MeshRenderInfo::Surface::VISIBLE, false);
                settings.setWireframe(
                    vcl::MeshRenderInfo::Wireframe::VISIBLE, true);
                settings.setWireframe(
                    vcl::MeshRenderInfo::Wireframe::SHADING_VERT);
                settings.setWireframeUserColor(vcl::Color::Green);
                settings.setWireframe(
                    vcl::MeshRenderInfo::Wireframe::COLOR_USER);
                settings.setWireframeWidth(3);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            },
            0.f,
            false,
            -150.f);
    }
}

TEST_CASE("Wireframe Color Modes")
{
    SECTION("Color User")
    {
        runRenderTest(
            TEST_NAME,
            "color_user",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh =
                    getDrawableMesh<vcl::TriMesh>("bunny_simplified.obj");

                auto settings = mesh.renderSettings();
                settings.setSurface(
                    vcl::MeshRenderInfo::Surface::VISIBLE, false);
                settings.setWireframe(
                    vcl::MeshRenderInfo::Wireframe::VISIBLE, true);
                settings.setWireframeUserColor(vcl::Color::Red);
                settings.setWireframe(
                    vcl::MeshRenderInfo::Wireframe::COLOR_USER);
                settings.setWireframeWidth(3);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            },
            0.f,
            false,
            -150.f);
    }

    SECTION("Color Mesh")
    {
        runRenderTest(
            TEST_NAME,
            "color_mesh",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh =
                    getDrawableMesh<vcl::TriMesh>("bunny_simplified.obj");
                mesh.color() =
                    vcl::Color::Blue; // Set a color different from default/user
                mesh.updateBuffers(
                    {vcl::MeshRenderInfo::Buffers::MESH_ADDITIONAL_DATA});

                auto settings = mesh.renderSettings();
                settings.setSurface(
                    vcl::MeshRenderInfo::Surface::VISIBLE, false);
                settings.setWireframe(
                    vcl::MeshRenderInfo::Wireframe::VISIBLE, true);
                settings.setWireframe(
                    vcl::MeshRenderInfo::Wireframe::COLOR_MESH);
                settings.setWireframeUserColor(
                    vcl::Color::Green); // Different color to ensure it's not
                                        // used
                settings.setWireframeWidth(3);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            },
            0.f,
            false,
            -150.f);
    }

    SECTION("Color Vertex")
    {
        runRenderTest(
            TEST_NAME,
            "color_vertex",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh =
                    getDrawableMesh<vcl::TriMesh>("bunny_simplified.obj");
                addPerVertexColor(mesh);

                auto settings = mesh.renderSettings();
                settings.setSurface(
                    vcl::MeshRenderInfo::Surface::VISIBLE, false);
                settings.setWireframe(
                    vcl::MeshRenderInfo::Wireframe::VISIBLE, true);
                settings.setWireframe(
                    vcl::MeshRenderInfo::Wireframe::COLOR_VERTEX);
                settings.setWireframeWidth(3);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            },
            0.f,
            false,
            -150.f);
    }
}

TEST_CASE("Wireframe Depth Offset")
{
    SECTION("Depth Offset")
    {
        runRenderTest(
            TEST_NAME,
            "depth_offset",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh =
                    getDrawableMesh<vcl::TriMesh>("bunny_simplified.obj");

                auto settings = mesh.renderSettings();
                // Surface visible, shading smooth
                settings.setSurface(
                    vcl::MeshRenderInfo::Surface::VISIBLE, true);
                settings.setSurface(vcl::MeshRenderInfo::Surface::COLOR_USER);
                settings.setSurface(
                    vcl::MeshRenderInfo::Surface::SHADING_SMOOTH);

                // Wireframe visible, should render on top because of depth
                // offset
                settings.setWireframe(
                    vcl::MeshRenderInfo::Wireframe::VISIBLE, true);
                settings.setWireframeWidth(3);
                settings.setWireframeUserColor(vcl::Color::Red);
                settings.setWireframe(
                    vcl::MeshRenderInfo::Wireframe::COLOR_USER);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            },
            0.f,
            false,
            -150.f);
    }
}

TEST_CASE("Wireframe Cross Section")
{
    SECTION("Vertex Cross Section")
    {
        runRenderTest("cross_section_vertex", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bunny_simplified.obj");

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::VISIBLE, false);
            settings.setWireframe(
                vcl::MeshRenderInfo::Wireframe::VISIBLE, true);
            settings.setWireframeWidth(3);
            mesh.setRenderSettings(settings);

            vcl::CrossSectionSettings css(mesh);
            css.type() =
                vcl::CrossSectionSettings::CrossSectionType::PER_VERTEX;

            vcl::Point3f min = css.boundingBox().min();
            vcl::Point3f max = css.boundingBox().max();
            css.setLowerUpper(min + (max - min) * 0.25f,
                              max - (max - min) * 0.25f);

            mesh.setCrossSectionSettings(css);

            mv.pushDrawableObject(std::move(mesh));
        });
    }

    SECTION("Fragment Cross Section")
    {
        runRenderTest("cross_section_fragment", [](vcl::HeadlessMeshViewer& mv) {
            auto mesh = getDrawableMesh<vcl::TriMesh>("bunny_simplified.obj");

            auto settings = mesh.renderSettings();
            settings.setSurface(vcl::MeshRenderInfo::Surface::VISIBLE, false);
            settings.setWireframe(
                vcl::MeshRenderInfo::Wireframe::VISIBLE, true);
            settings.setWireframeWidth(3);
            mesh.setRenderSettings(settings);

            vcl::CrossSectionSettings css(mesh);
            css.type() =
                vcl::CrossSectionSettings::CrossSectionType::PER_FRAGMENT;

            vcl::Point3f min = css.boundingBox().min();
            vcl::Point3f max = css.boundingBox().max();
            css.setLowerUpper(min + (max - min) * 0.25f,
                              max - (max - min) * 0.25f);

            mesh.setCrossSectionSettings(css);

            mv.pushDrawableObject(std::move(mesh));
        });
    }
}
