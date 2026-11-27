// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_UPDATE_GAUSSIAN_SPLAT_H
#define VCL_ALGORITHMS_MESH_UPDATE_GAUSSIAN_SPLAT_H

#include <vclib/mesh.h>

namespace vcl {

/**
 * @brief Sets the spherical harmonics degree of all the vertices' Gaussian
 * Splat components in the mesh, resizing each vertex's SH coefficient vector
 * accordingly.
 *
 * If `degree == UINT_NULL`, all vertices' SH coefficient vectors are cleared
 * and their degree is set to `UINT_NULL`. Otherwise, each vertex's SH
 * coefficient vector is resized to `SH_COLOR * (degree + 1)^2` (zero-padding
 * newly added coefficients or truncating excess ones).
 *
 * @note This function operates on the vertices currently in the mesh; vertices
 * added later will keep the default uninitialized SH state (`UINT_NULL`) until
 * this function is called again.
 *
 * @throws vcl::MissingComponentException if the mesh does not have the per
 * vertex GaussianSplat component available.
 *
 * @tparam MeshType: type of the input mesh. It must satisfy the MeshConcept.
 *
 * @param[in,out] mesh: the mesh on which set the vertex Gaussian Splat SH
 * degree.
 * @param[in] degree: the SH degree to set (or `UINT_NULL` to clear SH
 * coefficients).
 *
 * @ingroup update
 */
template<MeshConcept MeshType>
requires HasVertexGaussianSplat<MeshType>
void setVertexGaussianSplatSHDegree(MeshType& mesh, uint degree)
{
    requireVertexGaussianSplat(mesh);

    using GSType = typename VertexType<MeshType>::GaussianSplatType;

    const uint shCoeffCount =
        (degree == UINT_NULL) ? 0u :
                                GSType::SH_COLOR * (degree + 1) * (degree + 1);

    for (auto&& gs : mesh.vertices() | views::gaussianSplats) {
        gs.resizeSHCoeffs(shCoeffCount);
    }
}

/**
 * @brief Uniforms the spherical harmonics degree across all the vertices'
 * Gaussian Splat components in the mesh to the maximum non-null SH degree
 * present among the vertices.
 *
 * Vertices with a smaller SH degree (or uninitialized SH coefficients,
 * `UINT_NULL`) are resized and zero-padded to match the maximum degree. If all
 * vertices have `shDegree() == UINT_NULL` (or the mesh has no vertices), the
 * mesh vertices remain unchanged and `UINT_NULL` is returned.
 *
 * @note This function operates on the vertices currently in the mesh; vertices
 * added later will keep the default uninitialized SH state (`UINT_NULL`) until
 * this function is called again.
 *
 * @throws vcl::MissingComponentException if the mesh does not have the per
 * vertex GaussianSplat component available.
 *
 * @tparam MeshType: type of the input mesh. It must satisfy the MeshConcept.
 *
 * @param[in,out] mesh: the mesh on which uniform the vertex Gaussian Splat SH
 * degree.
 * @return the uniform SH degree applied across the mesh, or `UINT_NULL` if no
 * vertex has SH coefficients.
 *
 * @ingroup update
 */
template<MeshConcept MeshType>
requires HasVertexGaussianSplat<MeshType>
uint uniformVertexGaussianSplatSHDegree(MeshType& mesh)
{
    requireVertexGaussianSplat(mesh);

    uint maxDegree = UINT_NULL;
    for (const auto& gs : mesh.vertices() | views::gaussianSplats) {
        const uint d = gs.shDegree();
        if (d != UINT_NULL && (maxDegree == UINT_NULL || d > maxDegree)) {
            maxDegree = d;
        }
    }

    setVertexGaussianSplatSHDegree(mesh, maxDegree);
    return maxDegree;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_UPDATE_GAUSSIAN_SPLAT_H
