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

#ifdef WIN32
#undef DIFFERENCE
#endif

namespace vcl::igl {

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
 * @param[in] m1: First input mesh.
 * @param[in] m2: Second input mesh.
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
MeshType meshBoolean(const MeshType& m1, const MeshType& m2, MeshBoolean op)
{
    // TODO: allow to use non-compact meshes

    using ScalarType = MeshType::VertexType::PositionType::ScalarType;

    using EigenMatrixX3m = Eigen::Matrix<ScalarType, Eigen::Dynamic, 3>;

    // vcl to eigen meshes
    TriPolyIndexBiMap m1BiMap;
    TriPolyIndexBiMap m2BiMap;

    auto V1 = vcl::vertexPositionsMatrix<EigenMatrixX3m>(m1);
    auto F1 =
        vcl::triangulatedFaceVertexIndicesMatrix<Eigen::MatrixX3i>(m1, m1BiMap);

    auto V2 = vcl::vertexPositionsMatrix<EigenMatrixX3m>(m2);
    auto F2 =
        vcl::triangulatedFaceVertexIndicesMatrix<Eigen::MatrixX3i>(m2, m2BiMap);

    EigenMatrixX3m   VR;
    Eigen::MatrixX3i FR;
    Eigen::VectorXi  indices; // mapping indices for birth faces

    bool result = ::igl::copyleft::cgal::mesh_boolean(
        V1,
        F1,
        V2,
        F2,
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

    // TODO: before returning, we should post-process the output mesh to
    // transfer the attributes (e.g., vertex colors, face colors, etc.) from the
    // input meshes to the output mesh.

    return vcl::meshFromMatrices<MeshType>(VR, FR);
}

} // namespace vcl::igl

#endif // VCLIB_WITH_CGAL

#endif // VCL_IGL_BOOLEANS_H
