/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#include <vclib/algorithms/core/polygon.h>
#include <vclib/algorithms/mesh/stat.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

/**
 * @brief Sets a constant value to all the vertex quality of the mesh.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Quality
 *
 * @param[in,out] m: mesh on which set the vertex quality
 * @param[in] s: quality value to set
 */
template<MeshConcept MeshType>
void setPerVertexQuality(
    MeshType&                                  m,
    typename MeshType::VertexType::QualityType s)
{
    vcl::requirePerVertexQuality(m);

    using VertexType = MeshType::VertexType;

    for (VertexType& v : m.vertices()) {
        v.quality() = s;
    }
}

/**
 * @brief Sets a constant value to all the face quality of the mesh.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Quality
 *
 * @param[in,out] m: mesh on which set the face quality
 * @param[in] s: quality value to set
 */
template<FaceMeshConcept MeshType>
void setPerFaceQuality(MeshType& m, typename MeshType::FaceType::QualityType s)
{
    vcl::requirePerFaceQuality(m);

    using FaceType = MeshType::FaceType;

    for (FaceType& f : m.faces()) {
        f.quality() = s;
    }
}

/**
 * @brief Clamps the vertex quality of a mesh in a given interval.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Quality
 *
 * @param[in,out] m: mesh on which clamp the vertex quality
 * @param[in] minS: minimum value of the clamping interval
 * @param[in] maxS: maximum value of the clamping interval
 */
template<MeshConcept MeshType>
void clampPerVertexQuality(
    MeshType&                                  m,
    typename MeshType::VertexType::QualityType minS,
    typename MeshType::VertexType::QualityType maxS)
{
    vcl::requirePerVertexQuality(m);

    using VertexType = MeshType::VertexType;

    for (VertexType& v : m.vertices()) {
        v.quality() = std::min(maxS, std::max(minS, v.quality()));
    }
}

/**
 * @brief Clamps the face quality of a mesh in a given interval.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Quality
 *
 * @param[in,out] m: mesh on which clamp the face quality
 * @param[in] minS: minimum value of the clamping interval
 * @param[in] maxS: maximum value of the clamping interval
 */
template<FaceMeshConcept MeshType>
void clampPerFaceQuality(
    MeshType&                                m,
    typename MeshType::FaceType::QualityType minS,
    typename MeshType::FaceType::QualityType maxS)
{
    vcl::requirePerFaceQuality(m);

    using FaceType = MeshType::FaceType;

    for (FaceType& f : m.faces()) {
        f.quality() = std::min(maxS, std::max(minS, f.quality()));
    }
}

/**
 * @brief Normalizes the vertex quality of a mesh in a given interval (default
 * [0, 1]).
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Quality
 *
 * @param[in,out] m: mesh on which normalize the vertex quality
 * @param[in] minS: minimum value of the normalizing interval
 * @param[in] maxS: maximum value of the normalizing interval
 */
template<MeshConcept MeshType>
void normalizePerVertexQuality(
    MeshType&                                  m,
    typename MeshType::VertexType::QualityType minS = 0,
    typename MeshType::VertexType::QualityType maxS = 1)
{
    vcl::requirePerVertexQuality(m);

    using VertexType  = MeshType::VertexType;
    using QualityType = VertexType::QualityType;

    QualityType                         range = maxS - minS;
    std::pair<QualityType, QualityType> p     = vertexQualityMinMax(m);

    for (VertexType& v : m.vertices()) {
        v.quality() =
            minS + range * ((v.quality() - p.first) / (p.second - p.first));
    }
}

/**
 * @brief Normalizes the face quality of a mesh in a given interval (default [0,
 * 1]).
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Quality
 *
 * @param[in,out] m: mesh on which normalize the face quality
 * @param[in] minS: minimum value of the normalizing interval
 * @param[in] maxS: maximum value of the normalizing interval
 */
template<FaceMeshConcept MeshType>
void normalizePerFaceQuality(
    MeshType&                                m,
    typename MeshType::FaceType::QualityType minS = 0,
    typename MeshType::FaceType::QualityType maxS = 1)
{
    vcl::requirePerFaceQuality(m);

    using FaceType    = MeshType::FaceType;
    using QualityType = FaceType::QualityType;

    QualityType                         range = maxS - minS;
    std::pair<QualityType, QualityType> p     = faceQualityMinMax(m);

    for (FaceType& f : m.faces()) {
        f.quality() =
            minS + range * ((f.quality() - p.first) / (p.second - p.first));
    }
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
    vcl::requirePerVertexQuality(m);

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
    vcl::requirePerFaceQuality(m);

    using FaceType = MeshType::FaceType;

    for (FaceType& f : m.faces()) {
        f.quality() = faceArea(f);
    }
}

template<MeshConcept MeshType>
void setPerVertexQualityFromPrincipalCurvatureGaussian(MeshType& m)
{
    vcl::requirePerVertexQuality(m);
    vcl::requirePerVertexPrincipalCurvature(m);

    using VertexType = MeshType::VertexType;

    for (VertexType& v : m.vertices()) {
        v.quality() = v.principalCurvature().maxValue() *
                      v.principalCurvature().minValue();
    }
}

template<MeshConcept MeshType>
void setPerVertexQualityFromPrincipalCurvatureMean(MeshType& m)
{
    vcl::requirePerVertexQuality(m);
    vcl::requirePerVertexPrincipalCurvature(m);

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
    vcl::requirePerVertexQuality(m);
    vcl::requirePerVertexPrincipalCurvature(m);

    using VertexType = MeshType::VertexType;

    for (VertexType& v : m.vertices()) {
        v.quality() = v.principalCurvature().minValue();
    }
}

template<MeshConcept MeshType>
void setPerVertexQualityFromPrincipalCurvatureMaxValue(MeshType& m)
{
    vcl::requirePerVertexQuality(m);
    vcl::requirePerVertexPrincipalCurvature(m);

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
    vcl::requirePerVertexQuality(m);
    vcl::requirePerVertexPrincipalCurvature(m);

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
    vcl::requirePerVertexQuality(m);
    vcl::requirePerVertexPrincipalCurvature(m);

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
