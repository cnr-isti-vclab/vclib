// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "run_render_test.h"

#include <vclib/meshes.h>

static const std::string TEST_NAME = "004-lines-headless";

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

TEST_CASE("Lines Shading Modes")
{
    SECTION("Shading None")
    {
        runRenderTest(
            TEST_NAME,
            "shading_none",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh = getDrawableEdgeMesh();

                auto settings = mesh.renderSettings();
                settings.setEdges(vcl::MeshRenderInfo::Edges::VISIBLE, true);
                settings.setEdges(vcl::MeshRenderInfo::Edges::SHADING_NONE);
                settings.setEdgesWidth(4);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            },
            0.f,
            false,
            -150.f);
    }

    SECTION("Shading Smooth")
    {
        runRenderTest(
            TEST_NAME,
            "shading_smooth",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh = getDrawableEdgeMesh();

                auto settings = mesh.renderSettings();
                settings.setEdges(vcl::MeshRenderInfo::Edges::VISIBLE, true);
                settings.setEdges(vcl::MeshRenderInfo::Edges::SHADING_SMOOTH);
                settings.setEdgesWidth(4);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            },
            0.f,
            false,
            -150.f);
    }
}

TEST_CASE("Lines Color Modes")
{
    SECTION("Color User")
    {
        runRenderTest(
            TEST_NAME,
            "color_user",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh = getDrawableEdgeMesh();

                auto settings = mesh.renderSettings();
                settings.setEdges(vcl::MeshRenderInfo::Edges::VISIBLE, true);
                settings.setEdgesUserColor(vcl::Color::DarkYellow);
                settings.setEdges(vcl::MeshRenderInfo::Edges::COLOR_USER);
                settings.setEdgesWidth(4);
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
                auto mesh = getDrawableEdgeMesh();

                auto settings = mesh.renderSettings();
                settings.setEdges(vcl::MeshRenderInfo::Edges::VISIBLE, true);
                settings.setEdges(vcl::MeshRenderInfo::Edges::COLOR_MESH);
                settings.setEdgesUserColor(
                    vcl::Color::Green); // Different color to ensure it's not
                                        // used
                settings.setEdgesWidth(4);
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
                auto mesh = getDrawableEdgeMesh();

                auto settings = mesh.renderSettings();
                settings.setEdges(vcl::MeshRenderInfo::Edges::VISIBLE, true);
                settings.setEdges(vcl::MeshRenderInfo::Edges::COLOR_VERTEX);
                settings.setEdgesWidth(4);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            },
            0.f,
            false,
            -150.f);
    }

    SECTION("Color Edge")
    {
        runRenderTest(
            TEST_NAME,
            "color_edge",
            [](vcl::HeadlessMeshViewer& mv) {
                auto mesh = getDrawableEdgeMesh();

                auto settings = mesh.renderSettings();
                settings.setEdges(vcl::MeshRenderInfo::Edges::VISIBLE, true);
                settings.setEdges(vcl::MeshRenderInfo::Edges::COLOR_EDGE);
                settings.setEdgesWidth(4);
                mesh.setRenderSettings(settings);

                mv.pushDrawableObject(std::move(mesh));
            },
            0.f,
            true,
            -150.f);
    }
}
