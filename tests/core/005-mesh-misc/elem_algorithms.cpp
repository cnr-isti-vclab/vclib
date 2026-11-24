// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <vector>

TEST_CASE("Elem Algorithms - Ear Cut Concepts")
{
    // FaceConcept checks on various proxy types and value categories
    static_assert(FaceConcept<SimplePolyMesh::Face>);
    static_assert(FaceConcept<SimplePolyMesh::Face&>);
    static_assert(FaceConcept<const SimplePolyMesh::Face&>);
    static_assert(FaceConcept<SimplePolyMesh::Face&&>);

    static_assert(FaceConcept<SimplePolyMesh::ConstFace>);
    static_assert(FaceConcept<SimplePolyMesh::ConstFace&>);
    static_assert(FaceConcept<const SimplePolyMesh::ConstFace&>);
    static_assert(FaceConcept<SimplePolyMesh::ConstFace&&>);

    static_assert(FaceConcept<TriBitMesh::Face>);
    static_assert(FaceConcept<TriBitMesh::ConstFace>);

    // Negative concept test: Vertex and Edge are not Faces
    static_assert(!FaceConcept<SimplePolyMesh::Vertex>);
    static_assert(!FaceConcept<SimplePolyMesh::ConstVertex>);
}

TEST_CASE("Elem Algorithms - Polygon Normal")
{
    const Point3d expectedZ(0.0, 0.0, 1.0);

    const std::vector<Point3d> triangle = {
        Point3d(0.0, 0.0, 0.0),
        Point3d(2.0, 0.0, 0.0),
        Point3d(0.0, 1.0, 0.0),
    };
    REQUIRE(Polygon3d::normal(triangle).isApprox(expectedZ, 1e-12));

    const std::vector<Point3d> quad = {
        Point3d(0.0, 0.0, 0.0),
        Point3d(2.0, 0.0, 0.0),
        Point3d(2.0, 1.0, 1.0),
        Point3d(0.0, 1.0, 1.0),
    };
    const Point3d quadNormal =
        ((quad[2] - quad[0]).cross(quad[3] - quad[1])).normalized();
    REQUIRE(Polygon3d::normal(quad).isApprox(quadNormal, 1e-12));

    const std::vector<Point3d> pentagon = {
        Point3d(0.0, 0.0, 0.0),
        Point3d(2.0, 0.0, 0.0),
        Point3d(2.0, 1.0, 0.0),
        Point3d(1.0, 2.0, 0.0),
        Point3d(0.0, 1.0, 0.0),
    };
    REQUIRE(Polygon3d::normal(pentagon).isApprox(expectedZ, 1e-12));

    REQUIRE(
        Polygon3d::normal(std::vector<Point3d> {})
            .isApprox(Point3d::Zero(), 1e-12));
}

TEST_CASE("Elem Algorithms - Ear Cut Quad")
{
    SimplePolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(1.0f, 1.0f, 0.0f);
    poly.vertex(3).position() = Point3f(0.0f, 1.0f, 0.0f);
    poly.addFace(0, 1, 2, 3);

    // 1. Calling with mutable lvalue proxy
    auto                   f     = poly.face(0);
    std::vector<vcl::uint> tris1 = earCut(f);
    REQUIRE(tris1.size() == 6); // 2 triangles

    // 2. Calling with temporary rvalue proxy
    std::vector<vcl::uint> tris2 = earCut(poly.face(0));
    REQUIRE(tris2 == tris1);

    // 3. Calling with const lvalue proxy
    const auto             cf    = poly.face(0);
    std::vector<vcl::uint> tris3 = earCut(cf);
    REQUIRE(tris3 == tris1);

    // 4. Calling with ConstFace from a const mesh reference
    const auto&            constPoly = poly;
    std::vector<vcl::uint> tris4     = earCut(constPoly.face(0));
    REQUIRE(tris4 == tris1);

    // 5. Calling unqualified via ADL (since Face is in namespace vcl)
    std::vector<vcl::uint> tris5 = earCut(poly.face(0));
    REQUIRE(tris5 == tris1);

    // Verify all generated triangle indices are in the valid range [0, 3]
    for (vcl::uint idx : tris1) {
        REQUIRE(idx < 4);
    }
}

TEST_CASE("Elem Algorithms - Ear Cut Pentagon")
{
    SimplePolyMesh poly;
    poly.addVertices(5);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(2.0f, 2.0f, 0.0f);
    poly.vertex(3).position() = Point3f(1.0f, 3.0f, 0.0f);
    poly.vertex(4).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly.addFace(0, 1, 2, 3, 4);

    std::vector<vcl::uint> tris = earCut(poly.face(0));
    REQUIRE(tris.size() == 9); // 5-gon -> 3 triangles = 9 indices

    for (vcl::uint idx : tris) {
        REQUIRE(idx < 5);
    }
}

TEST_CASE("Elem Algorithms - Ear Cut Concave")
{
    // L-shaped concave polygon with 6 vertices
    SimplePolyMesh poly;
    poly.addVertices(6);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(2.0f, 1.0f, 0.0f);
    poly.vertex(3).position() = Point3f(1.0f, 1.0f, 0.0f);
    poly.vertex(4).position() = Point3f(1.0f, 2.0f, 0.0f);
    poly.vertex(5).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly.addFace(0, 1, 2, 3, 4, 5);

    std::vector<vcl::uint> tris = earCut(poly.face(0));
    REQUIRE(tris.size() == 12); // 6-gon -> 4 triangles = 12 indices

    for (vcl::uint idx : tris) {
        REQUIRE(idx < 6);
    }
}

TEST_CASE("Elem Algorithms - Ear Cut Triangle")
{
    TriBitMesh triMesh;
    triMesh.addVertices(3);
    triMesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    triMesh.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    triMesh.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    triMesh.addFace(0, 1, 2);

    std::vector<vcl::uint> tris = earCut(triMesh.face(0));
    REQUIRE(tris.size() == 3);
    REQUIRE((tris[0] == 1 && tris[1] == 2 && tris[2] == 0));
}

TEST_CASE("Elem Algorithms - Ear Cut3 DPlane")
{
    // Planar quad situated on the YZ plane (x = 5.0)
    SimplePolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(5.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(5.0f, 2.0f, 0.0f);
    poly.vertex(2).position() = Point3f(5.0f, 2.0f, 2.0f);
    poly.vertex(3).position() = Point3f(5.0f, 0.0f, 2.0f);
    poly.addFace(0, 1, 2, 3);

    std::vector<vcl::uint> tris = earCut(poly.face(0));
    REQUIRE(tris.size() == 6); // 2 triangles
    for (vcl::uint idx : tris) {
        REQUIRE(idx < 4);
    }
}

TEST_CASE("Elem Algorithms - Polygon Geometry Triangle")
{
    TriBitMesh triMesh;
    triMesh.addVertices(3);
    triMesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    triMesh.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    triMesh.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    triMesh.addFace(0, 1, 2);
    auto f = triMesh.face(0);

    // Normal: counter-clockwise (0,0,0) -> (1,0,0) -> (0,1,0) has cross-product
    // (0,0,1)
    Point3f n = faceNormal(f);
    REQUIRE(vcl::epsilonEquals(n, Point3f(0.0f, 0.0f, 1.0f)));

    // Barycenter: (0+1+0)/3, (0+0+1)/3, 0
    Point3f bar = faceBarycenter(f);
    REQUIRE(vcl::epsilonEquals(bar, Point3f(1.0f / 3.0f, 1.0f / 3.0f, 0.0f)));

    // Area: 1 * 1 / 2 = 0.5
    float a = faceArea(f);
    REQUIRE(vcl::epsilonEquals(a, 0.5f));

    // Perimeter: 1 + 1 + sqrt(2)
    float p         = facePerimeter(f);
    float expectedP = 2.0f + std::sqrt(2.0f);
    REQUIRE(vcl::epsilonEquals(p, expectedP));

    // Internal angles
    float a0 = faceAngleOnVertexRad(f, 0);
    REQUIRE(vcl::epsilonEquals(a0, float(M_PI / 2.0)));
    float a1 = faceAngleOnVertexRad(f, 1);
    REQUIRE(vcl::epsilonEquals(a1, float(M_PI / 4.0)));
    float a2 = faceAngleOnVertexRad(f, 2);
    REQUIRE(vcl::epsilonEquals(a2, float(M_PI / 4.0)));
}

TEST_CASE("Elem Algorithms - Polygon Geometry Poly3")
{
    // Same triangle geometry as above, but on a dynamic-vertex-count
    // SimplePolyMesh (vertexCount() == 3)
    SimplePolyMesh poly;
    poly.addVertices(3);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    poly.addFace(0, 1, 2);
    auto f = poly.face(0);

    Point3f n = faceNormal(f);
    REQUIRE(vcl::epsilonEquals(n, Point3f(0.0f, 0.0f, 1.0f)));

    Point3f bar = faceBarycenter(f);
    REQUIRE(vcl::epsilonEquals(bar, Point3f(1.0f / 3.0f, 1.0f / 3.0f, 0.0f)));

    float a = faceArea(f);
    REQUIRE(vcl::epsilonEquals(a, 0.5f));

    float p         = facePerimeter(f);
    float expectedP = 2.0f + std::sqrt(2.0f);
    REQUIRE(vcl::epsilonEquals(p, expectedP));

    float a0 = faceAngleOnVertexRad(f, 0);
    REQUIRE(vcl::epsilonEquals(a0, float(M_PI / 2.0)));
    float a1 = faceAngleOnVertexRad(f, 1);
    REQUIRE(vcl::epsilonEquals(a1, float(M_PI / 4.0)));
    float a2 = faceAngleOnVertexRad(f, 2);
    REQUIRE(vcl::epsilonEquals(a2, float(M_PI / 4.0)));
}

TEST_CASE("Elem Algorithms - Polygon Geometry Quad")
{
    // Square of side 3 on XY plane (normal = +Z)
    SimplePolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(3.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(3.0f, 3.0f, 0.0f);
    poly.vertex(3).position() = Point3f(0.0f, 3.0f, 0.0f);
    poly.addFace(0, 1, 2, 3);
    auto f = poly.face(0);

    Point3f n = faceNormal(f);
    REQUIRE(vcl::epsilonEquals(n, Point3f(0.0f, 0.0f, 1.0f)));

    Point3f bar = faceBarycenter(f);
    REQUIRE(vcl::epsilonEquals(bar, Point3f(1.5f, 1.5f, 0.0f)));

    float a = faceArea(f);
    REQUIRE(vcl::epsilonEquals(a, 9.0f));

    float p = facePerimeter(f);
    REQUIRE(vcl::epsilonEquals(p, 12.0f));

    for (uint i = 0; i < 4; ++i) {
        float angle = faceAngleOnVertexRad(f, i);
        REQUIRE(vcl::epsilonEquals(angle, float(M_PI / 2.0)));
    }
}

TEST_CASE("Elem Algorithms - Polygon Geometry Const And ADL")
{
    SimplePolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(2.0f, 2.0f, 0.0f);
    poly.vertex(3).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly.addFace(0, 1, 2, 3);

    // 1. Const proxy
    const auto cf = poly.face(0);
    REQUIRE(vcl::epsilonEquals(faceArea(cf), 4.0f));
    REQUIRE(vcl::epsilonEquals(facePerimeter(cf), 8.0f));

    // 2. Const mesh reference
    const auto& constPoly = poly;
    REQUIRE(vcl::epsilonEquals(faceArea(constPoly.face(0)), 4.0f));

    // 3. Temporary rvalue proxy
    REQUIRE(
        vcl::epsilonEquals(
            faceNormal(poly.face(0)), Point3f(0.0f, 0.0f, 1.0f)));

    // 4. ADL unqualified calls
    REQUIRE(vcl::epsilonEquals(faceArea(poly.face(0)), 4.0f));
    REQUIRE(vcl::epsilonEquals(facePerimeter(poly.face(0)), 8.0f));
    REQUIRE(
        vcl::epsilonEquals(
            faceBarycenter(poly.face(0)), Point3f(1.0f, 1.0f, 0.0f)));
    REQUIRE(
        vcl::epsilonEquals(
            faceNormal(poly.face(0)), Point3f(0.0f, 0.0f, 1.0f)));
    REQUIRE(
        vcl::epsilonEquals(
            faceAngleOnVertexRad(poly.face(0), 0), float(M_PI / 2.0)));
}

TEST_CASE("Elem Algorithms - Bounding Box")
{
    SimplePolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(2.0f, 3.0f, 0.0f);
    poly.vertex(3).position() = Point3f(0.0f, 3.0f, 0.0f);
    poly.addFace(0, 1, 2, 3);

    // 1. Vertex bounding box
    auto vBox = boundingBox(poly.vertex(1));
    REQUIRE(vBox.min() == Point3f(2.0f, 0.0f, 0.0f));
    REQUIRE(vBox.max() == Point3f(2.0f, 0.0f, 0.0f));

    // 2. Face bounding box
    auto fBox = boundingBox(poly.face(0));
    REQUIRE(fBox.min() == Point3f(0.0f, 0.0f, 0.0f));
    REQUIRE(fBox.max() == Point3f(2.0f, 3.0f, 0.0f));

    // 3. Edge bounding box
    MyEdgeMesh edgeMesh;
    edgeMesh.addVertices(2);
    edgeMesh.vertex(0).position() = Point3f(-1.0f, 5.0f, 2.0f);
    edgeMesh.vertex(1).position() = Point3f(4.0f, -2.0f, 1.0f);
    edgeMesh.addEdge(0, 1);

    auto eBox = boundingBox(edgeMesh.edge(0));
    REQUIRE(eBox.min() == Point3f(-1.0f, -2.0f, 1.0f));
    REQUIRE(eBox.max() == Point3f(4.0f, 5.0f, 2.0f));

    // 4. Const proxy and const mesh
    const auto cf = poly.face(0);
    REQUIRE(boundingBox(cf).min() == Point3f(0.0f, 0.0f, 0.0f));
    const auto& cpoly = poly;
    REQUIRE(boundingBox(cpoly.face(0)).max() == Point3f(2.0f, 3.0f, 0.0f));

    // 5. ADL calls
    REQUIRE(boundingBox(poly.face(0)).min() == Point3f(0.0f, 0.0f, 0.0f));
    REQUIRE(boundingBox(edgeMesh.edge(0)).max() == Point3f(4.0f, 5.0f, 2.0f));
}

TEST_CASE("Elem Algorithms - Distance Vertex Point")
{
    SimplePolyMesh poly;
    poly.addVertex(Point3f(1.0f, 2.0f, 3.0f));
    Point3f p(4.0f, 6.0f, 3.0f); // dx=3, dy=4 -> dist=5

    // 1. mutable proxy
    REQUIRE(vcl::epsilonEquals(distance(poly.vertex(0), p), 5.0f));
    REQUIRE(vcl::epsilonEquals(distance(p, poly.vertex(0)), 5.0f));

    // 2. const proxy
    const auto cv = poly.vertex(0);
    REQUIRE(vcl::epsilonEquals(distance(cv, p), 5.0f));
    REQUIRE(vcl::epsilonEquals(distance(p, cv), 5.0f));

    // 3. const mesh ref
    const auto& cpoly = poly;
    REQUIRE(vcl::epsilonEquals(distance(cpoly.vertex(0), p), 5.0f));

    // 4. ADL unqualified
    REQUIRE(vcl::epsilonEquals(distance(poly.vertex(0), p), 5.0f));
    REQUIRE(vcl::epsilonEquals(distance(p, poly.vertex(0)), 5.0f));
}

TEST_CASE("Elem Algorithms - Distance Vertex Vertex")
{
    SimplePolyMesh poly;
    poly.addVertices(2);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(3.0f, 4.0f, 0.0f);

    REQUIRE(vcl::epsilonEquals(distance(poly.vertex(0), poly.vertex(1)), 5.0f));
    REQUIRE(vcl::epsilonEquals(distance(poly.vertex(1), poly.vertex(0)), 5.0f));

    const auto cv0 = poly.vertex(0);
    const auto cv1 = poly.vertex(1);
    REQUIRE(vcl::epsilonEquals(distance(cv0, cv1), 5.0f));

    // ADL
    REQUIRE(vcl::epsilonEquals(distance(poly.vertex(0), poly.vertex(1)), 5.0f));
}

TEST_CASE("Elem Algorithms - Distance Triangle Face")
{
    TriBitMesh triMesh;
    triMesh.addVertices(3);
    triMesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    triMesh.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    triMesh.vertex(2).position() = Point3f(0.0f, 2.0f, 0.0f);
    triMesh.addFace(0, 1, 2);

    auto    f = triMesh.face(0);
    Point3f pAbove(0.5f, 0.5f, 3.0f);

    // 1. Basic distance
    REQUIRE(vcl::epsilonEquals(distance(pAbove, f), 3.0f));
    REQUIRE(vcl::epsilonEquals(distance(f, pAbove), 3.0f));

    // 2. Closest point
    Point3f closest;
    float   d = distance(pAbove, f, closest);
    REQUIRE(vcl::epsilonEquals(d, 3.0f));
    REQUIRE(vcl::epsilonEquals(closest, Point3f(0.5f, 0.5f, 0.0f)));

    // Symmetrical distance with closest point
    Point3f closestSym;
    float   dSym = distance(f, pAbove, closestSym);
    REQUIRE(vcl::epsilonEquals(dSym, 3.0f));
    REQUIRE(vcl::epsilonEquals(closestSym, Point3f(0.5f, 0.5f, 0.0f)));

    // 3. Signed distance
    Point3f pBelow(0.5f, 0.5f, -3.0f);
    float   dSigned = distance(pBelow, f, closest, true);
    REQUIRE(vcl::epsilonEquals(dSigned, -3.0f));
    REQUIRE(vcl::epsilonEquals(closest, Point3f(0.5f, 0.5f, 0.0f)));

    // 4. Point outside triangle
    Point3f pOutside(-1.0f, 0.0f, 0.0f);
    REQUIRE(vcl::epsilonEquals(distance(pOutside, f, closest), 1.0f));
    REQUIRE(vcl::epsilonEquals(closest, Point3f(0.0f, 0.0f, 0.0f)));

    // 5. boundedDistance
    REQUIRE(boundedDistance(pAbove, f, 1.0f) >= 1.0f);
    REQUIRE(
        vcl::epsilonEquals(boundedDistance(pAbove, f, 5.0f, closest), 3.0f));
    REQUIRE(
        vcl::epsilonEquals(boundedDistance(f, pAbove, 5.0f, closest), 3.0f));

    // 6. Const proxy & ADL
    const auto cf = triMesh.face(0);
    REQUIRE(vcl::epsilonEquals(distance(pAbove, cf), 3.0f));
    REQUIRE(vcl::epsilonEquals(distance(cf, pAbove), 3.0f));
}

TEST_CASE("Elem Algorithms - Distance Polygon Face")
{
    SimplePolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(2.0f, 2.0f, 0.0f);
    poly.vertex(3).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly.addFace(0, 1, 2, 3);

    auto    f = poly.face(0);
    Point3f p(1.0f, 1.0f, 4.0f);

    // 1. Point above planar polygon face
    REQUIRE(vcl::epsilonEquals(distance(p, f), 4.0f));
    REQUIRE(vcl::epsilonEquals(distance(f, p), 4.0f));

    Point3f closest;
    float   d = distance(p, f, closest);
    REQUIRE(vcl::epsilonEquals(d, 4.0f));
    REQUIRE(vcl::epsilonEquals(closest, Point3f(1.0f, 1.0f, 0.0f)));

    // 2. Point outside polygon face
    Point3f pOutside(3.0f, 1.0f, 0.0f);
    float   dOut = distance(pOutside, f, closest);
    REQUIRE(vcl::epsilonEquals(dOut, 1.0f));
    REQUIRE(vcl::epsilonEquals(closest, Point3f(2.0f, 1.0f, 0.0f)));

    // 3. Const mesh and ADL
    const auto& cpoly = poly;
    REQUIRE(vcl::epsilonEquals(distance(p, cpoly.face(0)), 4.0f));
    REQUIRE(vcl::epsilonEquals(distance(cpoly.face(0), p), 4.0f));
}

TEST_CASE("Elem Algorithms - Distance Vertex Face")
{
    SimplePolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(2.0f, 2.0f, 0.0f);
    poly.vertex(3).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly.addFace(0, 1, 2, 3);

    SimplePolyMesh queryMesh;
    queryMesh.addVertex(Point3f(1.0f, 1.0f, 2.5f));

    auto v = queryMesh.vertex(0);
    auto f = poly.face(0);

    REQUIRE(vcl::epsilonEquals(distance(v, f), 2.5f));
    REQUIRE(vcl::epsilonEquals(distance(f, v), 2.5f));

    const auto cv = queryMesh.vertex(0);
    const auto cf = poly.face(0);
    REQUIRE(vcl::epsilonEquals(distance(cv, cf), 2.5f));
    REQUIRE(vcl::epsilonEquals(distance(cf, cv), 2.5f));

    // ADL
    REQUIRE(vcl::epsilonEquals(distance(v, f), 2.5f));
    REQUIRE(vcl::epsilonEquals(distance(f, v), 2.5f));
}

TEST_CASE("Elem Algorithms - Intersection Face Box")
{
    // 1. Triangle Face
    TriBitMesh triMesh;
    triMesh.addVertices(3);
    triMesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    triMesh.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    triMesh.vertex(2).position() = Point3f(0.0f, 2.0f, 0.0f);
    triMesh.addFace(0, 1, 2);

    Box3f boxHit(Point3f(-0.5f, -0.5f, -0.5f), Point3f(1.5f, 1.5f, 0.5f));
    Box3f boxMiss(Point3f(10.0f, 10.0f, 10.0f), Point3f(12.0f, 12.0f, 12.0f));

    REQUIRE(intersect(triMesh.face(0), boxHit));
    REQUIRE(intersect(boxHit, triMesh.face(0))); // symmetry

    REQUIRE(!intersect(triMesh.face(0), boxMiss));
    REQUIRE(!intersect(boxMiss, triMesh.face(0)));

    // 2. Polygonal Quad Face
    SimplePolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(2.0f, 2.0f, 0.0f);
    poly.vertex(3).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly.addFace(0, 1, 2, 3);

    Box3f quadBoxHit(Point3f(1.0f, 1.0f, -1.0f), Point3f(3.0f, 3.0f, 1.0f));
    Box3f quadBoxMiss(Point3f(5.0f, 5.0f, 0.0f), Point3f(6.0f, 6.0f, 1.0f));

    REQUIRE(intersect(poly.face(0), quadBoxHit));
    REQUIRE(intersect(quadBoxHit, poly.face(0)));

    REQUIRE(!intersect(poly.face(0), quadBoxMiss));
    REQUIRE(!intersect(quadBoxMiss, poly.face(0)));

    // 3. 3-vertex polygon face
    SimplePolyMesh poly3;
    poly3.addVertices(3);
    poly3.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly3.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly3.vertex(2).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly3.addFace(0, 1, 2);

    REQUIRE(intersect(poly3.face(0), boxHit));
    REQUIRE(!intersect(poly3.face(0), boxMiss));

    // 4. Const proxy and ADL
    const auto cf = poly.face(0);
    REQUIRE(intersect(cf, quadBoxHit));
    REQUIRE(intersect(quadBoxHit, cf));
}

TEST_CASE("Elem Algorithms - Intersection Face Sphere")
{
    // 1. Triangle Face
    TriBitMesh triMesh;
    triMesh.addVertices(3);
    triMesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    triMesh.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    triMesh.vertex(2).position() = Point3f(0.0f, 2.0f, 0.0f);
    triMesh.addFace(0, 1, 2);

    Sphere<float> sphereHit(Point3f(0.5f, 0.5f, 0.5f), 1.0f);
    Sphere<float> sphereMiss(Point3f(0.5f, 0.5f, 5.0f), 1.0f);

    REQUIRE(intersect(triMesh.face(0), sphereHit));
    REQUIRE(intersect(sphereHit, triMesh.face(0)));

    REQUIRE(!intersect(triMesh.face(0), sphereMiss));
    REQUIRE(!intersect(sphereMiss, triMesh.face(0)));

    // Witness and res
    Point3f                 witness;
    std::pair<float, float> res;
    REQUIRE(intersect(triMesh.face(0), sphereHit, witness, res));
    REQUIRE(vcl::epsilonEquals(witness, Point3f(0.5f, 0.5f, 0.0f)));

    Point3f                 witnessSym;
    std::pair<float, float> resSym;
    REQUIRE(intersect(sphereHit, triMesh.face(0), witnessSym, resSym));
    REQUIRE(vcl::epsilonEquals(witnessSym, Point3f(0.5f, 0.5f, 0.0f)));

    // 2. Polygonal Quad Face
    SimplePolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(2.0f, 2.0f, 0.0f);
    poly.vertex(3).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly.addFace(0, 1, 2, 3);

    Sphere<float> quadSphereHit(Point3f(1.0f, 1.0f, 0.5f), 1.0f);
    REQUIRE(intersect(poly.face(0), quadSphereHit));
    REQUIRE(intersect(quadSphereHit, poly.face(0)));

    REQUIRE(!intersect(poly.face(0), sphereMiss));

    // 3. Const proxy and ADL
    const auto cf = poly.face(0);
    REQUIRE(intersect(cf, quadSphereHit));
    REQUIRE(intersect(quadSphereHit, cf));
}

TEST_CASE("Elem Algorithms - Intersection Face Ray")
{
    // 1. Triangle Face
    TriBitMesh triMesh;
    triMesh.addVertices(3);
    triMesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    triMesh.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    triMesh.vertex(2).position() = Point3f(0.0f, 2.0f, 0.0f);
    triMesh.addFace(0, 1, 2);

    Ray3f rayHit(Point3f(0.5f, 0.5f, 5.0f), Point3f(0.0f, 0.0f, -1.0f));
    Ray3f rayAway(Point3f(0.5f, 0.5f, 5.0f), Point3f(0.0f, 0.0f, 1.0f));
    Ray3f rayMiss(Point3f(5.0f, 5.0f, 5.0f), Point3f(0.0f, 0.0f, -1.0f));

    float t  = 0.0f;
    auto  pt = intersection(rayHit, triMesh.face(0), std::ref(t));
    REQUIRE(pt.has_value());
    REQUIRE(vcl::epsilonEquals(*pt, Point3f(0.5f, 0.5f, 0.0f)));
    REQUIRE(vcl::epsilonEquals(t, 5.0f));

    // Symmetrical call
    float tSym  = 0.0f;
    auto  ptSym = intersection(triMesh.face(0), rayHit, std::ref(tSym));
    REQUIRE(ptSym.has_value());
    REQUIRE(vcl::epsilonEquals(*ptSym, Point3f(0.5f, 0.5f, 0.0f)));
    REQUIRE(vcl::epsilonEquals(tSym, 5.0f));

    REQUIRE(intersect(rayHit, triMesh.face(0)));
    REQUIRE(intersect(triMesh.face(0), rayHit));

    REQUIRE(!intersection(rayAway, triMesh.face(0)).has_value());
    REQUIRE(!intersect(rayAway, triMesh.face(0)));

    REQUIRE(!intersection(rayMiss, triMesh.face(0)).has_value());
    REQUIRE(!intersect(rayMiss, triMesh.face(0)));

    // 2. Polygonal Quad Face
    SimplePolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(2.0f, 2.0f, 0.0f);
    poly.vertex(3).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly.addFace(0, 1, 2, 3);

    Ray3f rayQuad(Point3f(1.0f, 1.5f, 3.0f), Point3f(0.0f, 0.0f, -1.0f));
    float tQuad  = 0.0f;
    auto  ptQuad = intersection(rayQuad, poly.face(0), std::ref(tQuad));
    REQUIRE(ptQuad.has_value());
    REQUIRE(vcl::epsilonEquals(*ptQuad, Point3f(1.0f, 1.5f, 0.0f)));
    REQUIRE(vcl::epsilonEquals(tQuad, 3.0f));

    REQUIRE(intersect(rayQuad, poly.face(0)));
    REQUIRE(intersect(poly.face(0), rayQuad));

    REQUIRE(!intersect(rayMiss, poly.face(0)));

    // 3. Const proxy and ADL
    const auto cf = poly.face(0);
    REQUIRE(intersection(rayQuad, cf).has_value());
    REQUIRE(intersection(cf, rayQuad).has_value());
    REQUIRE(intersect(rayQuad, cf));
    REQUIRE(intersect(cf, rayQuad));
}

TEST_CASE("Elem Algorithms - Visibility")
{
    // 1. Triangle Face
    TriBitMesh triMesh;
    triMesh.addVertices(3);
    triMesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    triMesh.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    triMesh.vertex(2).position() = Point3f(0.0f, 2.0f, 0.0f);
    triMesh.addFace(0, 1, 2);

    Point3f pAbove(0.5f, 0.5f, 2.0f);
    Point3f pBelow(0.5f, 0.5f, -2.0f);
    Point3f pOnPlane(0.5f, 0.5f, 0.0f);

    REQUIRE(halfSpaceDeterminant(triMesh.face(0), pAbove) > 0.0f);
    REQUIRE(halfSpaceDeterminant(triMesh.face(0), pBelow) < 0.0f);
    REQUIRE(
        vcl::epsilonEquals(
            halfSpaceDeterminant(triMesh.face(0), pOnPlane), 0.0f));

    REQUIRE(facePointVisibility(triMesh.face(0), pAbove));
    REQUIRE(!facePointVisibility(triMesh.face(0), pBelow));
    REQUIRE(!facePointVisibility(triMesh.face(0), pOnPlane));

    // Symmetrical facePointVisibility
    REQUIRE(facePointVisibility(pAbove, triMesh.face(0)));
    REQUIRE(!facePointVisibility(pBelow, triMesh.face(0)));

    // 2. 3-vertex polygon face
    SimplePolyMesh poly3;
    poly3.addVertices(3);
    poly3.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly3.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly3.vertex(2).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly3.addFace(0, 1, 2);

    REQUIRE(halfSpaceDeterminant(poly3.face(0), pAbove) > 0.0f);
    REQUIRE(halfSpaceDeterminant(poly3.face(0), pBelow) < 0.0f);
    REQUIRE(facePointVisibility(poly3.face(0), pAbove));
    REQUIRE(!facePointVisibility(poly3.face(0), pBelow));

    // 3. Polygonal Quad Face
    SimplePolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(2.0f, 2.0f, 0.0f);
    poly.vertex(3).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly.addFace(0, 1, 2, 3);

    REQUIRE(halfSpaceDeterminant(poly.face(0), pAbove) > 0.0f);
    REQUIRE(halfSpaceDeterminant(poly.face(0), pBelow) < 0.0f);
    REQUIRE(facePointVisibility(poly.face(0), pAbove));
    REQUIRE(!facePointVisibility(poly.face(0), pBelow));

    // 4. Const proxy and ADL
    const auto cf = poly.face(0);
    REQUIRE(halfSpaceDeterminant(cf, pAbove) > 0.0f);
    REQUIRE(facePointVisibility(cf, pAbove));
    REQUIRE(facePointVisibility(pAbove, cf));
}
