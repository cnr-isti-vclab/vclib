/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_IGL_BOOLEANS_H
#define VCL_IGL_BOOLEANS_H

#ifdef VCLIB_WITH_CGAL

#include <vclib/algorithms/mesh.h>
#include <vclib/mesh.h>

#include <igl/copyleft/cgal/CSGTree.h>

namespace vcl::igl {

/**
 * @brief Perform boolean operations between two meshes using libigl's
 * CGAL-based implementation.
 *
 * @param[in] m1: First input mesh.
 * @param[in] m2: Second input mesh.
 * @param[in] op: The type of boolean operation to perform (union, intersection,
 * difference).
 * @return The resulting mesh after the boolean operation.
 *
 * @throws std::runtime_error if the input meshes do not induce a piecewise
 * constant winding number field (i.e., if they are not watertight).
 */
template<FaceMeshConcept MeshType>
MeshType meshBoolean(
    const MeshType& m1,
    const MeshType& m2,
    ::igl::MeshBooleanType op)
{
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
        V1, F1, V2, F2, op, VR, FR, indices);

    if (!result) {
        throw std::runtime_error(
            "Mesh inputs must induce a piecewise constant winding number "
            "field. Make sure that both the input mesh are watertight "
            "(closed).");
    }
    return vcl::meshFromMatrices<MeshType>(VR, FR);
}

} // namespace vcl::igl

#endif // VCLIB_WITH_CGAL

#endif // VCL_IGL_BOOLEANS_H
