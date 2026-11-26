// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <vclib/algorithms/mesh.h>
#include <vclib/meshes.h>
#include <vclib/space/complex.h>

#include <catch2/catch_test_macros.hpp>

#include <set>
#include <type_traits>

using MeshPosTestMesh = vcl::TriMesh;

// Hexagonal fan: vertex 0 is the center (interior vertex), vertices 1..6 are
// on the border. Face i is (0, i+1, i+2), with the last face closing the fan.
inline MeshPosTestMesh createHexagonFan()
{
    MeshPosTestMesh m;
    m.enableFaceAdjacentFaces();
    m.resizeVertices(7);
    m.vertex(0).position() = Point3d(0.0, 0.0, 0.0);
    for (uint i = 0; i < 6; ++i) {
        double a                   = i * 3.14159265358979 / 3.0f;
        m.vertex(i + 1).position() = Point3d(std::cos(a), std::sin(a), 0.0);
    }

    m.resizeFaces(6);
    for (uint i = 0; i < 6; ++i) {
        m.face(i).setVertices(0, i + 1, (i + 1) % 6 + 1);
    }

    vcl::updateFaceAdjacentFaces(m);
    return m;
}

inline void testMeshPosBasics(MeshPosTestMesh& m)
{
    using Pos = vcl::MeshPos<MeshPosTestMesh>;

    // null MeshPos
    Pos nullPos;
    REQUIRE(nullPos.isNull());
    REQUIRE(!nullPos.isValid());

    // from a (mutable) face and an edge
    Pos pos(m.face(0), 0);
    REQUIRE((!pos.isNull() && pos.isValid()));
    REQUIRE(pos.face().index() == 0);
    REQUIRE(pos.vertex().index() == 0);
    REQUIRE(pos.edge() == 0);

    // from a face and a vertex
    Pos posFV(m.face(0), m.vertex(2));
    REQUIRE(posFV.edge() == 2);
    REQUIRE(posFV.vertex().index() == 2);

    // from a const mesh
    const MeshPosTestMesh& cm = m;
    Pos                    posC(cm.face(0), cm.vertex(0), 0);
    REQUIRE(posC == pos);

    // the MeshPos gives access only to const elements, also if it has been
    // built from mutable ones
    static_assert(
        std::is_same_v<decltype(pos.face()), MeshPosTestMesh::ConstFace>,
        "MeshPos must expose ConstFace");
    static_assert(
        std::is_same_v<decltype(pos.vertex()), MeshPosTestMesh::ConstVertex>,
        "MeshPos must expose ConstVertex");
    static_assert(
        std::is_const_v<
            std::remove_reference_t<decltype(pos.face().parentContainer())>>);

    // static validity check
    REQUIRE(Pos::isValid(cm.face(0), cm.vertex(1), 0));
    REQUIRE(Pos::isValid(cm.face(0), cm.vertex(0), 0));
    REQUIRE(!Pos::isValid(cm.face(0), cm.vertex(5), 0));
    REQUIRE(!Pos::isValid(cm.face(0), cm.vertex(0), 3));
    REQUIRE(!Pos::isValid(cm.face(0), cm.vertex(0), -1));
}

inline void testMeshPosMoves(const MeshPosTestMesh& m)
{
    using Pos = vcl::MeshPos<MeshPosTestMesh>;

    // face 0 = (0, 1, 2), edge 0 = (0, 1), vertex 0
    Pos pos(m.face(0), m.vertex(0), 0);

    // adjacent elements do not change the MeshPos
    REQUIRE(pos.adjFace().index() == 5);
    REQUIRE(pos.adjVertex().index() == 1);
    REQUIRE(pos.adjEdge() == 2);
    REQUIRE((pos.face().index() == 0 && pos.vertex().index() == 0));

    // flipVertex
    Pos p = pos;
    p.flipVertex();
    REQUIRE(p.isValid());
    REQUIRE(
        (p.vertex().index() == 1 && p.edge() == 0 && p.face().index() == 0));
    p.flipVertex();
    REQUIRE(p == pos);

    // flipEdge
    p = pos;
    p.flipEdge();
    REQUIRE(p.isValid());
    REQUIRE(
        (p.edge() == 2 && p.vertex().index() == 0 && p.face().index() == 0));
    p.flipEdge();
    REQUIRE(p == pos);

    // flipFace: face 5 = (0, 6, 1), the shared edge is the edge 2
    p = pos;
    REQUIRE(p.flipFace());
    REQUIRE(p.isValid());
    REQUIRE(
        (p.face().index() == 5 && p.edge() == 2 && p.vertex().index() == 0));
    REQUIRE(p.flipFace());
    REQUIRE(p == pos);

    // CCW orientation changes with the flipVertex
    REQUIRE(pos.isCCWOriented());
    p.flipVertex();
    REQUIRE(p != pos);
}

inline void testMeshPosBorderAndStar(const MeshPosTestMesh& m)
{
    using Pos = vcl::MeshPos<MeshPosTestMesh>;

    // face 0 = (0, 1, 2), edge 1 = (1, 2) is on border
    Pos border(m.face(0), m.vertex(1), 1);
    REQUIRE(border.isEdgeOnBorder());
    REQUIRE(!border.adjFace());
    REQUIRE(!border.flipFace()); // does not change anything
    REQUIRE(border.isValid());
    REQUIRE((border.face().index() == 0 && border.edge() == 1));

    // edge 0 = (0, 1) is not on border
    Pos inner(m.face(0), m.vertex(0), 0);
    REQUIRE(!inner.isEdgeOnBorder());

    // star of the interior vertex: it visits all the 6 faces
    std::set<vcl::uint> visited;
    Pos                 p = inner;
    do {
        visited.insert(p.face().index());
        p.nextEdgeAdjacentToV();
    } while (p != inner);
    REQUIRE(visited.size() == 6);
    REQUIRE(inner.numberOfAdjacentFacesToV() == 6);

    // border vertices have two adjacent faces, regardless of the starting
    // face/edge
    for (vcl::uint vi = 1; vi <= 6; ++vi) {
        for (auto f : m.faces()) {
            for (vcl::uint i = 0; i < 3; ++i) {
                if (f.vertex(i).index() == vi) {
                    Pos pv(f, i);
                    REQUIRE(pv.numberOfAdjacentFacesToV() == 2);
                }
            }
        }
    }

    // moves to the next border edge adjacent to the border vertex 1: from the
    // edge (1, 2) of face 0 to the edge (6, 1) of face 5
    Pos b = border;
    REQUIRE(b.nextEdgeOnBorderAdjacentToV());
    REQUIRE(b.isEdgeOnBorder());
    REQUIRE(b != border);
    REQUIRE((b.face().index() == 5 && b.edge() == 1));
    REQUIRE(b.vertex().index() == 1);
}

inline void testMeshPosOrdering(const MeshPosTestMesh& m)
{
    using Pos = vcl::MeshPos<MeshPosTestMesh>;

    Pos a(m.face(0), m.vertex(0), 0);
    Pos b(m.face(1), m.vertex(0), 2);
    Pos c = a;

    REQUIRE(a == c);
    REQUIRE(a != b);
    REQUIRE(a < b);
    REQUIRE(!(b < a));
    REQUIRE(!(a < c));

    // null MeshPos are all equal
    REQUIRE(Pos() == Pos());

    std::set<Pos> positions {a, b, c};
    REQUIRE(positions.size() == 2);
}

TEST_CASE("Mesh Pos")
{
    MeshPosTestMesh m = createHexagonFan();

    testMeshPosBasics(m);
    testMeshPosMoves(m);
    testMeshPosBorderAndStar(m);
    testMeshPosOrdering(m);
}
