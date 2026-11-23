// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/meshes.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

TEMPLATE_TEST_CASE(
    "PolyMesh Usage - An empty PolyMesh",
    "",
    vcl::PolyMesh,
    vcl::PolyMeshf,
    vcl::PolyMesh64,
    vcl::PolyMesh64f)
{
    using PolyMesh = TestType;

    using PolyMeshPoint = vcl::PositionType<PolyMesh>;
    PolyMesh m;

    THEN("The size for each container start at 0")
    {
        REQUIRE(m.vertexCount() == 0);
        REQUIRE(m.faceCount() == 0);
    }

    THEN("The optional components are all disabled")
    {
        REQUIRE(m.isVertexQualityAvailable() == false);
        REQUIRE(m.isVertexColorAvailable() == false);
        REQUIRE(m.isVertexAdjacentFacesAvailable() == false);
        REQUIRE(m.isVertexAdjacentVerticesAvailable() == false);
        REQUIRE(m.isVertexPrincipalCurvatureAvailable() == false);
        REQUIRE(m.isVertexTexCoordAvailable() == false);
        REQUIRE(m.vertexCustomComponentNames().size() == 0);
        REQUIRE(m.isFaceQualityAvailable() == false);
        REQUIRE(m.isFaceColorAvailable() == false);
        REQUIRE(m.isFaceAdjacentFacesAvailable() == false);
        REQUIRE(m.isFaceWedgeTexCoordsAvailable() == false);
        REQUIRE(m.faceCustomComponentNames().size() == 0);
    }

    WHEN("Enabling and disabling all the per vertex optional components")
    {
        m.enableAllVertexOptionalComponents();

        REQUIRE(m.isVertexQualityAvailable() == true);
        REQUIRE(m.isVertexColorAvailable() == true);
        REQUIRE(m.isVertexAdjacentFacesAvailable() == true);
        REQUIRE(m.isVertexAdjacentVerticesAvailable() == true);
        REQUIRE(m.isVertexPrincipalCurvatureAvailable() == true);
        REQUIRE(m.isVertexTexCoordAvailable() == true);

        m.disableAllVertexOptionalComponents();

        REQUIRE(m.isVertexQualityAvailable() == false);
        REQUIRE(m.isVertexColorAvailable() == false);
        REQUIRE(m.isVertexAdjacentFacesAvailable() == false);
        REQUIRE(m.isVertexAdjacentVerticesAvailable() == false);
        REQUIRE(m.isVertexPrincipalCurvatureAvailable() == false);
        REQUIRE(m.isVertexTexCoordAvailable() == false);
    }

    WHEN("Adding and removing vertices")
    {
        unsigned int vi0 = m.addVertex();

        REQUIRE(m.vertexCount() == 1);
        REQUIRE(m.faceCount() == 0);
        REQUIRE(m.vertex(0).position() == PolyMeshPoint(0, 0, 0));
        REQUIRE(m.vertex(vi0) == m.vertex(0));

        unsigned int vi1 = m.addVertex();

        REQUIRE(m.vertexCount() == 2);
        REQUIRE(m.vertex(1).position() == PolyMeshPoint(0, 0, 0));
        REQUIRE(m.vertex(vi0) == m.vertex(0));
        REQUIRE(m.vertex(vi1) == m.vertex(1));

        unsigned int vi2 = m.addVertices(5);
        REQUIRE(m.vertexCount() == 7);
        REQUIRE(m.vertex(vi0) == m.vertex(0));
        REQUIRE(m.vertex(vi1) == m.vertex(1));
        REQUIRE(m.vertex(vi2) == m.vertex(2));
    }

    WHEN("Adding and removing vertices and faces")
    {
        REQUIRE(m.vertexCount() == 0);
        REQUIRE(m.faceCount() == 0);
        m.addVertices(4);
        REQUIRE(m.vertexCount() == 4);
        unsigned int fi0 = m.addFace();
        REQUIRE(m.faceCount() == 1);
        REQUIRE(m.face(fi0) == m.face(0));
        m.addFace(0, 1, 2);
        m.face(1).edgeSelected(0) = true;
        m.face(1).edgeSelected(2) = true;
        REQUIRE(m.faceCount() == 2);
        REQUIRE(m.face(1).vertexIndex(0) == 0);
        REQUIRE(m.face(1).vertexIndex(1) == 1);
        REQUIRE(m.face(1).vertexIndex(2) == 2);
        REQUIRE(m.face(1).vertex(0) == m.vertex(0));
        REQUIRE(m.face(1).vertex(1) == m.vertex(1));
        REQUIRE(m.face(1).vertex(2) == m.vertex(2));
        REQUIRE(m.face(1).vertexMod(0) == m.vertex(0));
        REQUIRE(m.face(1).vertexMod(-1) == m.vertex(2));
        REQUIRE(m.face(1).vertexMod(5) == m.vertex(2));
        REQUIRE(m.face(1).vertexMod(-5) == m.vertex(1));
        REQUIRE(m.face(1).vertexIndexMod(0) == 0);
        REQUIRE(m.face(1).vertexIndexMod(-1) == 2);
        REQUIRE(m.face(1).vertexIndexMod(5) == 2);
        REQUIRE(m.face(1).vertexIndexMod(-5) == 1);
        m.face(1).edgeSelected(0) = true;
        m.face(1).edgeSelected(2) = true;
        REQUIRE(m.face(1).edgeSelected(0) == true);
        REQUIRE(m.face(1).edgeSelected(1) == false);
        REQUIRE(m.face(1).edgeSelected(2) == true);
        m.face(1).pushVertex(m.vertex(3));
        REQUIRE(m.face(1).vertexCount() == 4);
        REQUIRE(m.face(1).vertex(3) == m.vertex(3));
        REQUIRE(m.face(1).vertexIndex(3) == 3);
        REQUIRE(m.face(1).edgeSelected(0) == true);
        REQUIRE(m.face(1).edgeSelected(1) == false);
        REQUIRE(m.face(1).edgeSelected(2) == true);
        REQUIRE(m.face(1).edgeSelected(3) == false);
        m.face(1).insertVertex(2, m.vertex(2));
        REQUIRE(m.face(1).vertexCount() == 5);
        REQUIRE(m.face(1).vertexIndex(0) == 0);
        REQUIRE(m.face(1).vertexIndex(1) == 1);
        REQUIRE(m.face(1).vertexIndex(2) == 2);
        REQUIRE(m.face(1).vertexIndex(3) == 2);
        REQUIRE(m.face(1).vertexIndex(4) == 3);
        REQUIRE(m.face(1).edgeSelected(0) == true);
        REQUIRE(m.face(1).edgeSelected(1) == false);
        REQUIRE(m.face(1).edgeSelected(2) == false);
        REQUIRE(m.face(1).edgeSelected(3) == true);
        REQUIRE(m.face(1).edgeSelected(4) == false);
        m.face(1).resizeVertices(6);
        REQUIRE(m.face(1).vertexCount() == 6);
        REQUIRE(m.face(1).vertexIndex(0) == 0);
        REQUIRE(m.face(1).vertexIndex(1) == 1);
        REQUIRE(m.face(1).vertexIndex(2) == 2);
        REQUIRE(m.face(1).vertexIndex(3) == 2);
        REQUIRE(m.face(1).vertexIndex(4) == 3);
        REQUIRE(m.face(1).vertex(5) == nullptr);
        REQUIRE(m.face(1).vertexIndex(5) == m.NULL_INDEX);
        m.face(1).eraseVertex(1);
        REQUIRE(m.face(1).vertexCount() == 5);
        REQUIRE(m.face(1).vertexIndex(0) == 0);
        REQUIRE(m.face(1).vertexIndex(1) == 2);
        REQUIRE(m.face(1).vertexIndex(2) == 2);
        REQUIRE(m.face(1).vertexIndex(3) == 3);
        REQUIRE(m.face(1).vertex(4) == nullptr);
        REQUIRE(m.face(1).vertexIndex(4) == m.NULL_INDEX);
        m.face(1).clearVertices();
        REQUIRE(m.face(1).vertexCount() == 0);

        // restore face 1
        m.face(1).pushVertex(m.vertex(0));
        m.face(1).pushVertex(m.vertex(1));
        m.face(1).pushVertex(m.vertex(2));
        REQUIRE(m.face(1).edgeSelected(0) == false);
        REQUIRE(m.face(1).edgeSelected(1) == false);
        REQUIRE(m.face(1).edgeSelected(2) == false);

        // force reallocation of vertex container
        m.addVertices(100);
        REQUIRE(m.vertexCount() == 104);
        REQUIRE(m.vertexContainerSize() == 104);
        REQUIRE(m.face(1).vertexIndex(0) == 0);
        REQUIRE(m.face(1).vertexIndex(1) == 1);
        REQUIRE(m.face(1).vertexIndex(2) == 2);
        REQUIRE(m.face(1).vertex(0) == m.vertex(0));
        REQUIRE(m.face(1).vertex(1) == m.vertex(1));
        REQUIRE(m.face(1).vertex(2) == m.vertex(2));

        m.face(1).setVertex(2, m.vertex(3));
        m.deleteVertex(2);
        REQUIRE(m.vertexCount() == 103);
        REQUIRE(m.vertexContainerSize() == 104);
        REQUIRE(m.face(1).vertexIndex(0) == 0);
        REQUIRE(m.face(1).vertexIndex(1) == 1);
        REQUIRE(m.face(1).vertexIndex(2) == 3);
        REQUIRE(m.face(1).vertex(0) == m.vertex(0));
        REQUIRE(m.face(1).vertex(1) == m.vertex(1));
        REQUIRE(m.face(1).vertex(2) == m.vertex(3));
        m.compactVertices();
        REQUIRE(m.vertexCount() == 103);
        REQUIRE(m.vertexContainerSize() == 103);
        REQUIRE(m.face(1).vertexIndex(0) == 0);
        REQUIRE(m.face(1).vertexIndex(1) == 1);
        REQUIRE(m.face(1).vertexIndex(2) == 2);
        REQUIRE(m.face(1).vertex(0) == m.vertex(0));
        REQUIRE(m.face(1).vertex(1) == m.vertex(1));
        REQUIRE(m.face(1).vertex(2) == m.vertex(2));
    }
}
