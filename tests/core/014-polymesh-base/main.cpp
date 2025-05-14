/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#include <vclib/meshes.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

TEMPLATE_TEST_CASE(
    "PolyMesh Usage - An empty PolyMesh",
    "",
    vcl::PolyMesh,
    vcl::PolyMeshf,
    vcl::PolyMeshIndexed,
    vcl::PolyMeshIndexedf)
{
    using PolyMesh = TestType;

    using PolyMeshPoint = PolyMesh::VertexType::PositionType;
    PolyMesh m;

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
        unsigned int vi0 = m.addVertex();

        REQUIRE(m.vertexNumber() == 1);
        REQUIRE(m.faceNumber() == 0);
        REQUIRE(m.vertex(0).position() == PolyMeshPoint(0, 0, 0));
        REQUIRE(&m.vertex(vi0) == &m.vertex(0));

        unsigned int vi1 = m.addVertex();

        REQUIRE(m.vertexNumber() == 2);
        REQUIRE(m.vertex(1).position() == PolyMeshPoint(0, 0, 0));
        REQUIRE(&m.vertex(vi0) == &m.vertex(0));
        REQUIRE(&m.vertex(vi1) == &m.vertex(1));

        unsigned int vi2 = m.addVertices(5);
        REQUIRE(m.vertexNumber() == 7);
        REQUIRE(&m.vertex(vi0) == &m.vertex(0));
        REQUIRE(&m.vertex(vi1) == &m.vertex(1));
        REQUIRE(&m.vertex(vi2) == &m.vertex(2));
    }

    WHEN("Adding and removing vertices and faces")
    {
        REQUIRE(m.vertexNumber() == 0);
        REQUIRE(m.faceNumber() == 0);
        m.addVertices(4);
        REQUIRE(m.vertexNumber() == 4);
        unsigned int fi0 = m.addFace();
        REQUIRE(m.faceNumber() == 1);
        REQUIRE(&m.face(fi0) == &m.face(0));
        m.addFace(0, 1, 2);
        m.face(1).edgeSelected(0) = true;
        m.face(1).edgeSelected(2) = true;
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
        m.face(1).edgeSelected(0) = true;
        m.face(1).edgeSelected(2) = true;
        REQUIRE(m.face(1).edgeSelected(0) == true);
        REQUIRE(m.face(1).edgeSelected(1) == false);
        REQUIRE(m.face(1).edgeSelected(2) == true);
        m.face(1).pushVertex(&m.vertex(3));
        REQUIRE(m.face(1).vertexNumber() == 4);
        REQUIRE(m.face(1).vertex(3) == &m.vertex(3));
        REQUIRE(m.face(1).vertexIndex(3) == 3);
        REQUIRE(m.face(1).edgeSelected(0) == true);
        REQUIRE(m.face(1).edgeSelected(1) == false);
        REQUIRE(m.face(1).edgeSelected(2) == true);
        REQUIRE(m.face(1).edgeSelected(3) == false);
        m.face(1).insertVertex(2, &m.vertex(2));
        REQUIRE(m.face(1).vertexNumber() == 5);
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
        REQUIRE(m.face(1).vertexNumber() == 6);
        REQUIRE(m.face(1).vertexIndex(0) == 0);
        REQUIRE(m.face(1).vertexIndex(1) == 1);
        REQUIRE(m.face(1).vertexIndex(2) == 2);
        REQUIRE(m.face(1).vertexIndex(3) == 2);
        REQUIRE(m.face(1).vertexIndex(4) == 3);
        REQUIRE(m.face(1).vertex(5) == nullptr);
        REQUIRE(m.face(1).vertexIndex(5) == vcl::UINT_NULL);
        m.face(1).eraseVertex(1);
        REQUIRE(m.face(1).vertexNumber() == 5);
        REQUIRE(m.face(1).vertexIndex(0) == 0);
        REQUIRE(m.face(1).vertexIndex(1) == 2);
        REQUIRE(m.face(1).vertexIndex(2) == 2);
        REQUIRE(m.face(1).vertexIndex(3) == 3);
        REQUIRE(m.face(1).vertex(4) == nullptr);
        REQUIRE(m.face(1).vertexIndex(4) == vcl::UINT_NULL);
        m.face(1).clearVertices();
        REQUIRE(m.face(1).vertexNumber() == 0);

        // restore face 1
        m.face(1).pushVertex(&m.vertex(0));
        m.face(1).pushVertex(&m.vertex(1));
        m.face(1).pushVertex(&m.vertex(2));
        REQUIRE(m.face(1).edgeSelected(0) == false);
        REQUIRE(m.face(1).edgeSelected(1) == false);
        REQUIRE(m.face(1).edgeSelected(2) == false);

        // force reallocation of vertex container
        m.addVertices(100);
        REQUIRE(m.vertexNumber() == 104);
        REQUIRE(m.vertexContainerSize() == 104);
        REQUIRE(m.face(1).vertexIndex(0) == 0);
        REQUIRE(m.face(1).vertexIndex(1) == 1);
        REQUIRE(m.face(1).vertexIndex(2) == 2);
        REQUIRE(m.face(1).vertex(0) == &m.vertex(0));
        REQUIRE(m.face(1).vertex(1) == &m.vertex(1));
        REQUIRE(m.face(1).vertex(2) == &m.vertex(2));

        m.face(1).setVertex(2, &m.vertex(3));
        m.deleteVertex(2);
        REQUIRE(m.vertexNumber() == 103);
        REQUIRE(m.vertexContainerSize() == 104);
        REQUIRE(m.face(1).vertexIndex(0) == 0);
        REQUIRE(m.face(1).vertexIndex(1) == 1);
        REQUIRE(m.face(1).vertexIndex(2) == 3);
        REQUIRE(m.face(1).vertex(0) == &m.vertex(0));
        REQUIRE(m.face(1).vertex(1) == &m.vertex(1));
        REQUIRE(m.face(1).vertex(2) == &m.vertex(3));
        m.compactVertices();
        REQUIRE(m.vertexNumber() == 103);
        REQUIRE(m.vertexContainerSize() == 103);
        REQUIRE(m.face(1).vertexIndex(0) == 0);
        REQUIRE(m.face(1).vertexIndex(1) == 1);
        REQUIRE(m.face(1).vertexIndex(2) == 2);
        REQUIRE(m.face(1).vertex(0) == &m.vertex(0));
        REQUIRE(m.face(1).vertex(1) == &m.vertex(1));
        REQUIRE(m.face(1).vertex(2) == &m.vertex(2));
    }
}
