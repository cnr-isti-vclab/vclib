/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <vclib/meshes.h>

TEMPLATE_TEST_CASE(
    "Test empty TriMesh",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf)
{
    using TriMesh = TestType;

    using TriMeshPoint = TriMesh::VertexType::CoordType;
    TriMesh m;

    THEN("The size for each container start at 0")
    {
        REQUIRE(m.vertexNumber() == 0);
        REQUIRE(m.faceNumber() == 0);
    }

    THEN("The optional components are all disabled")
    {
        REQUIRE(m.isPerVertexQualityEnabled() == false);
        REQUIRE(m.isPerVertexColorEnabled() == false);
        REQUIRE(m.isPerVertexAdjacentFacesEnabled() == false);
        REQUIRE(m.isPerVertexAdjacentVerticesEnabled() == false);
        REQUIRE(m.isPerVertexPrincipalCurvatureEnabled() == false);
        REQUIRE(m.isPerVertexTexCoordEnabled() == false);
        REQUIRE(m.isPerVertexMarkEnabled() == false);
        REQUIRE(m.perVertexCustomComponentNames().size() == 0);
        REQUIRE(m.isPerFaceQualityEnabled() == false);
        REQUIRE(m.isPerFaceColorEnabled() == false);
        REQUIRE(m.isPerFaceAdjacentFacesEnabled() == false);
        REQUIRE(m.isPerFaceWedgeTexCoordsEnabled() == false);
        REQUIRE(m.isPerFaceMarkEnabled() == false);
        REQUIRE(m.perFaceCustomComponentNames().size() == 0);
    }

    WHEN("Enabling and disabling all the per vertex optional components")
    {
        m.enableAllPerVertexOptionalComponents();

        REQUIRE(m.isPerVertexQualityEnabled() == true);
        REQUIRE(m.isPerVertexColorEnabled() == true);
        REQUIRE(m.isPerVertexAdjacentFacesEnabled() == true);
        REQUIRE(m.isPerVertexAdjacentVerticesEnabled() == true);
        REQUIRE(m.isPerVertexPrincipalCurvatureEnabled() == true);
        REQUIRE(m.isPerVertexTexCoordEnabled() == true);
        REQUIRE(m.isPerVertexMarkEnabled() == true);

        m.disableAllPerVertexOptionalComponents();

        REQUIRE(m.isPerVertexQualityEnabled() == false);
        REQUIRE(m.isPerVertexColorEnabled() == false);
        REQUIRE(m.isPerVertexAdjacentFacesEnabled() == false);
        REQUIRE(m.isPerVertexAdjacentVerticesEnabled() == false);
        REQUIRE(m.isPerVertexPrincipalCurvatureEnabled() == false);
        REQUIRE(m.isPerVertexTexCoordEnabled() == false);
        REQUIRE(m.isPerVertexMarkEnabled() == false);
    }

    WHEN("Adding and removing vertices")
    {
        uint vi0 = m.addVertex();

        REQUIRE(m.vertexNumber() == 1);
        REQUIRE(m.faceNumber() == 0);
        REQUIRE(m.vertex(0).coord() == TriMeshPoint(0, 0, 0));
        REQUIRE(&m.vertex(vi0) == &m.vertex(0));

        uint vi1 = m.addVertex();

        REQUIRE(m.vertexNumber() == 2);
        REQUIRE(m.vertex(1).coord() == TriMeshPoint(0, 0, 0));
        REQUIRE(&m.vertex(vi0) == &m.vertex(0));
        REQUIRE(&m.vertex(vi1) == &m.vertex(1));

        uint vi2 = m.addVertices(5);
        REQUIRE(m.vertexNumber() == 7);
        REQUIRE(&m.vertex(vi0) == &m.vertex(0));
        REQUIRE(&m.vertex(vi1) == &m.vertex(1));
        REQUIRE(&m.vertex(vi2) == &m.vertex(2));
    }

    WHEN("Adding and removing vertices and faces")
    {
        REQUIRE(m.vertexNumber() == 0);
        REQUIRE(m.faceNumber() == 0);
        m.addVertices(3);
        REQUIRE(m.vertexNumber() == 3);
        uint fi0 = m.addFace();
        REQUIRE(m.faceNumber() == 1);
        REQUIRE(&m.face(fi0) == &m.face(0));
        m.addFace(0, 1, 2);
        REQUIRE(m.faceNumber() == 2);
        REQUIRE(m.face(1).vertexIndex(0) == 0);
        REQUIRE(m.face(1).vertexIndex(1) == 1);
        REQUIRE(m.face(1).vertexIndex(2) == 2);
        REQUIRE(m.face(1).vertex(0) == &m.vertex(0));
        REQUIRE(m.face(1).vertex(1) == &m.vertex(1));
        REQUIRE(m.face(1).vertex(2) == &m.vertex(2));
        REQUIRE(m.face(1).vertexMod(0) == &m.vertex(0));
        REQUIRE(m.face(1).vertexMod(-1) == &m.vertex(2));
        REQUIRE(m.face(1).vertexMod(5) == &m.vertex(2));
        REQUIRE(m.face(1).vertexMod(-5) == &m.vertex(1));
        REQUIRE(m.face(1).vertexIndexMod(0) == 0);
        REQUIRE(m.face(1).vertexIndexMod(-1) == 2);
        REQUIRE(m.face(1).vertexIndexMod(5) == 2);
        REQUIRE(m.face(1).vertexIndexMod(-5) == 1);

        // test also iterators
        uint vi = 0;
        for (auto* v : m.face(1).vertices()) {
            REQUIRE(v == &m.vertex(vi++));
        }

        vi = 0;
        for (uint v : m.face(1).vertices() | vcl::views::indices) {
            REQUIRE(v == vi++);
        }

        vi = 0;
        for (auto v : m.face(1).vertexIndices()) {
            REQUIRE(v == vi++);
        }

        // force reallocation of vertex container
        m.addVertices(100);
        REQUIRE(m.vertexNumber() == 103);
        REQUIRE(m.vertexContainerSize() == 103);
        REQUIRE(m.face(1).vertexIndex(0) == 0);
        REQUIRE(m.face(1).vertexIndex(1) == 1);
        REQUIRE(m.face(1).vertexIndex(2) == 2);
        REQUIRE(m.face(1).vertex(0) == &m.vertex(0));
        REQUIRE(m.face(1).vertex(1) == &m.vertex(1));
        REQUIRE(m.face(1).vertex(2) == &m.vertex(2));

        m.face(1).setVertex(2, 3);
        m.deleteVertex(2);
        REQUIRE(m.vertexNumber() == 102);
        REQUIRE(m.vertexContainerSize() == 103);
        REQUIRE(m.face(1).vertexIndex(0) == 0);
        REQUIRE(m.face(1).vertexIndex(1) == 1);
        REQUIRE(m.face(1).vertexIndex(2) == 3);
        REQUIRE(m.face(1).vertex(0) == &m.vertex(0));
        REQUIRE(m.face(1).vertex(1) == &m.vertex(1));
        REQUIRE(m.face(1).vertex(2) == &m.vertex(3));
        m.compactVertices();
        REQUIRE(m.vertexNumber() == 102);
        REQUIRE(m.vertexContainerSize() == 102);
        REQUIRE(m.face(1).vertexIndex(0) == 0);
        REQUIRE(m.face(1).vertexIndex(1) == 1);
        REQUIRE(m.face(1).vertexIndex(2) == 2);
        REQUIRE(m.face(1).vertex(0) == &m.vertex(0));
        REQUIRE(m.face(1).vertex(1) == &m.vertex(1));
        REQUIRE(m.face(1).vertex(2) == &m.vertex(2));
    }

    WHEN("Adding Faces with setVertices")
    {
        m.addVertices(5);
        m.addFaces(4);

        m.face(0).setVertices(0, 1, 2);
        REQUIRE(m.face(0).vertexIndex(0) == 0);
        REQUIRE(m.face(0).vertexIndex(1) == 1);
        REQUIRE(m.face(0).vertexIndex(2) == 2);
        REQUIRE(m.face(0).vertex(0) == &m.vertex(0));
        REQUIRE(m.face(0).vertex(1) == &m.vertex(1));
        REQUIRE(m.face(0).vertex(2) == &m.vertex(2));

        m.face(1).setVertices(&m.vertex(2), &m.vertex(0), &m.vertex(1));
        REQUIRE(m.face(1).vertexIndex(0) == 2);
        REQUIRE(m.face(1).vertexIndex(1) == 0);
        REQUIRE(m.face(1).vertexIndex(2) == 1);
        REQUIRE(m.face(1).vertex(0) == &m.vertex(2));
        REQUIRE(m.face(1).vertex(1) == &m.vertex(0));
        REQUIRE(m.face(1).vertex(2) == &m.vertex(1));

        std::list<uint> l1 = {4, 3, 1};
        m.face(2).setVertices(l1);
        REQUIRE(m.face(2).vertexIndex(0) == 4);
        REQUIRE(m.face(2).vertexIndex(1) == 3);
        REQUIRE(m.face(2).vertexIndex(2) == 1);
        REQUIRE(m.face(2).vertex(0) == &m.vertex(4));
        REQUIRE(m.face(2).vertex(1) == &m.vertex(3));
        REQUIRE(m.face(2).vertex(2) == &m.vertex(1));

        std::list<typename TriMesh::Vertex*> l2 = {
            &m.vertex(2), &m.vertex(4), &m.vertex(3)};
        m.face(3).setVertices(l2);
        REQUIRE(m.face(3).vertexIndex(0) == 2);
        REQUIRE(m.face(3).vertexIndex(1) == 4);
        REQUIRE(m.face(3).vertexIndex(2) == 3);
        REQUIRE(m.face(3).vertex(0) == &m.vertex(2));
        REQUIRE(m.face(3).vertex(1) == &m.vertex(4));
        REQUIRE(m.face(3).vertex(2) == &m.vertex(3));
    }
}

TEMPLATE_TEST_CASE(
    "Test a cube TriMesh",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf)
{
    using TriMesh = TestType;

    TriMesh m;
    using PointT = TriMesh::VertexType::CoordType;

    m.addVertices(
        PointT(-0.5, -0.5, 0.5),
        PointT(0.5, -0.5, 0.5),
        PointT(-0.5, 0.5, 0.5),
        PointT(0.5, 0.5, 0.5),
        PointT(-0.5, 0.5, -0.5),
        PointT(0.5, 0.5, -0.5),
        PointT(-0.5, -0.5, -0.5),
        PointT(0.5, -0.5, -0.5));
    m.reserveFaces(12);
    m.addFace(0, 1, 2);
    m.addFace(1, 3, 2);
    m.addFace(2, 3, 4);
    m.addFace(3, 5, 4);
    m.addFace(4, 5, 6);
    m.addFace(5, 7, 6);
    m.addFace(6, 7, 0);
    m.addFace(7, 1, 0);
    m.addFace(1, 7, 3);
    m.addFace(7, 5, 3);
    m.addFace(5, 7, 6);
    m.addFace(7, 5, 4);

    REQUIRE(m.vertexNumber() == 8);
    REQUIRE(m.faceNumber() == 12);

    THEN("Test Vertex References queries")
    {
        REQUIRE(m.face(0).containsVertex(&m.vertex(0)) == true);
        REQUIRE(m.face(0).containsVertex(&m.vertex(1)) == true);
        REQUIRE(m.face(0).containsVertex(&m.vertex(2)) == true);
        REQUIRE(m.face(0).containsVertex(&m.vertex(3)) == false);

        REQUIRE(m.face(0).containsVertex((uint) 0) == true);
        REQUIRE(m.face(0).containsVertex(1) == true);
        REQUIRE(m.face(0).containsVertex(2) == true);
        REQUIRE(m.face(0).containsVertex(3) == false);

        REQUIRE(m.face(1).containsVertex(&m.vertex(1)) == true);
        REQUIRE(m.face(1).containsVertex(&m.vertex(4)) == false);
        REQUIRE(m.face(1).containsVertex(1) == true);
        REQUIRE(m.face(1).containsVertex(4) == false);

        REQUIRE(m.face(2).indexOfVertex(&m.vertex(0)) == vcl::UINT_NULL);
        REQUIRE(m.face(2).indexOfVertex(&m.vertex(3)) == 1);
        REQUIRE(m.face(2).indexOfVertex((uint) 0) == vcl::UINT_NULL);
        REQUIRE(m.face(2).indexOfVertex(4) == 2);

        REQUIRE(m.face(3).indexOfEdge(&m.vertex(5), &m.vertex(4)) == 1);
        REQUIRE(m.face(3).indexOfEdge(&m.vertex(4), &m.vertex(5)) == 1);
        REQUIRE(m.face(3).indexOfEdge(4, 3) == 2);
        REQUIRE(m.face(3).indexOfEdge(3, 4) == 2);
    }
}
