/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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
template<uint ELEM_ID, MeshConcept MeshType, typename QualityType>
void setPerElementQuality(MeshType& mesh, QualityType quality)
{
    requirePerElementComponent<ELEM_ID, CompId::QUALITY>(mesh);

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
template<uint ELEM_ID, MeshConcept MeshType, typename QualityType>
void clampPerElementQuality(MeshType& mesh, QualityType minQ, QualityType maxQ)
{
    requirePerElementComponent<ELEM_ID, CompId::QUALITY>(mesh);

    for (auto&& q : mesh.template elements<ELEM_ID>() | vcl::views::quality) {
        q = std::clamp(q, minQ, maxQ);
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
template<uint ELEM_ID, MeshConcept MeshType, typename QualityType>
void normalizePerElementQuality(
    MeshType&   mesh,
    QualityType minQ = 0,
    QualityType maxQ = 1)
{
    requirePerElementComponent<ELEM_ID, CompId::QUALITY>(mesh);

    QualityType range   = maxQ - minQ;
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
void setPerVertexQuality(MeshType& mesh, QualityType quality)
{
    setPerElementQuality<ElemId::VERTEX>(mesh, quality);
}

/**
 * @brief Sets a constant value to all the quality of the faces in the mesh.
 *
 * @param[in, out] mesh: the mesh on which set the face quality
 * @param[in] quality: quality value to set
 */
template<FaceMeshConcept MeshType, typename QualityType>
void setPerFaceQuality(MeshType& mesh, QualityType quality)
{
    setPerElementQuality<ElemId::FACE>(mesh, quality);
}

/**
 * @brief Clamps the quality of the vertices in the mesh in a given interval.
 *
 * @param[in,out] mesh: the mesh on which clamp the vertex quality
 * @param[in] minQ: minimum value of the clamping interval
 * @param[in] maxQ: maximum value of the clamping interval
 */
template<MeshConcept MeshType, typename QualityType>
void clampPerVertexQuality(MeshType& mesh, QualityType minQ, QualityType maxQ)
{
    clampPerElementQuality<ElemId::VERTEX>(mesh, minQ, maxQ);
}

/**
 * @brief Clamps the quality of the faces in the mesh in a given interval.
 *
 * @param[in,out] mesh: the mesh on which clamp the face quality
 * @param[in] minQ: minimum value of the clamping interval
 * @param[in] maxQ: maximum value of the clamping interval
 */
template<FaceMeshConcept MeshType, typename QualityType>
void clampPerFaceQuality(MeshType& mesh, QualityType minQ, QualityType maxQ)
{
    clampPerElementQuality<ElemId::FACE>(mesh, minQ, maxQ);
}

/**
 * @brief Normalizes the quality of the vertices in the mesh in a given interval
 * (default [0, 1]).
 *
 * @param[in,out] mesh: mesh on which normalize the vertex quality
 * @param[in] minQ: minimum value of the normalizing interval
 * @param[in] maxQ: maximum value of the normalizing interval
 */
template<MeshConcept MeshType, typename QualityType>
void normalizePerVertexQuality(
    MeshType&   mesh,
    QualityType minQ = 0,
    QualityType maxQ = 1)
{
    normalizePerElementQuality<ElemId::VERTEX>(mesh, minQ, maxQ);
}

/**
 * @brief Normalizes the quality of the faces in the mesh in a given interval
 * (default [0, 1]).
 *
 * @param[in,out] mesh: mesh on which normalize the face quality
 * @param[in] minQ: minimum value of the normalizing interval
 * @param[in] maxQ: maximum value of the normalizing interval
 */
template<FaceMeshConcept MeshType, typename QualityType>
void normalizePerFaceQuality(
    MeshType&   mesh,
    QualityType minQ = 0,
    QualityType maxQ = 1)
{
    normalizePerElementQuality<ElemId::FACE>(mesh, minQ, maxQ);
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
void setPerVertexQualityFromVertexValence(MeshType& m)
{
    requirePerVertexQuality(m);

    using VertexType = MeshType::VertexType;
    using FaceType   = MeshType::FaceType;

    setPerVertexQuality(m, 0);

    for (FaceType& f : m.faces()) {
        for (VertexType* v : f.vertices()) {
            v->quality() += 1;
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
void setPerFaceQualityFromFaceArea(MeshType& m)
{
    requirePerFaceQuality(m);

    using FaceType = MeshType::FaceType;

    for (FaceType& f : m.faces()) {
        f.quality() = faceArea(f);
    }
}

template<MeshConcept MeshType>
void setPerVertexQualityFromPrincipalCurvatureGaussian(MeshType& m)
{
    requirePerVertexQuality(m);
    requirePerVertexPrincipalCurvature(m);

    using VertexType = MeshType::VertexType;

    for (VertexType& v : m.vertices()) {
        v.quality() = v.principalCurvature().maxValue() *
                      v.principalCurvature().minValue();
    }
}

template<MeshConcept MeshType>
void setPerVertexQualityFromPrincipalCurvatureMean(MeshType& m)
{
    requirePerVertexQuality(m);
    requirePerVertexPrincipalCurvature(m);

    using VertexType = MeshType::VertexType;

    for (VertexType& v : m.vertices()) {
        v.quality() = (v.principalCurvature().maxValue() +
                       v.principalCurvature().minValue()) /
                      2;
    }
}

template<MeshConcept MeshType>
void setPerVertexQualityFromPrincipalCurvatureMinValue(MeshType& m)
{
    requirePerVertexQuality(m);
    requirePerVertexPrincipalCurvature(m);

    using VertexType = MeshType::VertexType;

    for (VertexType& v : m.vertices()) {
        v.quality() = v.principalCurvature().minValue();
    }
}

template<MeshConcept MeshType>
void setPerVertexQualityFromPrincipalCurvatureMaxValue(MeshType& m)
{
    requirePerVertexQuality(m);
    requirePerVertexPrincipalCurvature(m);

    using VertexType = MeshType::VertexType;

    for (VertexType& v : m.vertices()) {
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
void setPerVertexQualityFromPrincipalCurvatureShapeIndex(MeshType& m)
{
    requirePerVertexQuality(m);
    requirePerVertexPrincipalCurvature(m);

    using VertexType = MeshType::VertexType;
    using ScalarType = VertexType::PrincipalCurvatureType::ScalarType;

    for (VertexType& v : m.vertices()) {
        ScalarType k1 = v.principalCurvature().maxValue();
        ScalarType k2 = v.principalCurvature().minValue();
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
void setPerVertexQualityFromPrincipalCurvatureCurvedness(MeshType& m)
{
    requirePerVertexQuality(m);
    requirePerVertexPrincipalCurvature(m);

    using VertexType = MeshType::VertexType;
    using ScalarType = VertexType::PrincipalCurvatureType::ScalarType;

    for (VertexType& v : m.vertices()) {
        ScalarType k1 = v.principalCurvature().maxValue();
        ScalarType k2 = v.principalCurvature().minValue();

        v.quality() = std::sqrt((k1 * k1 + k2 * k2) / 2.0);
    }
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_UPDATE_QUALITY_H
