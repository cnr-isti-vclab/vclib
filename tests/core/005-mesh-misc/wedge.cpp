// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <catch2/catch_test_macros.hpp>

// Wedge mesh types for tests:
using WedgeFaceComponents = FaceComponents<
    face::BitFlags,
    face::TriangleVertexReferences<vcl::uint>,
    face::TriangleWedgeColors,
    face::TriangleWedgeTexCoordsf,
    face::OptionalTriangleWedgeNormals3f>;

using WedgeMesh = Mesh<
    mesh::VertexContainer<SimpleVertexComponents>,
    mesh::FaceContainer<WedgeFaceComponents>>;

using PolyWedgeFaceComponents = FaceComponents<
    face::PolygonBitFlags,
    face::PolygonVertexReferences<vcl::uint>,
    face::PolygonWedgeColors,
    face::PolygonWedgeTexCoordsf>;

using PolyWedgeMesh = Mesh<
    mesh::VertexContainer<SimpleVertexComponents>,
    mesh::FaceContainer<PolyWedgeFaceComponents>>;

TEST_CASE("Wedge - Wedge Concepts")
{
    // Wedge component concepts on Face
    static_assert(face::HasWedgeColors<WedgeMesh::Face>);
    static_assert(!face::HasOptionalWedgeColors<WedgeMesh::Face>);
    static_assert(face::HasWedgeTexCoords<WedgeMesh::Face>);
    static_assert(!face::HasOptionalWedgeTexCoords<WedgeMesh::Face>);
    static_assert(face::HasWedgeNormals<WedgeMesh::Face>);
    static_assert(face::HasOptionalWedgeNormals<WedgeMesh::Face>);
    static_assert(face::HasWedges<WedgeMesh::Face>);

    // Unified element_concepts on Face
    static_assert(HasWedgeColors<WedgeMesh::Face>);
    static_assert(HasWedgeTexCoords<WedgeMesh::Face>);
    static_assert(HasWedgeNormals<WedgeMesh::Face>);
    static_assert(HasWedges<WedgeMesh::Face>);

    // Mesh-level wedge concepts
    static_assert(HasFaceWedgeColors<WedgeMesh>);
    static_assert(HasFaceWedgeTexCoords<WedgeMesh>);
    static_assert(HasFaceWedgeNormals<WedgeMesh>);
    static_assert(HasFaceOptionalWedgeNormals<WedgeMesh>);
    static_assert(HasFaceWedges<WedgeMesh>);

    // Negative concepts on mesh without wedges
    static_assert(!face::HasWedgeColors<SimpleMesh::Face>);
    static_assert(!face::HasWedgeTexCoords<SimpleMesh::Face>);
    static_assert(!face::HasWedgeNormals<SimpleMesh::Face>);
    static_assert(!face::HasWedges<SimpleMesh::Face>);
    static_assert(!HasWedges<SimpleMesh::Face>);
    static_assert(!HasFaceWedges<SimpleMesh>);

    // Polygonal wedge mesh concepts
    static_assert(face::HasWedgeColors<PolyWedgeMesh::Face>);
    static_assert(face::HasWedgeTexCoords<PolyWedgeMesh::Face>);
    static_assert(!face::HasWedgeNormals<PolyWedgeMesh::Face>);
    static_assert(HasFaceWedges<PolyWedgeMesh>);
    static_assert(!HasFaceWedgeNormals<PolyWedgeMesh>);

    // ComponentConcept check
    static_assert(comp::ComponentConcept<face::TriangleWedgeColors>);
    static_assert(comp::ComponentConcept<face::TriangleWedgeTexCoordsf>);
    static_assert(comp::ComponentConcept<face::OptionalTriangleWedgeNormals3f>);
    static_assert(comp::IsTiedToVertexCount<face::PolygonWedgeColors>);
    static_assert(!comp::IsTiedToVertexCount<face::TriangleWedgeColors>);

    // Wedge traits
    static_assert(WedgeConcept<WedgeType<WedgeMesh::Face>>);
    static_assert(std::is_same_v<
                  WedgeType<WedgeMesh::Face>,
                  comp::WedgeProxy<WedgeMesh::Face>>);
    static_assert(std::is_same_v<
                  VertexType<WedgeType<WedgeMesh::Face>>,
                  WedgeMesh::Vertex>);
    static_assert(
        std::is_same_v<FaceType<WedgeType<WedgeMesh::Face>>, WedgeMesh::Face>);
    static_assert(
        std::is_same_v<NormalType<WedgeType<WedgeMesh::Face>>, Point3f>);
    static_assert(std::is_same_v<
                  TexCoordType<WedgeType<WedgeMesh::Face>>,
                  vcl::TexCoordf>);
    static_assert(
        std::is_same_v<ScalarType<WedgeType<WedgeMesh::Face>>, float>);
}

TEST_CASE("Wedge - Triangle Wedges")
{
    WedgeMesh m;
    m.addVertices(3);
    vcl::uint fi = m.addFace(0, 1, 2);
    auto      f  = m.face(fi);

    // 1. Access via WedgeProxy: f.wedge(i)
    f.wedge(0).color() = vcl::Color::Red;
    f.wedge(1).color() = vcl::Color::Green;
    f.wedge(2).color() = vcl::Color::Blue;

    REQUIRE(f.wedge(0).color() == vcl::Color::Red);
    REQUIRE(f.wedge(1).color() == vcl::Color::Green);
    REQUIRE(f.wedge(2).color() == vcl::Color::Blue);
    REQUIRE(f.wedge(0).index() == 0);
    REQUIRE(f.wedge(1).index() == 1);
    REQUIRE(f.wedge(2).index() == 2);
    REQUIRE(f.wedge(0).vertexIndex() == 0);
    REQUIRE(f.wedge(1).vertexIndex() == 1);
    REQUIRE(f.wedge(2).vertexIndex() == 2);

    // Wedge texcoords via proxy
    f.wedge(0).texCoord() = vcl::TexCoordf(0.0f, 0.0f);
    f.wedge(1).texCoord() = vcl::TexCoordf(1.0f, 0.0f);
    f.wedge(2).texCoord() = vcl::TexCoordf(0.5f, 1.0f);

    REQUIRE(
        (f.wedge(0).texCoord().u() == 0.0f &&
         f.wedge(0).texCoord().v() == 0.0f));
    REQUIRE(
        (f.wedge(1).texCoord().u() == 1.0f &&
         f.wedge(1).texCoord().v() == 0.0f));
    REQUIRE(
        (f.wedge(2).texCoord().u() == 0.5f &&
         f.wedge(2).texCoord().v() == 1.0f));

    // Optional WedgeNormals
    REQUIRE(!f.wedge(0).isNormalAvailable());
    REQUIRE(!f.wedges().isNormalsAvailable());

    m.enableElementComponent<ElemId::FACE, CompId::WEDGE_NORMALS>();
    REQUIRE(f.wedge(0).isNormalAvailable());
    REQUIRE(f.wedges().isNormalsAvailable());

    f.wedge(0).normal() = Point3f(0.0f, 0.0f, 1.0f);
    f.wedge(1).normal() = Point3f(0.0f, 1.0f, 0.0f);
    f.wedge(2).normal() = Point3f(1.0f, 0.0f, 0.0f);

    REQUIRE(f.wedge(0).normal() == Point3f(0.0f, 0.0f, 1.0f));
    REQUIRE(f.wedge(1).normal() == Point3f(0.0f, 1.0f, 0.0f));
    REQUIRE(f.wedge(2).normal() == Point3f(1.0f, 0.0f, 0.0f));

    // 2. Access via WedgeContainerProxy: f.wedges()
    REQUIRE(f.wedges().size() == 3);
    REQUIRE(f.wedges().colors().size() == 3);
    REQUIRE(f.wedges().colors()[0] == vcl::Color::Red);
    REQUIRE(f.wedges().colors()[1] == vcl::Color::Green);
    REQUIRE(f.wedges().colors()[2] == vcl::Color::Blue);
    REQUIRE(f.wedges().normals()[0] == Point3f(0.0f, 0.0f, 1.0f));
    REQUIRE(f.wedges().texCoords()[1].u() == 1.0f);
    REQUIRE(f.wedges()[0].color() == vcl::Color::Red);

    // Range-for over WedgeProxy: for (auto w : f.wedges())
    uint count = 0;
    for (auto w : f.wedges()) {
        REQUIRE(w.index() == count);
        REQUIRE(w.vertexIndex() == count);
        ++count;
    }
    REQUIRE(count == 3);

    // Range-for over direct column vectors: for (auto& n :
    // f.wedges().normals())
    for (auto& n : f.wedges().normals()) {
        REQUIRE(n.norm() > 0.0f);
    }

    // Const-correctness
    const auto cf = f;
    REQUIRE(cf.wedge(0).color() == vcl::Color::Red);
    REQUIRE(cf.wedges().colors().size() == 3);
    for (const auto& c : cf.wedges().colors()) {
        REQUIRE((c.red() > 0 || c.green() > 0 || c.blue() > 0));
    }

    // Test importFrom between faces
    WedgeMesh m2;
    m2.enableElementComponent<ElemId::FACE, CompId::WEDGE_NORMALS>();
    m2.addVertices(3);
    vcl::uint fi2 = m2.addFace(0, 1, 2);
    auto      f2  = m2.face(fi2);

    f2.importFrom(f);
    REQUIRE(f2.wedge(0).color() == vcl::Color::Red);
    REQUIRE(f2.wedge(1).color() == vcl::Color::Green);
    REQUIRE(f2.wedge(2).color() == vcl::Color::Blue);
    REQUIRE(f2.wedge(1).texCoord().u() == 1.0f);
    REQUIRE(f2.wedge(0).normal() == Point3f(0.0f, 0.0f, 1.0f));
}

TEST_CASE("Wedge - Polygon Wedges")
{
    PolyWedgeMesh pm;
    pm.addVertices(6);
    vcl::uint pfi = pm.addFace(0, 1, 2);
    auto      pf  = pm.face(pfi);

    // Initial 3 vertices
    REQUIRE(pf.vertexCount() == 3);
    REQUIRE(pf.wedges().size() == 3);
    REQUIRE(pf.wedges().colors().size() == 3);
    REQUIRE(pf.wedges().texCoords().size() == 3);

    pf.wedge(0).color() = vcl::Color::Red;
    pf.wedge(1).color() = vcl::Color::Green;
    pf.wedge(2).color() = vcl::Color::Blue;

    pf.wedge(0).texCoord() = vcl::TexCoordf(0.0f, 0.0f);
    pf.wedge(1).texCoord() = vcl::TexCoordf(1.0f, 0.0f);
    pf.wedge(2).texCoord() = vcl::TexCoordf(0.5f, 1.0f);

    // Test pushVertex: dynamic polygon expansion resizes wedges container
    pf.pushVertex(3);
    REQUIRE(pf.vertexCount() == 4);
    REQUIRE(pf.wedges().size() == 4);
    REQUIRE(pf.wedges().colors().size() == 4);
    REQUIRE(pf.wedges().texCoords().size() == 4);
    REQUIRE(pf.wedge(0).color() == vcl::Color::Red);
    REQUIRE(pf.wedge(2).color() == vcl::Color::Blue);

    pf.wedge(3).color() = vcl::Color::Yellow;
    REQUIRE(pf.wedge(3).color() == vcl::Color::Yellow);

    // Test insertVertex
    pf.insertVertex(1, 4);
    REQUIRE(pf.vertexCount() == 5);
    REQUIRE(pf.wedges().size() == 5);
    REQUIRE(pf.wedges().colors().size() == 5);
    REQUIRE(pf.wedge(0).color() == vcl::Color::Red);
    REQUIRE(pf.wedge(2).color() == vcl::Color::Green);
    REQUIRE(pf.wedge(3).color() == vcl::Color::Blue);

    // Test eraseVertex
    pf.eraseVertex(1);
    REQUIRE(pf.vertexCount() == 4);
    REQUIRE(pf.wedges().size() == 4);
    REQUIRE(pf.wedges().colors().size() == 4);
    REQUIRE(pf.wedge(0).color() == vcl::Color::Red);
    REQUIRE(pf.wedge(1).color() == vcl::Color::Green);
    REQUIRE(pf.wedge(2).color() == vcl::Color::Blue);

    // Test clearVertices
    pf.clearVertices();
    REQUIRE(pf.vertexCount() == 0);
    REQUIRE(pf.wedges().empty());
    REQUIRE(pf.wedges().colors().empty());
    REQUIRE(pf.wedges().texCoords().empty());
}

TEST_CASE("Wedge - Poly To Tri Wedges")
{
    using PolyWedgeNormalsFaceComponents = FaceComponents<
        face::PolygonBitFlags,
        face::PolygonVertexReferences<vcl::uint>,
        face::PolygonWedgeColors,
        face::PolygonWedgeTexCoordsd,
        face::PolygonWedgeNormals3d>;

    using PolyWedgeNormalsMesh = Mesh<
        mesh::VertexContainer<SimpleVertexComponents>,
        mesh::FaceContainer<PolyWedgeNormalsFaceComponents>>;

    PolyWedgeNormalsMesh pm;
    pm.addVertices(4);
    pm.vertex(0).position() = Point3f(0, 0, 0);
    pm.vertex(1).position() = Point3f(1, 0, 0);
    pm.vertex(2).position() = Point3f(1, 1, 0);
    pm.vertex(3).position() = Point3f(0, 1, 0);

    pm.addFace(0, 1, 2, 3);
    auto pf             = pm.face(0);
    pf.wedge(0).color() = Color::Red;
    pf.wedge(1).color() = Color::Green;
    pf.wedge(2).color() = Color::Blue;
    pf.wedge(3).color() = Color::Yellow;

    pf.wedge(0).texCoord() = TexCoordd(0.0, 0.0);
    pf.wedge(1).texCoord() = TexCoordd(1.0, 0.0);
    pf.wedge(2).texCoord() = TexCoordd(1.0, 1.0);
    pf.wedge(3).texCoord() = TexCoordd(0.0, 1.0);

    pf.wedge(0).normal() = Point3d(0, 0, 1);
    pf.wedge(1).normal() = Point3d(0, 0, 1);
    pf.wedge(2).normal() = Point3d(0, 0, 1);
    pf.wedge(3).normal() = Point3d(0, 0, 1);

    WedgeMesh tm;
    tm.enableElementComponent<ElemId::FACE, CompId::WEDGE_NORMALS>();
    tm.importFrom(pm);

    REQUIRE(tm.vertexCount() == 4);
    REQUIRE(tm.faceCount() == 2);

    for (uint fi = 0; fi < 2; ++fi) {
        auto tf = tm.face(fi);
        for (uint j = 0; j < 3; ++j) {
            uint origCorner = tf.vertexIndex(j);
            REQUIRE(tf.wedge(j).color() == pf.wedge(origCorner).color());
            REQUIRE(
                tf.wedge(j).texCoord().u() ==
                static_cast<float>(pf.wedge(origCorner).texCoord().u()));
            REQUIRE(
                tf.wedge(j).texCoord().v() ==
                static_cast<float>(pf.wedge(origCorner).texCoord().v()));
            REQUIRE(
                tf.wedge(j).normal() ==
                pf.wedge(origCorner).normal().cast<float>());
        }
    }
}

TEST_CASE("Wedge - Const Correctness and Conversions")
{
    WedgeMesh wm;
    wm.addVertices(3);
    uint fi               = wm.addFace(0, 1, 2);
    auto f                = wm.face(fi);
    f.wedge(0).color()    = Color::Red;
    f.wedge(0).texCoord() = TexCoordf(0.5f, 0.5f);

    // Mutation via wedges().front()
    f.wedges().front().color() = Color::Green;
    REQUIRE(f.wedge(0).color() == Color::Green);
    f.wedges().front().color() = Color::Red;

    const WedgeMesh& cwm = wm;
    const auto&      cf  = cwm.face(0);

    // Access through const Face returns read-only references
    static_assert(std::is_same_v<decltype(cf.wedge(0).color()), const Color&>);
    static_assert(
        std::is_same_v<decltype(cf.wedge(0).texCoord()), const TexCoordf&>);
    static_assert(
        std::is_same_v<decltype(cf.wedge(0).vertex()), WedgeMesh::ConstVertex>);
    REQUIRE(cf.wedge(0).color() == Color::Red);

    // Access through const Face& reference to mutable face
    const auto& constFaceRef = f;
    static_assert(
        std::is_same_v<decltype(constFaceRef.wedge(0).color()), const Color&>);

    // WedgeProxy conversion mutable -> const
    WedgeMesh::ConstFace                   cFace      = cf;
    comp::WedgeProxy<WedgeMesh::Face>      mutWedge   = f.wedge(0);
    comp::WedgeProxy<WedgeMesh::ConstFace> constWedge = mutWedge;
    REQUIRE(constWedge == mutWedge);
    REQUIRE(constWedge.index() == 0);

    // WedgeContainerProxy conversion mutable -> const
    comp::WedgeContainerProxy<WedgeMesh::Face>      mutWedges   = f.wedges();
    comp::WedgeContainerProxy<WedgeMesh::ConstFace> constWedges = mutWedges;
    REQUIRE(constWedges.size() == 3);

    // WedgeIterator conversion mutable -> const and difference
    comp::WedgeIterator<WedgeMesh::Face>      mutIt   = mutWedges.begin();
    comp::WedgeIterator<WedgeMesh::ConstFace> constIt = mutIt;
    REQUIRE(mutIt == constIt);
    REQUIRE((constWedges.end() - mutIt) == 3);
    REQUIRE((mutWedges.end() - constIt) == 3);
}
