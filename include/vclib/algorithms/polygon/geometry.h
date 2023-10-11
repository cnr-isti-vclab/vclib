/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_ALGORITHMS_POLYGON_GEOMETRY_H
#define VCL_ALGORITHMS_POLYGON_GEOMETRY_H

#include <vclib/mesh/requirements.h>
#include <vclib/space/polygon.h>
#include <vclib/views/mesh.h>

namespace vcl {

/**
 * @brief Computes the normal of a face, without modifying the face. Works both
 * for triangle and polygonal faces, and it is optimized in case of triangle
 * faces.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: the input face.
 * @return the normal of the face.
 */
template<FaceConcept FaceType>
typename FaceType::VertexType::CoordType faceNormal(const FaceType& f)
{
    using CoordType = FaceType::VertexType::CoordType;
    if constexpr (TriangleFaceConcept<FaceType>) {
        return Triangle<CoordType>::normal(
            f.vertex(0)->coord(), f.vertex(1)->coord(), f.vertex(2)->coord());
    }
    else {
        if (f.vertexNumber() == 3) {
            return Triangle<CoordType>::normal(
                f.vertex(0)->coord(),
                f.vertex(1)->coord(),
                f.vertex(2)->coord());
        }
        else {
            return Polygon<CoordType>::normal(f.vertices() | views::coords);
        }
    }
}

/**
 * @brief Computes the barycenter of a face. Works both for triangle and
 * polygonal faces, and it is optimized in case of triangle faces.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: the input face.
 * @return the barycenter of the face.
 */
template<FaceConcept FaceType>
typename FaceType::VertexType::CoordType faceBarycenter(const FaceType& f)
{
    using CoordType = FaceType::VertexType::CoordType;
    if constexpr (TriangleFaceConcept<FaceType>) {
        return Triangle<CoordType>::barycenter(
            f.vertex(0)->coord(), f.vertex(1)->coord(), f.vertex(2)->coord());
    }
    else {
        return Polygon<CoordType>::barycenter(f.vertices() | views::coords);
    }
}

/**
 * @brief Computes the area of a face. Works both for triangle and polygonal
 * faces, and it is optimized in case of triangle faces.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: the input face.
 * @return the area of the face.
 */
template<FaceConcept FaceType>
auto faceArea(const FaceType& f)
{
    using CoordType = FaceType::VertexType::CoordType;
    if constexpr (TriangleFaceConcept<FaceType>) {
        return Triangle<CoordType>::area(
            f.vertex(0)->coord(), f.vertex(1)->coord(), f.vertex(2)->coord());
    }
    else {
        if (f.vertexNumber() == 3) {
            return Triangle<CoordType>::area(
                f.vertex(0)->coord(), f.vertex(1)->coord(), f.vertex(2)->coord());
        }
        else {
            return Polygon<CoordType>::area(f.vertices() | views::coords);
        }
    }
}

/**
 * @brief Computes the perimeter of a face. Works both for triangle and
 * polygonal faces, and it is optimized in case of triangle faces.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: the input face.
 * @return the perimeter of the face.
 */
template<FaceConcept FaceType>
auto facePerimeter(const FaceType& f)
{
    using CoordType = FaceType::VertexType::CoordType;
    if constexpr (TriangleFaceConcept<FaceType>) {
        return Triangle<CoordType>::perimeter(
            f.vertex(0)->coord(), f.vertex(1)->coord(), f.vertex(2)->coord());
    }
    else {
        if (f.vertexNumber() == 3) {
            return Triangle<CoordType>::perimeter(
                f.vertex(0)->coord(),
                f.vertex(1)->coord(),
                f.vertex(2)->coord());
        }
        else {
            return Polygon<CoordType>::perimeter(f.vertices() | views::coords);
        }
    }
}

/**
 * @brief Returns the internal angle (in radians) of the vi-th vertex of the
 * face.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: the input face.
 * @param[in] vi: the index of the vertex in the face on which calculate the
 * angle
 * @return the angle in radians at the vi-th vertex.
 */
template<FaceConcept FaceType>
auto faceAngleOnVertexRad(const FaceType& f, uint vi)
{
    const auto& p0 = f.vertex(vi)->coord();
    const auto& p1 = f.vertexMod((int)vi+1)->coord();
    const auto& p2 = f.vertexMod((int)vi-1)->coord();
    return (p2 - p0).angle(p1 - p0);
}

/**
 * * @brief Compute the signed dihedral angle between the normals of the given
 * face and its adjacent face on the edge \p e.
 *
 * The angle between the normal is signed according to the concavity/convexity
 * of the dihedral angle: negative if the edge shared between the two faces is
 * concave, positive otherwise. The surface it is assumend to be oriented. It
 * simply use the projection of  the opposite vertex onto the plane of the other
 * one. It does not assume anything on face normals.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f The face for which to compute the dihedral angle on an edge.
 * @param[in] e The index of the edge shared between the two faces.
 * @return The signed dihedral angle between the normals of the given face and
 * its adjacent face on the edge e.
 * @throws vcl::MissingComponentException If the "AdjacentFaces" component is
 * not available on \p f.
 */
template<FaceConcept FaceType>
auto faceDihedralAngleOnEdge(const FaceType& f, uint e)
    requires comp::HasAdjacentFaces<FaceType>
{
    if (! comp::isAdjacentFacesAvailableOn(f)) {
        throw vcl::MissingComponentException(
            "Face has no Adjacent Faces component.");
    }

    /*
     *     v0 ___________ vf1
     *       |\          |
     *       | e\     f1 |
     *       |    \e1    |
     *       |f     \    |
     *       |        \  |
     *       |__________\|
     *    vf0             v1
     */

    assert(f.adjFace(e) != nullptr);
    const FaceType& f1 = *(f.adjFace(e));

    uint e1 = f1.indexOfAdjFace(&f);
    assert(e1 != UINT_NULL);

    const auto& vf0 = *(f.vertexMod((int) e - 1));
    const auto& vf1 = *(f1.vertexMod(e1 - 1));

    auto n0 = faceNormal(f);
    auto n1 = faceNormal(f1);

    auto off0 = n0 * vf0.coord();
    auto off1 = n1 * vf1.coord();

    auto dist01 = off0 - n0 * vf1.coord();
    auto dist10 = off1 - n1 * vf0.coord();

    auto sign = std::abs(dist01) > std::abs(dist10) ? dist01 : dist10;

    auto angleRad = n0.angle(n1);
    return sign > 0 ? angleRad : -angleRad;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_POLYGON_GEOMETRY_H
