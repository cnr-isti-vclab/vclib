// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <vclib/space/complex.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Mesh Info - Mesh Info Default And Manual")
{
    MeshInfo info;
    REQUIRE(info.isEmpty());
    REQUIRE(info.isUnkownMesh());
    REQUIRE(!info.hasVertices());
    REQUIRE(!info.hasFaces());
    REQUIRE(!info.hasEdges());
    REQUIRE(!info.hasMaterials());

    // Manual setters
    info.setVertices(true);
    info.setVertexPosition(true, PrimitiveType::FLOAT);
    info.setVertexNormal(true, PrimitiveType::FLOAT);
    info.setVertexColor(true, PrimitiveType::UCHAR);
    info.setFaces(true);
    info.setFaceVertexReferences(true);
    info.setFaceNormal(true, PrimitiveType::DOUBLE);
    info.setTriangleMesh();

    REQUIRE(!info.isEmpty());
    REQUIRE(info.isTriangleMesh());
    REQUIRE(!info.isQuadMesh());
    REQUIRE(!info.isPolygonMesh());
    REQUIRE(info.hasVertices());
    REQUIRE(info.hasVertexPosition());
    REQUIRE(info.vertexPositionType() == PrimitiveType::FLOAT);
    REQUIRE(info.hasVertexNormal());
    REQUIRE(info.vertexNormalType() == PrimitiveType::FLOAT);
    REQUIRE(info.hasVertexColor());
    REQUIRE(info.vertexColorType() == PrimitiveType::UCHAR);
    REQUIRE(info.hasFaces());
    REQUIRE(info.hasFaceVertexReferences());
    REQUIRE(info.hasFaceNormal());
    REQUIRE(info.faceNormalType() == PrimitiveType::DOUBLE);
    REQUIRE(!info.hasEdges());

    // Update mesh type
    MeshInfo polyInfo;
    polyInfo.updateMeshType(5);
    REQUIRE(polyInfo.isPolygonMesh());

    MeshInfo quadInfo;
    quadInfo.updateMeshType(4);
    REQUIRE(quadInfo.isQuadMesh());

    MeshInfo triInfo;
    triInfo.updateMeshType(3);
    REQUIRE(triInfo.isTriangleMesh());

    // Clear
    info.clear();
    REQUIRE(info.isEmpty());
    REQUIRE(info.isUnkownMesh());
    REQUIRE(!info.hasVertices());
}

TEST_CASE("Mesh Info - Mesh Info From Mesh")
{
    // 1. MyMesh with enabled optional normal and color
    {
        MyMesh   mesh = createSampleQuadMesh();
        MeshInfo info(mesh);

        REQUIRE(info.hasVertices());
        REQUIRE(info.hasVertexPosition());
        REQUIRE(info.vertexPositionType() == PrimitiveType::FLOAT);
        REQUIRE(info.hasVertexBitFlags());
        REQUIRE(info.hasVertexNormal());
        REQUIRE(info.vertexNormalType() == PrimitiveType::FLOAT);
        REQUIRE(info.hasVertexColor());
        REQUIRE(info.vertexColorType() == PrimitiveType::UCHAR);

        REQUIRE(info.hasFaces());
        REQUIRE(info.hasFaceVertexReferences());
        REQUIRE(info.hasFaceBitFlags());
        REQUIRE(info.hasFaceColor());
        REQUIRE(info.faceColorType() == PrimitiveType::UCHAR);
        REQUIRE(info.isTriangleMesh());

        REQUIRE(!info.hasEdges());
        REQUIRE(!info.hasMaterials());
    }

    // 2. MyMesh with disabled optional components
    {
        MyMesh   emptyMesh;
        MeshInfo info(emptyMesh);

        REQUIRE(info.hasVertices());
        REQUIRE(info.hasVertexPosition());
        REQUIRE(!info.hasVertexNormal());
        REQUIRE(!info.hasVertexColor());

        REQUIRE(info.hasFaces());
        REQUIRE(!info.hasFaceColor());
        REQUIRE(info.isTriangleMesh());
    }

    // 3. MeshDouble (scalar type is double)
    {
        MeshDouble dm;
        MeshInfo   info(dm);

        REQUIRE(info.hasVertices());
        REQUIRE(info.hasVertexPosition());
        REQUIRE(info.vertexPositionType() == PrimitiveType::DOUBLE);
        REQUIRE(info.hasFaces());
        REQUIRE(info.isTriangleMesh());
    }

    // 4. QuadMesh and PolyMesh
    {
        SimpleQuadMesh qm;
        MeshInfo       qInfo(qm);
        REQUIRE(qInfo.hasFaces());
        REQUIRE(qInfo.isQuadMesh());

        SimplePolyMesh pm;
        MeshInfo       pInfo(pm);
        REQUIRE(pInfo.hasFaces());
        REQUIRE(pInfo.isPolygonMesh());
    }

    // 5. EdgeMesh
    {
        MyEdgeMesh em = createSampleEdgeMesh();
        MeshInfo   info(em);

        REQUIRE(info.hasVertices());
        REQUIRE(info.hasVertexPosition());
        REQUIRE(info.hasEdges());
        REQUIRE(info.hasEdgeVertexReferences());
        REQUIRE(info.hasEdgeBitFlags());
        REQUIRE(!info.hasFaces());
    }

    // 6. MaterialMeshWithMaterials
    {
        MaterialMeshWithMaterials mm = createSampleMaterialMesh();
        MeshInfo                  info(mm);

        REQUIRE(info.hasMaterials());
        REQUIRE(info.hasFaces());
        REQUIRE(info.hasFaceMaterialIndex());
        REQUIRE(info.faceMaterialIndexType() == PrimitiveType::USHORT);
    }

    // 7. CCMesh (Custom Components)
    {
        CCMesh ccm;
        ccm.addVertexCustomComponent<double>("curvature");
        ccm.addFaceCustomComponent<int>("cluster_id");

        MeshInfo info(ccm);

        REQUIRE(info.hasVertexCustomComponents());
        REQUIRE(info.vertexCustomComponents().size() == 1);
        REQUIRE(info.vertexCustomComponents()[0].name == "curvature");
        REQUIRE(info.vertexCustomComponents()[0].type == PrimitiveType::DOUBLE);

        REQUIRE(info.hasFaceCustomComponents());
        REQUIRE(info.faceCustomComponents().size() == 1);
        REQUIRE(info.faceCustomComponents()[0].name == "cluster_id");
        REQUIRE(info.faceCustomComponents()[0].type == PrimitiveType::INT);
    }
}

TEST_CASE("Mesh Info - Mesh Info Intersect")
{
    MeshInfo info1;
    info1.setVertices(true);
    info1.setVertexPosition(true, PrimitiveType::FLOAT);
    info1.setVertexNormal(true, PrimitiveType::FLOAT);
    info1.setFaces(true);
    info1.setFaceColor(true, PrimitiveType::UCHAR);
    info1.setTriangleMesh();

    MeshInfo info2;
    info2.setVertices(true);
    info2.setVertexPosition(true, PrimitiveType::DOUBLE);
    info2.setVertexColor(true, PrimitiveType::UCHAR);
    info2.setFaces(true);
    info2.setFaceColor(true, PrimitiveType::UCHAR);
    info2.setTriangleMesh();

    MeshInfo inter = info1.intersect(info2);

    REQUIRE(inter.hasVertices());
    REQUIRE(inter.hasVertexPosition());
    REQUIRE(
        inter.vertexPositionType() == PrimitiveType::FLOAT); // Taken from info1
    REQUIRE(!inter.hasVertexNormal());                       // Not in info2
    REQUIRE(!inter.hasVertexColor());                        // Not in info1
    REQUIRE(inter.hasFaces());
    REQUIRE(inter.hasFaceColor());
    REQUIRE(inter.isTriangleMesh());
}

TEST_CASE("Mesh Info - Mesh Info Enable Optional Components")
{
    // 1. Enable optional components on MyMesh
    {
        MyMesh mesh;
        REQUIRE(!mesh.isVertexNormalAvailable());
        REQUIRE(!mesh.isVertexColorAvailable());
        REQUIRE(!mesh.isFaceColorAvailable());

        MeshInfo reqInfo;
        reqInfo.setVertices(true);
        reqInfo.setVertexNormal(true);
        reqInfo.setVertexColor(true);
        reqInfo.setFaces(true);
        reqInfo.setFaceColor(true);

        enableOptionalComponentsFromInfo(reqInfo, mesh);

        REQUIRE(mesh.isVertexNormalAvailable());
        REQUIRE(mesh.isVertexColorAvailable());
        REQUIRE(mesh.isFaceColorAvailable());
        REQUIRE(reqInfo.hasVertexNormal());
        REQUIRE(reqInfo.hasVertexColor());
        REQUIRE(reqInfo.hasFaceColor());

        // Component not supported by MyMesh (e.g. vertex tangent)
        MeshInfo tangentInfo;
        tangentInfo.setVertices(true);
        tangentInfo.setVertexTangent(true);
        enableOptionalComponentsFromInfo(tangentInfo, mesh);
        // info should have tangent reset to false since mesh cannot support it
        REQUIRE(!tangentInfo.hasVertexTangent());
    }

    // 2. Custom components enablement on CCMesh
    {
        CCMesh ccm;
        REQUIRE(!ccm.hasVertexCustomComponent("weight"));
        REQUIRE(!ccm.hasFaceCustomComponent("label"));

        MeshInfo ccInfo;
        ccInfo.setVertices(true);
        ccInfo.addVertexCustomComponent("weight", PrimitiveType::FLOAT);
        ccInfo.setFaces(true);
        ccInfo.addFaceCustomComponent("label", PrimitiveType::INT);

        enableOptionalComponentsFromInfo(ccInfo, ccm);

        REQUIRE(ccm.hasVertexCustomComponent("weight"));
        REQUIRE(ccm.isVertexCustomComponentOfType<float>("weight"));
        REQUIRE(ccm.hasFaceCustomComponent("label"));
        REQUIRE(ccm.isFaceCustomComponentOfType<int>("label"));
    }
}
