// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IGL_BOOLEANS_H
#define VCL_IGL_BOOLEANS_H

#if defined(VCLIB_WITH_CGAL) && defined(VCLIB_WITH_BOOST)

#include <vclib/algorithms/mesh.h>
#include <vclib/mesh.h>

#include <igl/copyleft/cgal/CSGTree.h>
#include <igl/triangle_triangle_adjacency.h>
#include <map>
#include <vector>

#ifdef WIN32
#undef DIFFERENCE
#endif

namespace vcl::igl {

namespace detail {

/**
 * @brief Rebuilds polygons from triangulated mesh boolean results.
 *
 * This function takes the triangular output of a mesh boolean operation and
 * attempts to reconstruct the original polygonal faces by grouping adjacent
 * triangles that originated from the same source polygon.
 *
 * @tparam ScalarType: The scalar type used for the vertex coordinates.
 * @param[in] FR: The face matrix (triangles) resulting from the boolean
 * operation.
 * @param[in] indices: A vector mapping each resulting triangle to its birth
 * face index (from the concatenated triangles of m0 and m1).
 * @param[in] F0Rows: The number of triangles in the first input mesh (m0).
 * @param[in] m0BiMap: A bidirectional map linking triangle indices to polygon
 * indices for m0.
 * @param[in] m1BiMap: A bidirectional map linking triangle indices to polygon
 * indices for m1.
 * @param[out] outIndices: An Eigen::VectorXi that will be populated with the
 * index of the original input polygonal face that generated each resulting
 * output polygon/triangle. The index is mapped over the concatenated input
 * polygons (m0 then m1).
 * @return An Eigen::MatrixXi containing the reconstructed polygons, padded with
 * -1 if polygons have different number of vertices.
 */
template<typename ScalarType>
Eigen::MatrixXi rebuildPolygons(
    const Eigen::MatrixX3i&  FR,
    const Eigen::VectorXi&   indices,
    uint                     F0Rows,
    uint                     m0FaceContainerSize,
    const TriPolyIndexBiMap& m0BiMap,
    const TriPolyIndexBiMap& m1BiMap,
    Eigen::VectorXi&         outIndices)
{
    // Step 1: Map each resulting triangle to its global polygon ID
    // (m0 polygons first, then m1 polygons)
    std::vector<int> globalPolyIds(FR.rows());

    // number of polygons in m0 to offset the global IDs for m1 polygons
    uint m0PolyCount = m0FaceContainerSize;

    for (int i = 0; i < FR.rows(); ++i) {
        int j = indices[i];
        if (j < (int) F0Rows) {
            globalPolyIds[i] = m0BiMap.polygon(j);
        }
        else {
            globalPolyIds[i] = m0PolyCount + m1BiMap.polygon(j - F0Rows);
        }
    }

    // Step 2: Compute triangle-triangle adjacency to find connected components
    Eigen::MatrixXi TT, TTi;
    ::igl::triangle_triangle_adjacency(FR, TT, TTi);

    std::vector<bool>             visited(FR.rows(), false);
    std::vector<std::vector<int>> newPolygons;
    std::vector<int>              newPolygonsGlobalIds;
    std::vector<int>              keepTriangles;
    std::vector<int>              keepTrianglesGlobalIds;

    // Step 3: Group adjacent triangles belonging to the same original polygon
    for (int i = 0; i < FR.rows(); ++i) {
        if (visited[i])
            continue;

        // Perform BFS to find all connected triangles from the same source
        // polygon
        std::vector<int> comp;
        std::vector<int> queue;
        queue.push_back(i);
        visited[i] = true;

        auto targetId = globalPolyIds[i];

        while (!queue.empty()) {
            int curr = queue.back();
            queue.pop_back();
            comp.push_back(curr);

            for (int e = 0; e < 3; ++e) {
                int adj = TT(curr, e);
                // If adjacent triangle exists, is unvisited, and comes from the
                // same polygon
                if (adj != -1 && !visited[adj] &&
                    globalPolyIds[adj] == targetId) {
                    visited[adj] = true;
                    queue.push_back(adj);
                }
            }
        }

        // If the component is just a single triangle, keep it as is
        if (comp.size() == 1) {
            keepTriangles.push_back(comp[0]);
            keepTrianglesGlobalIds.push_back(globalPolyIds[comp[0]]);
            continue;
        }

        // Step 4: Extract the boundary edges of the grouped triangles
        std::vector<std::pair<int, int>> boundEdges;
        bool                             manifoldBoundary = true;
        std::map<int, int>               inDegree, outDegree;
        std::map<int, int>               nextVert;

        for (int t : comp) {
            for (int e = 0; e < 3; ++e) {
                int adj = TT(t, e);
                // An edge is on the boundary if it has no neighbor or the
                // neighbor is from a different polygon
                if (adj == -1 || globalPolyIds[adj] != targetId) {
                    int v0 = FR(t, e);
                    int v1 = FR(t, (e + 1) % 3);
                    boundEdges.push_back({v0, v1});

                    // Track vertex degrees to ensure the boundary is a simple
                    // manifold loop
                    outDegree[v0]++;
                    inDegree[v1]++;
                    nextVert[v0] = v1;
                    if (outDegree[v0] > 1 || inDegree[v1] > 1) {
                        manifoldBoundary = false;
                    }
                }
            }
        }

        // If boundary is not manifold (e.g. self-intersecting) or empty,
        // fallback to triangles
        if (!manifoldBoundary || boundEdges.empty()) {
            for (int t : comp) {
                keepTriangles.push_back(t);
                keepTrianglesGlobalIds.push_back(globalPolyIds[t]);
            }
            continue;
        }

        // Step 5: Trace the boundary edges to form the new reconstructed
        // polygon
        int              startVert = boundEdges[0].first;
        int              currVert  = startVert;
        std::vector<int> loop;
        while (true) {
            loop.push_back(currVert);
            auto it = nextVert.find(currVert);
            if (it == nextVert.end()) {
                manifoldBoundary =
                    false; // Dead end, should not happen for a closed loop
                break;
            }
            currVert = it->second;
            if (currVert == startVert)
                break; // Loop closed

            // Prevent infinite loops in case of disjoint boundary components
            if (loop.size() > boundEdges.size()) {
                manifoldBoundary = false;
                break;
            }
        }

        // If a single closed loop was successfully traced and encompasses all
        // boundary edges
        if (manifoldBoundary && loop.size() == boundEdges.size()) {
            newPolygons.push_back(loop);
            newPolygonsGlobalIds.push_back(globalPolyIds[comp[0]]);
        }
        else {
            // Otherwise, keep the original triangles
            for (int t : comp) {
                keepTriangles.push_back(t);
                keepTrianglesGlobalIds.push_back(globalPolyIds[t]);
            }
        }
    }

    // Step 6: Format the output matrix
    if (newPolygons.empty()) {
        outIndices.resize(FR.rows());
        for (int i = 0; i < FR.rows(); ++i) {
            outIndices[i] = globalPolyIds[i];
        }
        return FR;
    }

    // Find the maximum polygon size for matrix padding
    int maxPolySize = 3;
    for (const auto& poly : newPolygons) {
        if ((int) poly.size() > maxPolySize) {
            maxPolySize = poly.size();
        }
    }

    // Create the padded matrix, initialized to -1
    Eigen::MatrixXi FRPoly(
        keepTriangles.size() + newPolygons.size(), maxPolySize);
    FRPoly.setConstant(-1);
    outIndices.resize(keepTriangles.size() + newPolygons.size());

    // Populate the matrix with retained triangles
    int row = 0;
    for (size_t i = 0; i < keepTriangles.size(); ++i) {
        int t           = keepTriangles[i];
        FRPoly(row, 0)  = FR(t, 0);
        FRPoly(row, 1)  = FR(t, 1);
        FRPoly(row, 2)  = FR(t, 2);
        outIndices[row] = keepTrianglesGlobalIds[i];
        row++;
    }

    // Populate the matrix with the newly reconstructed polygons
    for (size_t i = 0; i < newPolygons.size(); ++i) {
        const auto& poly = newPolygons[i];
        for (size_t j = 0; j < poly.size(); ++j) {
            FRPoly(row, j) = poly[j];
        }
        outIndices[row] = newPolygonsGlobalIds[i];
        row++;
    }

    return FRPoly;
}

template<typename MeshType>
void transferFaceComponents(
    const MeshType&        m0,
    const MeshType&        m1,
    const Eigen::VectorXi& indices,
    MeshType&              out,
    auto                   normalPlaceHolder,
    Color                  colorPlaceHolder,
    auto                   qualityPlaceHolder)
{
    constexpr uint F = vcl::ElemId::FACE;

    auto transferComponent = [&]<uint COMP_ID>(
                                 auto&       f,
                                 int         index,
                                 uint        m0Faces,
                                 const auto& placeHolder) {
        // avoid compile errors for meshes without the component COMP_ID
        if constexpr (MeshType::template hasPerElementComponent<F, COMP_ID>()) {
            if (index < (int) m0Faces) {
                if (vcl::isPerElementComponentAvailable<F, COMP_ID>(m0))
                    f.template componentValue<COMP_ID>() =
                        m0.face(index).template componentValue<COMP_ID>();
                else
                    f.template componentValue<COMP_ID>() = placeHolder;
            }
            else {
                int m1Idx = index - m0Faces;
                if (vcl::isPerElementComponentAvailable<F, COMP_ID>(m1))
                    f.template componentValue<COMP_ID>() =
                        m1.face(m1Idx).template componentValue<COMP_ID>();
                else
                    f.template componentValue<COMP_ID>() = placeHolder;
            }
        }
    };

    bool transferNormals =
        vcl::isPerFaceNormalAvailable(m0) || vcl::isPerFaceNormalAvailable(m1);
    bool transferColors =
        vcl::isPerFaceColorAvailable(m0) || vcl::isPerFaceColorAvailable(m1);
    bool transferQuality = vcl::isPerFaceQualityAvailable(m0) ||
                           vcl::isPerFaceQualityAvailable(m1);

    if (transferNormals || transferColors || transferQuality) {
        if (transferNormals)
            enableIfPerFaceNormalOptional(out);
        if (transferColors)
            enableIfPerFaceColorOptional(out);
        if (transferQuality)
            enableIfPerFaceQualityOptional(out);

        uint m0Faces = m0.faceContainerSize();
        for (uint i = 0; auto& f : out.faces()) {
            if (transferNormals)
                transferComponent.template operator()<CompId::NORMAL>(
                    f, indices[i], m0Faces, normalPlaceHolder);
            if (transferColors)
                transferComponent.template operator()<CompId::COLOR>(
                    f, indices[i], m0Faces, colorPlaceHolder);
            if (transferQuality)
                transferComponent.template operator()<CompId::QUALITY>(
                    f, indices[i], m0Faces, qualityPlaceHolder);
            ++i;
        }
    }
}

template<typename MeshType, typename FMat>
void transferVertexComponents(
    const MeshType&        m0,
    const MeshType&        m1,
    const FMat&            F0,
    const FMat&            F1,
    const Eigen::MatrixXi& FRTri,
    const Eigen::VectorXi& indicesTri,
    MeshType&              out,
    auto                   normalPlaceHolder,
    Color                  colorPlaceHolder,
    auto                   qualityPlaceHolder)
{
    constexpr uint V = vcl::ElemId::VERTEX;
    using ScalarType = typename MeshType::VertexType::PositionType::ScalarType;

    bool transferNormals = vcl::isPerVertexNormalAvailable(m0) ||
                           vcl::isPerVertexNormalAvailable(m1);
    bool transferColors = vcl::isPerVertexColorAvailable(m0) ||
                          vcl::isPerVertexColorAvailable(m1);
    bool transferQuality = vcl::isPerVertexQualityAvailable(m0) ||
                           vcl::isPerVertexQualityAvailable(m1);

    if (!transferNormals && !transferColors && !transferQuality)
        return;

    if (transferNormals)
        enableIfPerVertexNormalOptional(out);
    if (transferColors)
        enableIfPerVertexColorOptional(out);
    if (transferQuality)
        enableIfPerVertexQualityOptional(out);

    uint m0Triangles = F0.rows();

    std::vector<Point4f>            colorAccum;
    std::vector<Point3<ScalarType>> normalAccum;
    std::vector<double>             qualityAccum;
    std::vector<int>                incidentCount(out.vertexCount(), 0);

    if (transferColors)
        colorAccum.resize(out.vertexCount(), Point4f());
    if (transferNormals)
        normalAccum.resize(out.vertexCount(), Point3<ScalarType>());
    if (transferQuality)
        qualityAccum.resize(out.vertexCount(), 0.0);

    auto accumulateComponent = [&]<uint COMP_ID, typename AccumType>(
                                   std::vector<AccumType>& accum,
                                   int                     outVIdx,
                                   bool                    fromM0,
                                   const Eigen::Vector3i&  birthTri,
                                   const auto&             bcoords,
                                   const auto&             placeHolder) {
        if constexpr (MeshType::template hasPerElementComponent<V, COMP_ID>()) {
            auto c0 = placeHolder;
            auto c1 = placeHolder;
            auto c2 = placeHolder;

            if (fromM0 && vcl::isPerElementComponentAvailable<V, COMP_ID>(m0)) {
                c0 = m0.vertex(birthTri[0]).template componentValue<COMP_ID>();
                c1 = m0.vertex(birthTri[1]).template componentValue<COMP_ID>();
                c2 = m0.vertex(birthTri[2]).template componentValue<COMP_ID>();
            }
            else if (
                !fromM0 &&
                vcl::isPerElementComponentAvailable<V, COMP_ID>(m1)) {
                c0 = m1.vertex(birthTri[0]).template componentValue<COMP_ID>();
                c1 = m1.vertex(birthTri[1]).template componentValue<COMP_ID>();
                c2 = m1.vertex(birthTri[2]).template componentValue<COMP_ID>();
            }

            if constexpr (COMP_ID == CompId::COLOR) {
                Point4f v;
                for (uint i = 0; i < 4; i++) {
                    v[i] = c0[i] * bcoords[0] + c1[i] * bcoords[1] +
                           c2[i] * bcoords[2];
                }

                accum[outVIdx] += v;
            }
            else {
                auto v = c0 * bcoords[0] + c1 * bcoords[1] + c2 * bcoords[2];
                accum[outVIdx] += v;
            }
        }
    };

    for (int i = 0; i < FRTri.rows(); ++i) {
        int  birthTriIdx = indicesTri[i];
        bool fromM0      = birthTriIdx < (int) m0Triangles;

        Eigen::Vector3i birthTri;
        if (fromM0) {
            birthTri = F0.row(birthTriIdx);
        }
        else {
            birthTri = F1.row(birthTriIdx - m0Triangles);
        }

        Point3<ScalarType> p0 = fromM0 ? m0.vertex(birthTri[0]).position() :
                                         m1.vertex(birthTri[0]).position();
        Point3<ScalarType> p1 = fromM0 ? m0.vertex(birthTri[1]).position() :
                                         m1.vertex(birthTri[1]).position();
        Point3<ScalarType> p2 = fromM0 ? m0.vertex(birthTri[2]).position() :
                                         m1.vertex(birthTri[2]).position();

        for (int j = 0; j < 3; ++j) {
            int  outVIdx = FRTri(i, j);
            auto outP    = out.vertex(outVIdx).position();

            auto bcoords = vcl::Triangle3<ScalarType>::barycentricCoordinates(
                p0, p1, p2, outP);

            if (transferColors) {
                accumulateComponent.template operator()<CompId::COLOR>(
                    colorAccum,
                    outVIdx,
                    fromM0,
                    birthTri,
                    bcoords,
                    colorPlaceHolder);
            }
            if (transferNormals) {
                accumulateComponent.template operator()<CompId::NORMAL>(
                    normalAccum,
                    outVIdx,
                    fromM0,
                    birthTri,
                    bcoords,
                    normalPlaceHolder);
            }
            if (transferQuality) {
                accumulateComponent.template operator()<CompId::QUALITY>(
                    qualityAccum,
                    outVIdx,
                    fromM0,
                    birthTri,
                    bcoords,
                    qualityPlaceHolder);
            }

            incidentCount[outVIdx]++;
        }
    }

    auto finalizeComponent = [&]<uint COMP_ID, typename AccumType>(
                                 const std::vector<AccumType>& accum,
                                 int                           incCount,
                                 int                           vIdx) {
        if constexpr (MeshType::template hasPerElementComponent<V, COMP_ID>()) {
            if constexpr (COMP_ID == CompId::COLOR) {
                auto clampV = [](float v) {
                    return static_cast<uint8_t>(std::clamp(v, 0.0f, 255.0f));
                };

                float inv = 1.0f / incCount;
                out.vertex(vIdx).template componentValue<COMP_ID>() = Color(
                    clampV(accum[vIdx][0] * inv),
                    clampV(accum[vIdx][1] * inv),
                    clampV(accum[vIdx][2] * inv),
                    clampV(accum[vIdx][3] * inv));
            }
            else if constexpr (COMP_ID == CompId::NORMAL) {
                out.vertex(vIdx).template componentValue<COMP_ID>() =
                    accum[vIdx].normalized();
            }
            else {
                out.vertex(vIdx).template componentValue<COMP_ID>() =
                    accum[vIdx] / incCount;
            }
        }
    };

    for (uint i = 0; i < out.vertexCount(); ++i) {
        int incCount = incidentCount[i];
        if (incCount > 0) {
            if (transferColors) {
                finalizeComponent.template operator()<CompId::COLOR>(
                    colorAccum, incCount, i);
            }
            if (transferNormals) {
                finalizeComponent.template operator()<CompId::NORMAL>(
                    normalAccum, incCount, i);
            }
            if (transferQuality) {
                finalizeComponent.template operator()<CompId::QUALITY>(
                    qualityAccum, incCount, i);
            }
        }
    }
}

} // namespace detail

enum class MeshBoolean : int {
    UNION        = ::igl::MESH_BOOLEAN_TYPE_UNION,
    INTERSECTION = ::igl::MESH_BOOLEAN_TYPE_INTERSECT,
    DIFFERENCE   = ::igl::MESH_BOOLEAN_TYPE_MINUS,
    XOR          = ::igl::MESH_BOOLEAN_TYPE_XOR,
    RESOLVE      = ::igl::MESH_BOOLEAN_TYPE_RESOLVE,
    COUNT        = ::igl::NUM_MESH_BOOLEAN_TYPES
};

/**
 * @brief Perform boolean operations between two meshes using libigl's
 * CGAL-based implementation.
 *
 * @param[in] m0: First input mesh.
 * @param[in] m1: Second input mesh.
 * @param[in] op: The type of boolean operation to perform.
 * @return The resulting mesh after the boolean operation.
 *
 * @throws std::runtime_error if the input meshes do not induce a piecewise
 * constant winding number field (i.e., if they are not watertight).
 *
 * @throws vcl::MissingCompactnessException if the vertex containers of the
 * input meshes are not compact.
 */
template<FaceMeshConcept MeshType>
MeshType meshBoolean(const MeshType& m0, const MeshType& m1, MeshBoolean op)
{
    // TODO: allow to use non-compact meshes

    using ScalarType = MeshType::VertexType::PositionType::ScalarType;

    using EigenMatrixX3m = Eigen::Matrix<ScalarType, Eigen::Dynamic, 3>;

    // vcl to eigen meshes
    TriPolyIndexBiMap m0BiMap;
    TriPolyIndexBiMap m1BiMap;

    auto V0 = vcl::vertexPositionsMatrix<EigenMatrixX3m>(m0);
    auto F0 =
        vcl::triangulatedFaceVertexIndicesMatrix<Eigen::MatrixX3i>(m0, m0BiMap);

    auto V1 = vcl::vertexPositionsMatrix<EigenMatrixX3m>(m1);
    auto F1 =
        vcl::triangulatedFaceVertexIndicesMatrix<Eigen::MatrixX3i>(m1, m1BiMap);

    EigenMatrixX3m  VR;
    Eigen::MatrixXi FR;
    Eigen::VectorXi indices; // mapping indices for birth faces

    bool result = ::igl::copyleft::cgal::mesh_boolean(
        V0,
        F0,
        V1,
        F1,
        static_cast<::igl::MeshBooleanType>(op),
        VR,
        FR,
        indices);

    if (!result) {
        throw std::runtime_error(
            "Mesh inputs must induce a piecewise constant winding number "
            "field. Make sure that both the input meshes are watertight "
            "(closed).");
    }

    Eigen::MatrixXi FRTri      = FR;
    Eigen::VectorXi indicesTri = indices;

    Eigen::VectorXi outIndices;
    if constexpr (vcl::PolygonMeshConcept<MeshType>) {
        FR = detail::rebuildPolygons<ScalarType>(
            FR,
            indices,
            F0.rows(),
            m0.faceContainerSize(),
            m0BiMap,
            m1BiMap,
            outIndices);
        indices = outIndices; // to map back to original polygon indices
    }
    else {
        // if one of the meshes has deleted faces, we need to remap the indices
        // to the original face indices
        if (m0.faceContainerSize() != m0.faceCount() ||
            m1.faceContainerSize() != m1.faceCount()) {
            outIndices.resize(indices.size());
            uint m0Size = m0.faceContainerSize();
            for (int i = 0; i < indices.size(); ++i) {
                int j = indices[i];
                if (j < F0.rows()) {
                    outIndices[i] = m0BiMap.polygon(j);
                }
                else {
                    outIndices[i] = m0Size + m1BiMap.polygon(j - F0.rows());
                }
            }
            indices = outIndices;
        }
    }

    // TODO: before returning, we should post-process the output mesh to
    // transfer the vertex attributes (e.g., vertex colors, etc.) from the
    // input meshes to the output mesh.

    auto out = vcl::meshFromMatrices<MeshType>(VR, FR);

    using FNormalType = typename MeshType::FaceType::NormalType;
    using VNormalType = typename MeshType::VertexType::NormalType;

    detail::transferFaceComponents(
        m0, m1, indices, out, FNormalType(), Color::Gray, 0.0);

    detail::transferVertexComponents(
        m0,
        m1,
        F0,
        F1,
        FRTri,
        indicesTri,
        out,
        VNormalType(),
        Color::Gray,
        0.0);

    return out;
}

} // namespace vcl::igl

#endif // VCLIB_WITH_CGAL

#endif // VCL_IGL_BOOLEANS_H
