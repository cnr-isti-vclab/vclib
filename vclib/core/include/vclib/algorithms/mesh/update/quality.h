// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_UPDATE_QUALITY_H
#define VCL_ALGORITHMS_MESH_UPDATE_QUALITY_H

#include <vclib/algorithms/mesh/stat.h>

#include <vclib/algorithms/core.h>
#include <vclib/mesh.h>

namespace vcl {

/**
 * @brief Sets a constant value to all the quality of the elements having
 * ELEM_ID in the mesh.
 *
 * @param[in, out] mesh: the mesh on which set the element quality
 * @param[in] quality: quality value to set
 */
template<ElemId ELEM_ID, MeshConcept MeshType>
void setElementQuality(MeshType& mesh, auto quality)
{
    requireElementComponent<ELEM_ID, CompId::QUALITY>(mesh);

    std::ranges::fill(
        mesh.template elements<ELEM_ID>() | vcl::views::quality, quality);
}

/**
 * @brief Clamps the quality of the elements having ELEM_ID in the mesh in a
 * given interval.
 *
 * @param[in,out] mesh: the mesh on which clamp the element quality
 * @param[in] minQ: minimum value of the clamping interval
 * @param[in] maxQ: maximum value of the clamping interval
 */
template<
    ElemId      ELEM_ID,
    MeshConcept MeshType,
    typename Q1 = float,
    typename Q2 = float>
void clampElementQuality(MeshType& mesh, Q1 minQ = 0.f, Q2 maxQ = 1.f)
{
    requireElementComponent<ELEM_ID, CompId::QUALITY>(mesh);

    for (auto&& q : mesh.template elements<ELEM_ID>() | vcl::views::quality) {
        using QType = std::remove_cvref_t<decltype(q)>;
        q           = std::clamp<QType>(q, minQ, maxQ);
    }
}

/**
 * @brief Normalizes the quality of the elements having ELEM_ID in the mesh in a
 * given interval (default [0, 1]).
 *
 * @param[in,out] mesh: mesh on which normalize the element quality
 * @param[in] minQ: minimum value of the normalizing interval
 * @param[in] maxQ: maximum value of the normalizing interval
 */
template<
    ElemId      ELEM_ID,
    MeshConcept MeshType,
    typename Q1 = float,
    typename Q2 = float>
void normalizeElementQuality(MeshType& mesh, Q1 minQ = 0.f, Q2 maxQ = 1.f)
{
    requireElementComponent<ELEM_ID, CompId::QUALITY>(mesh);

    auto range          = maxQ - minQ;
    auto [eMinQ, eMaxQ] = elementQualityMinMax<ELEM_ID>(mesh);

    for (auto&& q : mesh.template elements<ELEM_ID>() | vcl::views::quality) {
        q = minQ + range * ((q - eMinQ) / (eMaxQ - eMinQ));
    }
}

/**
 * @brief Sets a constant value to all the quality of the vertices in the mesh.
 *
 * @param[in, out] mesh: the mesh on which set the vertex quality
 * @param[in] quality: quality value to set
 */
template<MeshConcept MeshType, typename QualityType>
void setVertexQuality(MeshType& mesh, QualityType quality)
{
    setElementQuality<ElemId::VERTEX>(mesh, quality);
}

/**
 * @brief Sets a constant value to all the quality of the faces in the mesh.
 *
 * @param[in, out] mesh: the mesh on which set the face quality
 * @param[in] quality: quality value to set
 */
template<FaceMeshConcept MeshType>
void setFaceQuality(MeshType& mesh, auto quality)
{
    setElementQuality<ElemId::FACE>(mesh, quality);
}

/**
 * @brief Clamps the quality of the vertices in the mesh in a given interval.
 *
 * @param[in,out] mesh: the mesh on which clamp the vertex quality
 * @param[in] minQ: minimum value of the clamping interval
 * @param[in] maxQ: maximum value of the clamping interval
 */
template<MeshConcept MeshType, typename Q1 = float, typename Q2 = float>
void clampVertexQuality(MeshType& mesh, Q1 minQ = 0.f, Q2 maxQ = 1.f)
{
    clampElementQuality<ElemId::VERTEX>(mesh, minQ, maxQ);
}

/**
 * @brief Clamps the quality of the faces in the mesh in a given interval.
 *
 * @param[in,out] mesh: the mesh on which clamp the face quality
 * @param[in] minQ: minimum value of the clamping interval
 * @param[in] maxQ: maximum value of the clamping interval
 */
template<FaceMeshConcept MeshType, typename Q1 = float, typename Q2 = float>
void clampFaceQuality(MeshType& mesh, Q1 minQ = 0.f, Q2 maxQ = 1.f)
{
    clampElementQuality<ElemId::FACE>(mesh, minQ, maxQ);
}

/**
 * @brief Normalizes the quality of the vertices in the mesh in a given interval
 * (default [0, 1]).
 *
 * @param[in,out] mesh: mesh on which normalize the vertex quality
 * @param[in] minQ: minimum value of the normalizing interval
 * @param[in] maxQ: maximum value of the normalizing interval
 */
template<MeshConcept MeshType, typename Q1 = float, typename Q2 = float>
void normalizeVertexQuality(MeshType& mesh, Q1 minQ = 0.f, Q2 maxQ = 1.f)
{
    normalizeElementQuality<ElemId::VERTEX>(mesh, minQ, maxQ);
}

/**
 * @brief Normalizes the quality of the faces in the mesh in a given interval
 * (default [0, 1]).
 *
 * @param[in,out] mesh: mesh on which normalize the face quality
 * @param[in] minQ: minimum value of the normalizing interval
 * @param[in] maxQ: maximum value of the normalizing interval
 */
template<FaceMeshConcept MeshType, typename Q1 = float, typename Q2 = float>
void normalizeFaceQuality(MeshType& mesh, Q1 minQ = 0.f, Q2 maxQ = 1.f)
{
    normalizeElementQuality<ElemId::FACE>(mesh, minQ, maxQ);
}

/**
 * @brief Assign to the vertex quality of the mesh the valence of each vertex,
 * that is the number of adjacent faces of the vertex.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Quality
 *   - Faces
 *
 * @param[in,out] m: mesh on which set the vertex quality to the vertex valence
 */
template<FaceMeshConcept MeshType>
void setVertexQualityFromVertexValence(MeshType& m)
{
    requireVertexQuality(m);

    setVertexQuality(m, 0);

    for (auto f : m.faces()) {
        for (auto v : f.vertices()) {
            v.quality() += 1;
        }
    }
}

/**
 * @brief Assign to the face quality of the mesh the area of each face.
 *
 * Requirements:
 * - Mesh:
 *   - Faces
 *     - Quality
 *
 * @param[in,out] m: mesh on which set the face quality to the face area
 */
template<FaceMeshConcept MeshType>
void setFaceQualityFromFaceArea(MeshType& m)
{
    requireFaceQuality(m);

    for (auto f : m.faces()) {
        f.quality() = faceArea(f);
    }
}

template<MeshConcept MeshType>
void setVertexQualityFromPrincipalCurvatureGaussian(MeshType& m)
{
    requireVertexQuality(m);
    requireVertexPrincipalCurvature(m);

    for (auto v : m.vertices()) {
        v.quality() = v.principalCurvature().maxValue() *
                      v.principalCurvature().minValue();
    }
}

template<MeshConcept MeshType>
void setVertexQualityFromPrincipalCurvatureMean(MeshType& m)
{
    requireVertexQuality(m);
    requireVertexPrincipalCurvature(m);

    for (auto v : m.vertices()) {
        v.quality() = (v.principalCurvature().maxValue() +
                       v.principalCurvature().minValue()) /
                      2;
    }
}

template<MeshConcept MeshType>
void setVertexQualityFromPrincipalCurvatureMinValue(MeshType& m)
{
    requireVertexQuality(m);
    requireVertexPrincipalCurvature(m);

    for (auto v : m.vertices()) {
        v.quality() = v.principalCurvature().minValue();
    }
}

template<MeshConcept MeshType>
void setVertexQualityFromPrincipalCurvatureMaxValue(MeshType& m)
{
    requireVertexQuality(m);
    requireVertexPrincipalCurvature(m);

    for (auto v : m.vertices()) {
        v.quality() = v.principalCurvature().maxValue();
    }
}

/**
 * @brief Computes the Shape Index S from the Principal Curvature, as defined by
 * [Koenderink 1992] and stores it in the per-vertex quality.
 *
 * S = 2/pi atan(k1+k2/k1-k2)
 *
 * J. Koenderink and A. van Doorn.
 * Surface shape and curvature scales.
 * Image and vision computing, 10(8):557–565, 1992.
 * @param m
 */
template<MeshConcept MeshType>
void setVertexQualityFromPrincipalCurvatureShapeIndex(MeshType& m)
{
    requireVertexQuality(m);
    requireVertexPrincipalCurvature(m);

    for (auto v : m.vertices()) {
        auto k1 = v.principalCurvature().maxValue();
        auto k2 = v.principalCurvature().minValue();
        if (k1 < k2)
            std::swap(k1, k2);
        v.quality() = (2.0 / M_PI) * std::atan2(k1 + k2, k1 - k2);
    }
}

/**
 * @brief Computes the Curvedness C from the Principal Curvature, as defined by
 * [Koenderink 1992] and stores it in the per-vertex quality.
 *
 * C =  Sqrt((k1*k1+k2*k2)/2.0)
 *
 * J. Koenderink and A. van Doorn.
 * Surface shape and curvature scales.
 * Image and vision computing, 10(8):557–565, 1992.
 * @param m
 */
template<MeshConcept MeshType>
void setVertexQualityFromPrincipalCurvatureCurvedness(MeshType& m)
{
    requireVertexQuality(m);
    requireVertexPrincipalCurvature(m);

    for (auto v : m.vertices()) {
        auto k1 = v.principalCurvature().maxValue();
        auto k2 = v.principalCurvature().minValue();

        v.quality() = std::sqrt((k1 * k1 + k2 * k2) / 2.0);
    }
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_UPDATE_QUALITY_H
