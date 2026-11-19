// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_TYPES_H
#define TEST_TYPES_H

#include <vclib/mesh.h>
#include <vclib/space/core.h>

#include <array>
#include <limits>
#include <string>
#include <vector>

using namespace vcl;

// =============================================================================
// Mesh Type Definitions
// =============================================================================

using MyVertexComponents = VertexComponents<
    vert::BitFlags,
    vert::Position3f,
    vert::OptionalNormal3f,
    vert::OptionalColor>;

using MyFaceComponents = FaceComponents<
    face::BitFlags,
    face::TriangleVertexReferences<vcl::uint>,
    face::OptionalColor>;

using MyMesh = Mesh<
    mesh::VertexContainer<MyVertexComponents>,
    mesh::FaceContainer<MyFaceComponents>,
    mesh::Color>;

using TransformedMesh = Mesh<
    mesh::VertexContainer<MyVertexComponents>,
    mesh::FaceContainer<MyFaceComponents>,
    mesh::Color,
    mesh::TransformMatrixf>;

using BoundingBoxMesh = Mesh<
    mesh::VertexContainer<MyVertexComponents>,
    mesh::FaceContainer<MyFaceComponents>,
    mesh::BoundingBox3f>;

using TransformedBoundingBoxMesh = Mesh<
    mesh::VertexContainer<MyVertexComponents>,
    mesh::FaceContainer<MyFaceComponents>,
    mesh::BoundingBox3f,
    mesh::TransformMatrixf>;

using NamedMesh = Mesh<
    mesh::VertexContainer<MyVertexComponents>,
    mesh::FaceContainer<MyFaceComponents>,
    mesh::Name>;

using CurvatureVertexComponents = VertexComponents<
    vert::BitFlags,
    vert::Position3f,
    vert::PrincipalCurvaturef>;

using OptionalCurvatureVertexComponents = VertexComponents<
    vert::BitFlags,
    vert::Position3f,
    vert::OptionalPrincipalCurvatured>;

using CurvatureFaceComponents = FaceComponents<
    face::BitFlags,
    face::TriangleVertexReferences<vcl::uint>,
    comp::PrincipalCurvaturef<true>>;

using CurvatureMesh = Mesh<
    mesh::VertexContainer<CurvatureVertexComponents>,
    mesh::FaceContainer<CurvatureFaceComponents>,
    mesh::Name>;

using OptionalCurvatureMesh = Mesh<
    mesh::VertexContainer<OptionalCurvatureVertexComponents>,
    mesh::FaceContainer<CurvatureFaceComponents>,
    mesh::Name>;

using QualityVertexComponents =
    VertexComponents<vert::BitFlags, vert::Position3f, vert::Qualityf>;

using OptionalQualityVertexComponents =
    VertexComponents<vert::BitFlags, vert::Position3f, vert::OptionalQualityd>;

using QualityFaceComponents = FaceComponents<
    face::BitFlags,
    face::TriangleVertexReferences<vcl::uint>,
    face::OptionalQualityf>;

using QualityMesh = Mesh<
    mesh::VertexContainer<QualityVertexComponents>,
    mesh::FaceContainer<QualityFaceComponents>>;

using OptionalQualityMesh = Mesh<
    mesh::VertexContainer<OptionalQualityVertexComponents>,
    mesh::FaceContainer<QualityFaceComponents>>;

using TangentVertexComponents = VertexComponents<
    vert::BitFlags,
    vert::Position3f,
    vert::Normal3f,
    vert::Tangent3f>;

using OptionalTangentVertexComponents = VertexComponents<
    vert::BitFlags,
    vert::Position3f,
    vert::Normal3d,
    vert::OptionalTangent3d>;

using TangentFaceComponents = FaceComponents<
    face::BitFlags,
    face::TriangleVertexReferences<vcl::uint>,
    face::Normal3f,
    comp::Tangent3<float, true>>;

using TangentMesh = Mesh<
    mesh::VertexContainer<TangentVertexComponents>,
    mesh::FaceContainer<TangentFaceComponents>>;

using OptionalTangentMesh = Mesh<
    mesh::VertexContainer<OptionalTangentVertexComponents>,
    mesh::FaceContainer<TangentFaceComponents>>;

using FullVertexComponents = VertexComponents<
    vert::BitFlags,
    vert::Position3f,
    vert::Normal3f,
    vert::Color>;

using FullFaceComponents = FaceComponents<
    face::BitFlags,
    face::TriangleVertexReferences<vcl::uint>,
    face::Color>;

using FullMesh = Mesh<
    mesh::VertexContainer<FullVertexComponents>,
    mesh::FaceContainer<FullFaceComponents>,
    mesh::Color>;

using SimpleVertexComponents =
    VertexComponents<vert::BitFlags, vert::Position3f>;

using SimpleMesh = Mesh<
    mesh::VertexContainer<SimpleVertexComponents>,
    mesh::FaceContainer<MyFaceComponents>>;

using PolyFaceComponents = FaceComponents<
    face::PolygonBitFlags,
    face::PolygonVertexReferences<vcl::uint>>;

using SimplePolyMesh = Mesh<
    mesh::VertexContainer<SimpleVertexComponents>,
    mesh::FaceContainer<PolyFaceComponents>>;

using QuadFaceComponents =
    FaceComponents<face::BitFlags, face::VertexReferences<4>>;

using SimpleQuadMesh = Mesh<
    mesh::VertexContainer<SimpleVertexComponents>,
    mesh::FaceContainer<QuadFaceComponents>>;

using PointCloudMesh = Mesh<mesh::VertexContainer<SimpleVertexComponents>>;

using TriBitMesh = vcl::Mesh<
    vcl::mesh::VertexContainer<
        vcl::TypeWrapper<vcl::comp::Position3f, vcl::comp::BitFlags>>,
    vcl::mesh::FaceContainer<vcl::TypeWrapper<
        vcl::comp::TriangleBitFlags,
        vcl::comp::VertexReferences<3>>>>;

using MeshDouble = vcl::Mesh<
    vcl::mesh::VertexContainer<
        vcl::TypeWrapper<vcl::comp::Position3d, vcl::comp::BitFlags>>,
    vcl::mesh::FaceContainer<vcl::TypeWrapper<
        vcl::comp::TriangleBitFlags,
        vcl::comp::VertexReferences<3>>>>;

using CCMeshVertexComponents =
    VertexComponents<vert::BitFlags, vert::Position3f, vert::CustomComponents>;

using CCMeshFaceComponents = FaceComponents<
    face::BitFlags,
    face::TriangleVertexReferences<vcl::uint>,
    face::CustomComponents>;

using CCMesh = Mesh<
    mesh::VertexContainer<CCMeshVertexComponents>,
    mesh::FaceContainer<CCMeshFaceComponents>,
    mesh::CustomComponents>;

using VertexComponents64 = VertexComponents<vert::BitFlags, vert::Position3d>;

using FaceComponents64 =
    FaceComponents<face::BitFlags, face::TriangleVertexReferences<uint64_t>>;

using Mesh64 = Mesh<
    mesh::VertexContainer<VertexComponents64, uint64_t>,
    mesh::FaceContainer<FaceComponents64, uint64_t>>;

using MaterialFaceComponents = FaceComponents<
    face::BitFlags,
    face::TriangleVertexReferences<vcl::uint>,
    face::MaterialIndex,
    face::OptionalColor>;

using MaterialMesh = Mesh<
    mesh::VertexContainer<MyVertexComponents>,
    mesh::FaceContainer<MaterialFaceComponents>>;

using OptionalMaterialFaceComponents = FaceComponents<
    face::BitFlags,
    face::TriangleVertexReferences<vcl::uint>,
    face::OptionalMaterialIndex>;

using OptionalMaterialMesh = Mesh<
    mesh::VertexContainer<MyVertexComponents>,
    mesh::FaceContainer<OptionalMaterialFaceComponents>>;

using MaterialMeshWithMaterials = Mesh<
    mesh::VertexContainer<MyVertexComponents>,
    mesh::FaceContainer<MaterialFaceComponents>,
    mesh::Materials>;

using AdjVertexComponents = VertexComponents<
    vert::BitFlags,
    vert::Position3f,
    vert::AdjacentFaces<>,
    vert::AdjacentVertices<>,
    vert::OptionalAdjacentEdges<>>;

using AdjFaceComponents = FaceComponents<
    face::BitFlags,
    face::TriangleVertexReferences<vcl::uint>,
    face::TriangleAdjacentFaces<vcl::uint>,
    face::OptionalTriangleAdjacentEdges<vcl::uint>,
    face::OptionalColor>;

using AdjMesh = Mesh<
    mesh::VertexContainer<AdjVertexComponents>,
    mesh::FaceContainer<AdjFaceComponents>>;

using MyEdgeComponents = EdgeComponents<
    edge::BitFlags,
    edge::VertexReferences<vcl::uint>,
    edge::Color,
    edge::OptionalQualityf>;

using MyEdgeMesh = Mesh<
    mesh::VertexContainer<MyVertexComponents>,
    mesh::EdgeContainer<MyEdgeComponents>>;

using GraphMesh = Mesh<
    mesh::VertexContainer<MyVertexComponents>,
    mesh::FaceContainer<MyFaceComponents>,
    mesh::EdgeContainer<MyEdgeComponents>>;

using CCEdgeComponents = EdgeComponents<
    edge::BitFlags,
    edge::VertexReferences<vcl::uint>,
    edge::CustomComponents>;

using EdgeMeshWithCC = Mesh<
    mesh::VertexContainer<CCMeshVertexComponents>,
    mesh::EdgeContainer<CCEdgeComponents>,
    mesh::CustomComponents>;

// Concept checking templates
template<typename T>
concept CanCallBitFlags = requires (T t) { t.bitFlags(); };

template<typename T>
concept CanCallPositions = requires (T t) { t.positions(); };

template<typename T>
concept CanCallNormals = requires (T t) { t.normals(); };

template<typename T>
concept CanCallVertexIndices = requires (T t) { t.vertexIndices(); };

/**
 * @brief Helper to create a standard sample mesh with 4 vertices and 2 faces
 * forming a quad.
 *
 * Vertex 0: (0, 0, 1), normal (0, 0, 1), color Blue
 * Vertex 1: (1, 0, 0), normal (0, 0, 1), color Green
 * Vertex 2: (0, 1, 0), normal (0, 0, 1), color Yellow
 * Vertex 3: (1, 1, 0), normal (0, 0, 1), color Cyan
 *
 * Face 0: (0, 1, 2), color DarkRed
 * Face 1: (1, 3, 2), color DarkBlue
 *
 * Mesh color: Red
 */
inline MyMesh createSampleQuadMesh()
{
    MyMesh mesh;
    mesh.enableElementComponent<ElemId::VERTEX, CompId::NORMAL>();
    mesh.resizeVertices(4);
    mesh.resizeFaces(2);

    mesh.vertex(0).position() = Point3f(0.0f, 0.0f, 1.0f);
    mesh.vertex(0).normal()   = Point3f(0.0f, 0.0f, 1.0f);

    mesh.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    mesh.vertex(1).normal()   = Point3f(0.0f, 0.0f, 1.0f);

    mesh.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    mesh.vertex(2).normal()   = Point3f(0.0f, 0.0f, 1.0f);

    mesh.vertex(3).position() = Point3f(1.0f, 1.0f, 0.0f);
    mesh.vertex(3).normal()   = Point3f(0.0f, 0.0f, 1.0f);

    mesh.face(0).setVertices(0, 1, 2);
    mesh.face(1).setVertices(mesh.vertex(1), mesh.vertex(3), mesh.vertex(2));

    mesh.color() = Color::Red;

    mesh.enableElementComponent<ElemId::VERTEX, CompId::COLOR>();
    mesh.vertex(0).color() = Color::Blue;
    mesh.vertex(1).color() = Color::Green;
    mesh.vertex(2).color() = Color::Yellow;
    mesh.vertex(3).color() = Color::Cyan;

    mesh.enableElementComponent<ElemId::FACE, CompId::COLOR>();
    mesh.face(0).color() = Color::DarkRed;
    mesh.face(1).color() = Color::DarkBlue;

    return mesh;
}

/**
 * @brief Helper to create a standard sample edge mesh with 4 vertices and 4
 * edges forming a square loop.
 *
 * Vertex 0: (0, 0, 1)
 * Vertex 1: (1, 0, 0)
 * Vertex 2: (0, 1, 0)
 * Vertex 3: (1, 1, 0)
 *
 * Edge 0: (0, 1)
 * Edge 1: (1, 3)
 * Edge 2: (3, 2)
 * Edge 3: (2, 0)
 */
inline MyEdgeMesh createSampleEdgeMesh()
{
    MyEdgeMesh mesh;
    mesh.resizeVertices(4);

    mesh.vertex(0).position() = Point3f(0.0f, 0.0f, 1.0f);
    mesh.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    mesh.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    mesh.vertex(3).position() = Point3f(1.0f, 1.0f, 0.0f);

    mesh.addEdge(0, 1);
    mesh.addEdge(1, 3);
    mesh.addEdge(3, 2);
    mesh.addEdge(2, 0);

    return mesh;
}

inline TransformedMesh createSampleTransformedMesh()
{
    TransformedMesh mesh;
    mesh.enableElementComponent<ElemId::VERTEX, CompId::NORMAL>();
    mesh.resizeVertices(4);
    mesh.resizeFaces(2);

    mesh.vertex(0).position() = Point3f(0.0f, 0.0f, 1.0f);
    mesh.vertex(0).normal()   = Point3f(0.0f, 0.0f, 1.0f);

    mesh.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    mesh.vertex(1).normal()   = Point3f(0.0f, 0.0f, 1.0f);

    mesh.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    mesh.vertex(2).normal()   = Point3f(0.0f, 0.0f, 1.0f);

    mesh.vertex(3).position() = Point3f(1.0f, 1.0f, 0.0f);
    mesh.vertex(3).normal()   = Point3f(0.0f, 0.0f, 1.0f);

    mesh.face(0).setVertices(0, 1, 2);
    mesh.face(1).setVertices(mesh.vertex(1), mesh.vertex(3), mesh.vertex(2));

    mesh.color() = Color::Red;

    mesh.enableElementComponent<ElemId::VERTEX, CompId::COLOR>();
    mesh.vertex(0).color() = Color::Blue;
    mesh.vertex(1).color() = Color::Green;
    mesh.vertex(2).color() = Color::Yellow;
    mesh.vertex(3).color() = Color::Cyan;

    mesh.enableElementComponent<ElemId::FACE, CompId::COLOR>();
    mesh.face(0).color() = Color::DarkRed;
    mesh.face(1).color() = Color::DarkBlue;

    return mesh;
}

inline MaterialMeshWithMaterials createSampleMaterialMesh()
{
    MaterialMeshWithMaterials mesh;
    mesh.resizeVertices(4);
    mesh.resizeFaces(2);

    mesh.vertex(0).position() = Point3f(0.0f, 0.0f, 1.0f);
    mesh.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    mesh.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    mesh.vertex(3).position() = Point3f(1.0f, 1.0f, 0.0f);

    mesh.face(0).setVertices(0, 1, 2);
    mesh.face(1).setVertices(1, 3, 2);

    Material mat0;
    mat0.name() = "Material_0";
    Material mat1;
    mat1.name() = "Material_1";

    mesh.pushMaterial(mat0);
    mesh.pushMaterial(mat1);

    mesh.face(0).materialIndex() = 0;
    mesh.face(1).materialIndex() = 1;

    return mesh;
}

#endif // TEST_TYPES_H
