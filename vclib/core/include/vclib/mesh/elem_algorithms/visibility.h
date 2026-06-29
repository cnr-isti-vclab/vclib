// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_ELEM_ALGORITHMS_VISIBILITY_H
#define VCL_MESH_ELEM_ALGORITHMS_VISIBILITY_H

#include <vclib/mesh/elements.h>

#include <vclib/algorithms/core.h>

namespace vcl {

/**
 * @brief Compute the determinant of the half-space defined by the triangle
 * and the point.
 *
 * @tparam FaceType: The type of the face that defines the half-space.
 * @tparam PointType: The type of the point to test.
 *
 * @param[in] face: The face that defines the half-space.
 * @param[in] point: The point to test.
 * @return The determinant of the half-space.
 *
 * @ingroup algorithms_core
 */
template<FaceConcept FaceType, Point3Concept PointType>
auto halfSpaceDeterminant(const FaceType& face, const PointType& point)
{
    // - if it is a triangle, compute the determinant using the triangle
    // - if it is a polygon, use its normal to compute the determinant
    if constexpr (TriangleFaceConcept<FaceType>) {
        return halfSpaceDeterminant(
            face.vertex(0)->position(),
            face.vertex(1)->position(),
            face.vertex(2)->position(),
            point);
    }
    else {
        if (face.vertexCount() == 3) {
            return halfSpaceDeterminant(
                face.vertex(0)->position(),
                face.vertex(1)->position(),
                face.vertex(2)->position(),
                point);
        }
        else {
            PointType n = faceNormal(face);
            return n.dot(point - face.vertex(0)->position());
        }
    }
}

/**
 * @brief Checks if a point is visible from a face, i.e., if the point is in the
 * half-space defined by the face.
 *
 * @tparam FaceType: The type of the face.
 * @tparam PointType: The type of the point.
 * @param[in] face: The input face.
 * @param[in] point: The point to test.
 * @return true if the point is visible from the face, false otherwise.
 *
 * @ingroup algorithms_core
 */
template<FaceConcept FaceType, Point3Concept PointType>
bool facePointVisibility(const FaceType& face, const PointType& point)
{
    return halfSpaceDeterminant(face, point) > 0;
}

} // namespace vcl

#endif // VCL_MESH_ELEM_ALGORITHMS_VISIBILITY_H
