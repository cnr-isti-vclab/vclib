// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <vclib/meshes.h>

#include <catch2/catch_test_macros.hpp>

#include <ranges>
#include <vector>

TEST_CASE("Views - Element Views")
{
    namespace views = views;

    MyMesh m = createSampleQuadMesh(); // 4 vertices, 2 faces

    // --- 1. Range Concepts ---
    static_assert(VertexRangeConcept<decltype(m.vertices())>);
    static_assert(VertexRangeConcept<decltype(m | views::vertices)>);
    static_assert(VertexRangeConcept<decltype(views::vertices(m))>);

    static_assert(FaceRangeConcept<decltype(m.faces())>);
    static_assert(FaceRangeConcept<decltype(m | views::faces)>);
    static_assert(FaceRangeConcept<decltype(views::faces(m))>);

    // --- 2. Pipe Syntax on Mesh (m | views::vertices / m | views::faces) ---
    std::size_t vCount = 0;
    for (auto v : m | views::vertices) {
        REQUIRE(v.isValid());
        ++vCount;
    }
    REQUIRE(vCount == 4);

    std::size_t fCount = 0;
    for (auto f : m | views::faces) {
        REQUIRE(f.isValid());
        ++fCount;
    }
    REQUIRE(fCount == 2);

    // --- 3. Function Call Syntax (views::vertices(m) / views::faces(m)) ---
    vCount = 0;
    for (auto v : views::vertices(m)) {
        ++vCount;
    }
    REQUIRE(vCount == 4);

    fCount = 0;
    for (auto f : views::faces(m)) {
        ++fCount;
    }
    REQUIRE(fCount == 2);

    // --- 4. Generic elements<ELEM_ID> Adaptor ---
    vCount = 0;
    for (auto v : m | views::elements<ElemId::VERTEX>) {
        ++vCount;
    }
    REQUIRE(vCount == 4);

    fCount = 0;
    for (auto f : m | views::elements<ElemId::FACE>) {
        ++fCount;
    }
    REQUIRE(fCount == 2);

    // --- 5. Topology Navigation (face | views::vertices) ---
    auto              f0 = m.face(0);
    std::vector<uint> face0VertexIndices;
    for (auto v : f0 | views::vertices) {
        face0VertexIndices.push_back(v.index());
    }
    REQUIRE(face0VertexIndices.size() == 3);
    REQUIRE(face0VertexIndices[0] == f0.vertexIndex(0));
    REQUIRE(face0VertexIndices[1] == f0.vertexIndex(1));
    REQUIRE(face0VertexIndices[2] == f0.vertexIndex(2));

    // Function call syntax on face: views::vertices(f0)
    face0VertexIndices.clear();
    for (auto v : views::vertices(f0)) {
        face0VertexIndices.push_back(v.index());
    }
    REQUIRE(face0VertexIndices.size() == 3);
    REQUIRE(face0VertexIndices[0] == f0.vertexIndex(0));

    // --- 6. views::indices ---
    std::vector<uint> vIndices;
    for (uint idx : m.vertices() | views::indices) {
        vIndices.push_back(idx);
    }
    REQUIRE((vIndices == std::vector<uint> {0, 1, 2, 3}));

    // Pipeline composition: m | views::vertices | views::indices
    vIndices.clear();
    for (uint idx : m | views::vertices | views::indices) {
        vIndices.push_back(idx);
    }
    REQUIRE((vIndices == std::vector<uint> {0, 1, 2, 3}));

    // Function syntax: views::indices(m.vertices())
    vIndices.clear();
    for (uint idx : views::indices(m.vertices())) {
        vIndices.push_back(idx);
    }
    REQUIRE((vIndices == std::vector<uint> {0, 1, 2, 3}));

    // Face indices
    std::vector<uint> fIndices;
    for (uint idx : m | views::faces | views::indices) {
        fIndices.push_back(idx);
    }
    REQUIRE((fIndices == std::vector<uint> {0, 1}));

    // Face vertices piped directly to indices: f0 | views::vertices |
    // views::indices
    std::vector<uint> f0Indices;
    for (uint idx : f0 | views::vertices | views::indices) {
        f0Indices.push_back(idx);
    }
    REQUIRE(f0Indices.size() == 3);
    REQUIRE((f0Indices[0] == 0 && f0Indices[1] == 1 && f0Indices[2] == 2));

    // --- 7. Element Views skipping deleted elements ---
    m.deleteVertex(1);
    vIndices.clear();
    for (uint idx : m | views::vertices | views::indices) {
        vIndices.push_back(idx);
    }
    REQUIRE((vIndices == std::vector<uint> {0, 2, 3}));

    // --- 8. 64-bit mesh indices view without truncation ---
    vcl::TriMesh64 m64;
    m64.addVertex(Point3d(0, 0, 0));
    m64.addVertex(Point3d(1, 0, 0));
    m64.addVertex(Point3d(0, 1, 0));
    m64.addFace(0, 1, 2);

    static_assert(std::same_as<
                  std::ranges::range_value_t<
                      decltype(m64.vertices() | views::indices)>,
                  uint64_t>);
    static_assert(
        std::same_as<
            std::ranges::range_value_t<decltype(m64.faces() | views::indices)>,
            uint64_t>);

    std::vector<uint64_t> v64Indices;
    for (auto idx : m64.vertices() | views::indices) {
        v64Indices.push_back(idx);
    }
    REQUIRE((v64Indices == std::vector<uint64_t> {0, 1, 2}));
}

TEST_CASE("Views - Selection Views")
{
    namespace views = views;

    MyMesh m =
        createSampleQuadMesh(); // 4 vertices (0, 1, 2, 3), 2 faces (0, 1)

    // --- 1. Concepts ---
    static_assert(SelectableRangeConcept<decltype(m.vertices())>);
    static_assert(SelectableRangeConcept<decltype(m | views::vertices)>);
    static_assert(SelectableRangeConcept<decltype(m.faces())>);
    static_assert(SelectableRangeConcept<decltype(m | views::faces)>);
    static_assert(
        SelectableRangeConcept<decltype(m.vertices() | views::selected)>);
    static_assert(
        SelectableRangeConcept<decltype(m.vertices() | views::notSelected)>);

    // --- 2. Initial state: nothing selected ---
    std::size_t selCount = 0;
    for (auto v : m.vertices() | views::selected) {
        (void) v;
        ++selCount;
    }
    REQUIRE(selCount == 0);

    std::size_t notSelCount = 0;
    for (auto v : m.vertices() | views::notSelected) {
        (void) v;
        ++notSelCount;
    }
    REQUIRE(notSelCount == 4);

    // --- 3. Selecting vertices 0 and 2 ---
    m.vertex(0).selected() = true;
    m.vertex(2).selected() = true;

    // Pipe syntax: m.vertices() | views::selected
    std::vector<uint> selIndices;
    for (uint idx : m.vertices() | views::selected | views::indices) {
        selIndices.push_back(idx);
    }
    REQUIRE((selIndices == std::vector<uint> {0, 2}));

    // Pipeline composition: m | views::vertices | views::selected |
    // views::indices
    selIndices.clear();
    for (uint idx : m | views::vertices | views::selected | views::indices) {
        selIndices.push_back(idx);
    }
    REQUIRE((selIndices == std::vector<uint> {0, 2}));

    // Function call syntax: views::selected(m.vertices())
    selIndices.clear();
    for (uint idx : views::selected(m.vertices()) | views::indices) {
        selIndices.push_back(idx);
    }
    REQUIRE((selIndices == std::vector<uint> {0, 2}));

    // --- 4. views::notSelected ---
    std::vector<uint> notSelIndices;
    for (uint idx : m.vertices() | views::notSelected | views::indices) {
        notSelIndices.push_back(idx);
    }
    REQUIRE((notSelIndices == std::vector<uint> {1, 3}));

    // Function call syntax: views::notSelected(m.vertices())
    notSelIndices.clear();
    for (uint idx : views::notSelected(m.vertices()) | views::indices) {
        notSelIndices.push_back(idx);
    }
    REQUIRE((notSelIndices == std::vector<uint> {1, 3}));

    // --- 5. views::selection (reading bit values) ---
    std::vector<bool> selValues;
    for (bool s : m.vertices() | views::selection) {
        selValues.push_back(s);
    }
    REQUIRE((selValues == std::vector<bool> {true, false, true, false}));

    // Writing through views::selection (BitProxy assignment)
    for (auto s : m.vertices() | views::selection) {
        s = true;
    }
    selIndices.clear();
    for (uint idx : m.vertices() | views::selected | views::indices) {
        selIndices.push_back(idx);
    }
    REQUIRE((selIndices == std::vector<uint> {0, 1, 2, 3}));

    // Resetting selection
    for (auto s : m.vertices() | views::selection) {
        s = false;
    }
    selIndices.clear();
    for (uint idx : m.vertices() | views::selected | views::indices) {
        selIndices.push_back(idx);
    }
    REQUIRE(selIndices.empty());

    // --- 6. Face Selection ---
    m.face(1).selected() = true;

    std::vector<uint> selFaces;
    for (uint idx : m | views::faces | views::selected | views::indices) {
        selFaces.push_back(idx);
    }
    REQUIRE((selFaces == std::vector<uint> {1}));

    std::vector<uint> notSelFaces;
    for (uint idx : m | views::faces | views::notSelected | views::indices) {
        notSelFaces.push_back(idx);
    }
    REQUIRE((notSelFaces == std::vector<uint> {0}));

    // --- 7. Selection combined with Deleted Elements ---
    m.vertex(0).selected() = true;
    m.vertex(1).selected() = true;
    m.vertex(2).selected() = true;
    m.deleteVertex(0); // Vertex 0 is deleted, so it must be skipped

    selIndices.clear();
    for (uint idx : m | views::vertices | views::selected | views::indices) {
        selIndices.push_back(idx);
    }
    REQUIRE((selIndices == std::vector<uint> {1, 2}));

    // --- 8. Modifying ElementProxy through views::selected iteration ---
    for (auto v : m.vertices() | views::selected) {
        v.selected() = false;
    }
    selIndices.clear();
    for (uint idx : m | views::vertices | views::selected | views::indices) {
        selIndices.push_back(idx);
    }
    REQUIRE(selIndices.empty());
}

TEST_CASE("Views - Component Views")
{
    namespace views = views;

    MyMesh m = createSampleQuadMesh(); // 4 vertices, 2 faces

    // --- 1. Concepts ---
    // Input ranges satisfy PositionRangeConcept
    static_assert(PositionRangeConcept<decltype(m.vertices())>);
    static_assert(PositionRangeConcept<decltype(m | views::vertices)>);
    static_assert(
        PositionRangeConcept<decltype(m.vertices() | views::selected)>);

    // Output ranges satisfy std::ranges::range and yield Point3f
    static_assert(
        std::ranges::range<decltype(m.vertices() | views::positions)>);
    static_assert(std::ranges::range<
                  decltype(m.vertices() | views::selected | views::positions)>);
    static_assert(std::same_as<
                  std::ranges::range_value_t<
                      decltype(m.vertices() | views::positions)>,
                  Point3f>);

    static_assert(NormalRangeConcept<decltype(m.vertices())>);
    static_assert(NormalRangeConcept<decltype(m | views::vertices)>);
    static_assert(NormalRangeConcept<decltype(m.vertices() | views::selected)>);
    static_assert(std::ranges::range<decltype(m.vertices() | views::normals)>);

    // MyFace does not have Normal, so NormalRangeConcept is false for m.faces()
    static_assert(!NormalRangeConcept<decltype(m.faces())>);
    static_assert(!NormalRangeConcept<decltype(m | views::faces)>);

    static_assert(ColorRangeConcept<decltype(m.vertices())>);
    static_assert(ColorRangeConcept<decltype(m | views::vertices)>);
    static_assert(ColorRangeConcept<decltype(m.faces())>);
    static_assert(ColorRangeConcept<decltype(m | views::faces)>);
    static_assert(std::ranges::range<decltype(m.vertices() | views::colors)>);
    static_assert(std::ranges::range<decltype(m.faces() | views::colors)>);

    // --- 2. views::positions ---
    // Fast path: m.vertices() | views::positions returns ComponentView
    std::size_t posCount = 0;
    for (const Point3f& p : m.vertices() | views::positions) {
        (void) p;
        ++posCount;
    }
    REQUIRE(posCount == 4);

    // Function call syntax: views::positions(m.vertices())
    posCount = 0;
    for (const Point3f& p : views::positions(m.vertices())) {
        (void) p;
        ++posCount;
    }
    REQUIRE(posCount == 4);

    // Pipeline composition: m | views::vertices | views::positions
    posCount = 0;
    for (const Point3f& p : m | views::vertices | views::positions) {
        (void) p;
        ++posCount;
    }
    REQUIRE(posCount == 4);

    // Mutation through ComponentView: shift all vertices by (1, 0, 0)
    for (Point3f& p : m.vertices() | views::positions) {
        p.x() += 1.0f;
    }
    REQUIRE(m.vertex(0).position().x() == 1.0f); // Was 0.0f
    REQUIRE(m.vertex(1).position().x() == 2.0f); // Was 1.0f

    // Transform path: filtered view m.vertices() | views::selected |
    // views::positions
    m.vertex(0).selected()  = true;
    m.vertex(2).selected()  = true;
    std::size_t selPosCount = 0;
    for (Point3f& p : m.vertices() | views::selected | views::positions) {
        p.y() += 5.0f;
        ++selPosCount;
    }
    REQUIRE(selPosCount == 2);
    REQUIRE(m.vertex(0).position().y() == 5.0f); // Was 0.0f
    REQUIRE(m.vertex(1).position().y() == 0.0f); // Not selected, unchanged
    REQUIRE(m.vertex(2).position().y() == 6.0f); // Was 1.0f
    REQUIRE(m.vertex(3).position().y() == 1.0f); // Not selected, unchanged

    // Topological traversal: face | views::vertices | views::positions
    auto                 f0 = m.face(0);
    std::vector<Point3f> f0Positions;
    for (const Point3f& p : f0 | views::vertices | views::positions) {
        f0Positions.push_back(p);
    }
    REQUIRE(f0Positions.size() == 3);
    REQUIRE(f0Positions[0] == m.vertex(f0.vertexIndex(0)).position());
    REQUIRE(f0Positions[1] == m.vertex(f0.vertexIndex(1)).position());
    REQUIRE(f0Positions[2] == m.vertex(f0.vertexIndex(2)).position());

    // --- 3. views::normals ---
    // Vertex normals
    std::ranges::fill(m.vertices() | views::normals, Point3f(0.0f, 0.0f, 1.0f));
    for (const Point3f& n : m.vertices() | views::normals) {
        REQUIRE((n == Point3f(0.0f, 0.0f, 1.0f)));
    }

    // Filtered vertex normals
    m.vertex(0).selected() = true;
    m.vertex(1).selected() = false;
    for (Point3f& n : m.vertices() | views::selected | views::normals) {
        n = Point3f(1.0f, 0.0f, 0.0f);
    }
    REQUIRE((m.vertex(0).normal() == Point3f(1.0f, 0.0f, 0.0f)));
    REQUIRE((m.vertex(1).normal() == Point3f(0.0f, 0.0f, 1.0f)));

    // Face normals on a mesh type with face normals
    using FaceWithNormals = FaceComponents<
        face::BitFlags,
        face::TriangleVertexReferences<vcl::uint>,
        face::Normal3f,
        face::Color>;
    using MeshWithFaceNormals = Mesh<
        mesh::VertexContainer<MyVertexComponents>,
        mesh::FaceContainer<FaceWithNormals>>;

    MeshWithFaceNormals mfn;
    mfn.addVertex();
    mfn.addVertex();
    mfn.addVertex();
    mfn.addFace(0, 1, 2);

    static_assert(NormalRangeConcept<decltype(mfn.faces())>);
    static_assert(NormalRangeConcept<decltype(mfn | views::faces)>);

    std::ranges::fill(mfn.faces() | views::normals, Point3f(0.0f, 1.0f, 0.0f));
    for (const Point3f& n : mfn.faces() | views::normals) {
        REQUIRE((n == Point3f(0.0f, 1.0f, 0.0f)));
    }

    // --- 4. views::colors ---
    // Fill all vertex colors
    std::ranges::fill(m.vertices() | views::colors, Color::Red);
    for (const Color& c : m.vertices() | views::colors) {
        REQUIRE(c == Color::Red);
    }

    // Fill selected vertex colors
    m.vertex(0).selected() = false;
    m.vertex(1).selected() = true;
    m.vertex(2).selected() = false;
    m.vertex(3).selected() = true;
    std::ranges::fill(
        m.vertices() | views::selected | views::colors, Color::Green);
    REQUIRE(m.vertex(0).color() == Color::Red);
    REQUIRE(m.vertex(1).color() == Color::Green);
    REQUIRE(m.vertex(2).color() == Color::Red);
    REQUIRE(m.vertex(3).color() == Color::Green);

    // Fill face colors
    std::ranges::fill(m.faces() | views::colors, Color::Blue);
    for (const Color& c : m.faces() | views::colors) {
        REQUIRE(c == Color::Blue);
    }

    // --- 5. Generic views::component<COMP_ID> ---
    std::size_t cPosCount = 0;
    for (const auto& p : m.vertices() | views::component<CompId::POSITION>) {
        (void) p;
        ++cPosCount;
    }
    REQUIRE(cPosCount == 4);

    std::size_t cNormCount = 0;
    for (const auto& n : m.vertices() | views::component<CompId::NORMAL>) {
        (void) n;
        ++cNormCount;
    }
    REQUIRE(cNormCount == 4);

    std::size_t cColCount = 0;
    for (const auto& c : m.faces() | views::component<CompId::COLOR>) {
        (void) c;
        ++cColCount;
    }
    REQUIRE(cColCount == 2);

    // --- 6. Composite adaptors ---
    auto        sel_pos   = views::selected | views::positions;
    std::size_t compCount = 0;
    for (const auto& p : m.vertices() | sel_pos) {
        (void) p;
        ++compCount;
    }
    REQUIRE(compCount == 2); // Vertices 1 and 3 are selected

    // --- 7. PrincipalCurvatures View ---
    CurvatureMesh cmesh;
    cmesh.addVertices(3);
    cmesh.vertex(0).principalCurvature().maxValue() = 1.0f;
    cmesh.vertex(1).principalCurvature().maxValue() = 2.0f;
    cmesh.vertex(2).principalCurvature().maxValue() = 3.0f;

    static_assert(PrincipalCurvaturesRangeConcept<decltype(cmesh.vertices())>);
    static_assert(std::ranges::range<
                  decltype(cmesh.vertices() | views::principalCurvatures)>);

    std::size_t curvCount  = 0;
    float       sumMaxCurv = 0.0f;
    for (const auto& pc : cmesh.vertices() | views::principalCurvatures) {
        sumMaxCurv += pc.maxValue();
        ++curvCount;
    }
    REQUIRE(curvCount == 3);
    REQUIRE(sumMaxCurv == 6.0f);

    // Direct proxy member
    REQUIRE(cmesh.vertices().principalCurvatures().size() == 3);
    REQUIRE(cmesh.vertices().principalCurvatures()[1].maxValue() == 2.0f);

    // --- 8. Qualities View ---
    QualityMesh qmesh;
    qmesh.addVertices(3);
    qmesh.vertex(0).quality() = 10.0f;
    qmesh.vertex(1).quality() = 20.0f;
    qmesh.vertex(2).quality() = 30.0f;

    static_assert(QualitiesRangeConcept<decltype(qmesh.vertices())>);
    static_assert(QualityRangeConcept<decltype(qmesh.vertices())>);
    static_assert(
        std::ranges::range<decltype(qmesh.vertices() | views::qualities)>);
    static_assert(
        std::ranges::range<decltype(qmesh.vertices() | views::quality)>);

    std::size_t qCount     = 0;
    float       sumQuality = 0.0f;
    for (float q : qmesh.vertices() | views::qualities) {
        sumQuality += q;
        ++qCount;
    }
    REQUIRE(qCount == 3);
    REQUIRE(sumQuality == 60.0f);

    // Both views::qualities and views::quality work identically
    float sumQualityAlt = 0.0f;
    for (float q : qmesh.vertices() | views::quality) {
        sumQualityAlt += q;
    }
    REQUIRE(sumQualityAlt == 60.0f);

    // Direct proxy member
    REQUIRE(qmesh.vertices().qualities().size() == 3);
    REQUIRE(qmesh.vertices().qualities()[1] == 20.0f);

    // --- 9. Tangents View & bitangent ---
    TangentMesh tmesh;
    tmesh.addVertices(3);
    tmesh.vertex(0).tangent() = Point3f(1.0f, 0.0f, 0.0f);
    tmesh.vertex(1).tangent() = Point3f(0.0f, 2.0f, 0.0f);
    tmesh.vertex(2).tangent() = Point3f(0.0f, 0.0f, 3.0f);
    tmesh.vertex(0).normal()  = Point3f(0.0f, 0.0f, 1.0f);

    static_assert(TangentsRangeConcept<decltype(tmesh.vertices())>);
    static_assert(TangentRangeConcept<decltype(tmesh.vertices())>);
    static_assert(
        std::ranges::range<decltype(tmesh.vertices() | views::tangents)>);
    static_assert(
        std::ranges::range<decltype(tmesh.vertices() | views::tangent)>);

    std::size_t tCount = 0;
    Point3f     sumTan(0.0f, 0.0f, 0.0f);
    for (const auto& t : tmesh.vertices() | views::tangents) {
        sumTan += t;
        ++tCount;
    }
    REQUIRE(tCount == 3);
    REQUIRE(sumTan == Point3f(1.0f, 2.0f, 3.0f));

    // Both views::tangents and views::tangent work identically
    Point3f sumTanAlt(0.0f, 0.0f, 0.0f);
    for (const auto& t : tmesh.vertices() | views::tangent) {
        sumTanAlt += t;
    }
    REQUIRE(sumTanAlt == Point3f(1.0f, 2.0f, 3.0f));

    // Direct proxy member (returns ComponentView over std::pair<Point3f, bool>)
    REQUIRE(tmesh.vertices().tangents().size() == 3);
    REQUIRE(tmesh.vertices().tangents()[1].first == Point3f(0.0f, 2.0f, 0.0f));
    REQUIRE(tmesh.vertices().tangents()[1].second == true);

    // bitangent calculation: n = (0, 0, 1), t = (1, 0, 0), rightHanded = true
    // => n x t = (0, 1, 0)
    Point3f bitan = tmesh.vertex(0).bitangent();
    REQUIRE(bitan == Point3f(0.0f, 1.0f, 0.0f));

    // left-handed bitangent: t x n = (0, -1, 0)
    tmesh.vertex(0).tangentRightHanded() = false;
    REQUIRE(tmesh.vertex(0).bitangent() == Point3f(0.0f, -1.0f, 0.0f));
}
