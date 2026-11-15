// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/algorithms/mesh.h>
#include <vclib/mesh.h>
#include <vclib/meshes.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Presets")
{
    // 1. PointCloud
    {
        using PC = vcl::PointCloud;
        static_assert(vcl::MeshConcept<PC>);
        static_assert(!vcl::HasFaces<PC>);
        static_assert(!vcl::HasEdges<PC>);
        static_assert(
            !vcl::mesh::HasColor<PC>); // Mesh itself has no color in V1

        // Vertex components
        static_assert(vcl::vert::HasColor<PC::Vertex>);
        static_assert(vcl::vert::HasNormal<PC::Vertex>);
        static_assert(vcl::vert::HasQuality<PC::Vertex>);
        static_assert(vcl::vert::HasAdjacentVertices<PC::Vertex>);
        static_assert(vcl::vert::HasTexCoord<PC::Vertex>);
        static_assert(vcl::vert::HasMaterialIndex<PC::Vertex>);
        static_assert(
            !vcl::vert::HasAdjacentFaces<PC::Vertex>); // No faces in PC!
        static_assert(
            !vcl::vert::HasAdjacentEdges<PC::Vertex>); // No edges in PC!
        static_assert(!vcl::vert::HasTangent<PC::Vertex>);
        static_assert(!vcl::vert::HasPrincipalCurvature<PC::Vertex>);

        PC pc;
        REQUIRE(pc.vertexCount() == 0);
    }

    // 2. TriMesh
    {
        using TM = vcl::TriMesh;
        static_assert(vcl::TriangleMeshConcept<TM>);
        static_assert(vcl::HasFaces<TM>);
        static_assert(!vcl::HasEdges<TM>);
        static_assert(vcl::mesh::HasColor<TM>);

        // Vertex components
        static_assert(vcl::vert::HasAdjacentFaces<TM::Vertex>);
        static_assert(vcl::vert::HasAdjacentVertices<TM::Vertex>);
        static_assert(vcl::vert::HasPrincipalCurvature<TM::Vertex>);
        static_assert(vcl::vert::HasTangent<TM::Vertex>);
        static_assert(!vcl::vert::HasAdjacentEdges<TM::Vertex>);

        // Face components
        static_assert(!vcl::face::HasAdjacentEdges<TM::Face>);

        TM tm;
        REQUIRE((tm.vertexCount() == 0 && tm.faceCount() == 0));
    }

    // 3. PolyMesh
    {
        using PM = vcl::PolyMesh;
        static_assert(vcl::PolygonMeshConcept<PM>);
        static_assert(vcl::HasFaces<PM>);
        static_assert(!vcl::HasEdges<PM>);
        static_assert(vcl::mesh::HasColor<PM>);

        // Vertex components
        static_assert(vcl::vert::HasAdjacentFaces<PM::Vertex>);
        static_assert(vcl::vert::HasAdjacentVertices<PM::Vertex>);
        static_assert(!vcl::vert::HasAdjacentEdges<PM::Vertex>);

        // Face components
        static_assert(!vcl::face::HasAdjacentEdges<PM::Face>);

        PM pm;
        REQUIRE((pm.vertexCount() == 0 && pm.faceCount() == 0));
    }

    // 4. EdgeMesh
    {
        using EM = vcl::EdgeMesh;
        static_assert(vcl::EdgeMeshConcept<EM>);
        static_assert(!vcl::HasFaces<EM>);
        static_assert(vcl::HasEdges<EM>);
        static_assert(vcl::mesh::HasColor<EM>);

        // Vertex components
        static_assert(vcl::vert::HasAdjacentEdges<EM::Vertex>);
        static_assert(vcl::vert::HasAdjacentVertices<EM::Vertex>);
        static_assert(!vcl::vert::HasAdjacentFaces<EM::Vertex>);
        static_assert(!vcl::vert::HasTangent<EM::Vertex>);
        static_assert(!vcl::vert::HasPrincipalCurvature<EM::Vertex>);

        // Edge components
        static_assert(vcl::edge::HasAdjacentEdges<EM::Edge>);
        static_assert(vcl::edge::HasOptionalNormal<EM::Edge>);
        static_assert(!vcl::edge::HasAdjacentFaces<EM::Edge>);

        EM em;
        REQUIRE((em.vertexCount() == 0 && em.edgeCount() == 0));
    }

    // 5. TriEdgeMesh
    {
        using TEM = vcl::TriEdgeMesh;
        static_assert(vcl::TriangleMeshConcept<TEM>);
        static_assert(vcl::EdgeMeshConcept<TEM>);
        static_assert(vcl::HasFaces<TEM>);
        static_assert(vcl::HasEdges<TEM>);
        static_assert(vcl::mesh::HasColor<TEM>);

        // Vertex components
        static_assert(vcl::vert::HasAdjacentEdges<TEM::Vertex>);
        static_assert(vcl::vert::HasAdjacentFaces<TEM::Vertex>);
        static_assert(vcl::vert::HasAdjacentVertices<TEM::Vertex>);
        static_assert(vcl::vert::HasPrincipalCurvature<TEM::Vertex>);
        static_assert(vcl::vert::HasTangent<TEM::Vertex>);

        // Face components
        static_assert(vcl::face::HasAdjacentEdges<TEM::Face>);

        // Edge components
        static_assert(vcl::edge::HasAdjacentEdges<TEM::Edge>);
        static_assert(vcl::edge::HasAdjacentFaces<TEM::Edge>);

        TEM tem;
        REQUIRE(
            (tem.vertexCount() == 0 && tem.faceCount() == 0 &&
             tem.edgeCount() == 0));
    }

    // 6. PolyEdgeMesh
    {
        using PEM = vcl::PolyEdgeMesh;
        static_assert(vcl::PolygonMeshConcept<PEM>);
        static_assert(vcl::EdgeMeshConcept<PEM>);
        static_assert(vcl::HasFaces<PEM>);
        static_assert(vcl::HasEdges<PEM>);
        static_assert(vcl::mesh::HasColor<PEM>);

        // Vertex components
        static_assert(vcl::vert::HasAdjacentEdges<PEM::Vertex>);
        static_assert(vcl::vert::HasAdjacentFaces<PEM::Vertex>);
        static_assert(vcl::vert::HasAdjacentVertices<PEM::Vertex>);
        static_assert(vcl::vert::HasPrincipalCurvature<PEM::Vertex>);
        static_assert(vcl::vert::HasTangent<PEM::Vertex>);

        // Face components
        static_assert(vcl::face::HasAdjacentEdges<PEM::Face>);

        // Edge components
        static_assert(vcl::edge::HasAdjacentEdges<PEM::Edge>);
        static_assert(vcl::edge::HasAdjacentFaces<PEM::Edge>);

        PEM pem;
        REQUIRE(
            (pem.vertexCount() == 0 && pem.faceCount() == 0 &&
             pem.edgeCount() == 0));
    }

    // 7. TMPSimpleTriMesh and TMPSimplePolyMesh
    {
        using TTM = vcl::detail::TMPSimpleTriMesh;
        static_assert(vcl::TriangleMeshConcept<TTM>);
        static_assert(vcl::HasFaces<TTM>);
        static_assert(!vcl::HasEdges<TTM>);
        static_assert(vcl::vert::HasPosition<TTM::Vertex>);
        static_assert(vcl::vert::HasBitFlags<TTM::Vertex>);
        static_assert(vcl::face::HasTriangleVertexReferences<TTM::Face>);
        static_assert(vcl::face::HasTriangleBitFlags<TTM::Face>);
        static_assert(vcl::face::HasBitFlags<TTM::Face>);

        TTM ttm;
        REQUIRE((ttm.vertexCount() == 0 && ttm.faceCount() == 0));

        using TPM = vcl::detail::TMPSimplePolyMesh;
        static_assert(vcl::PolygonMeshConcept<TPM>);
        static_assert(vcl::HasFaces<TPM>);
        static_assert(!vcl::HasEdges<TPM>);
        static_assert(vcl::vert::HasPosition<TPM::Vertex>);
        static_assert(vcl::vert::HasBitFlags<TPM::Vertex>);
        static_assert(vcl::face::HasPolygonVertexReferences<TPM::Face>);
        static_assert(vcl::face::HasPolygonBitFlags<TPM::Face>);
        static_assert(vcl::face::HasBitFlags<TPM::Face>);

        TPM tpm;
        REQUIRE((tpm.vertexCount() == 0 && tpm.faceCount() == 0));
    }
}

TEST_CASE("Mesh Type Names")
{
    REQUIRE(vcl::meshTypeName<vcl::PointCloud>() == "PointCloud");
    REQUIRE(vcl::meshTypeName<vcl::PointCloudf>() == "PointCloudf");
    REQUIRE(vcl::meshTypeName<vcl::PointCloud64>() == "PointCloud64");
    REQUIRE(vcl::meshTypeName<vcl::PointCloud64f>() == "PointCloud64f");

    REQUIRE(vcl::meshTypeName<vcl::TriMesh>() == "TriMesh");
    REQUIRE(vcl::meshTypeName<vcl::TriMeshf>() == "TriMeshf");
    REQUIRE(vcl::meshTypeName<vcl::TriMesh64>() == "TriMesh64");
    REQUIRE(vcl::meshTypeName<vcl::TriMesh64f>() == "TriMesh64f");

    REQUIRE(vcl::meshTypeName<vcl::PolyMesh>() == "PolyMesh");
    REQUIRE(vcl::meshTypeName<vcl::PolyMeshf>() == "PolyMeshf");
    REQUIRE(vcl::meshTypeName<vcl::PolyMesh64>() == "PolyMesh64");
    REQUIRE(vcl::meshTypeName<vcl::PolyMesh64f>() == "PolyMesh64f");

    REQUIRE(vcl::meshTypeName<vcl::EdgeMesh>() == "EdgeMesh");
    REQUIRE(vcl::meshTypeName<vcl::EdgeMeshf>() == "EdgeMeshf");
    REQUIRE(vcl::meshTypeName<vcl::EdgeMesh64>() == "EdgeMesh64");
    REQUIRE(vcl::meshTypeName<vcl::EdgeMesh64f>() == "EdgeMesh64f");

    REQUIRE(vcl::meshTypeName<vcl::TriEdgeMesh>() == "TriEdgeMesh");
    REQUIRE(vcl::meshTypeName<vcl::TriEdgeMeshf>() == "TriEdgeMeshf");
    REQUIRE(vcl::meshTypeName<vcl::TriEdgeMesh64>() == "TriEdgeMesh64");
    REQUIRE(vcl::meshTypeName<vcl::TriEdgeMesh64f>() == "TriEdgeMesh64f");
}
