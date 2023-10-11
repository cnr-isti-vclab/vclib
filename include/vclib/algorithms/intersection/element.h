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

#ifndef VCL_ALGORITHMS_INTERSECTION_ELEMENT_H
#define VCL_ALGORITHMS_INTERSECTION_ELEMENT_H

#include <vclib/algorithms/polygon.h>
#include <vclib/mesh/requirements.h>
#include <vclib/space/box.h>
#include <vclib/space/sphere.h>
#include <vclib/space/triangle_wrapper.h>

#include "misc.h"

namespace vcl {

template<FaceConcept FaceType, PointConcept PointType>
bool faceBoxIntersect(const FaceType& f, const Box<PointType>& box)
{
    if constexpr (TriangleFaceConcept<FaceType>) {
        return triangleBoxIntersect(
            TriangleWrapper(
                f.vertex(0)->coord(),
                f.vertex(1)->coord(),
                f.vertex(2)->coord()),
            box);
    }
    else {
        bool b = false;

        std::vector<uint> tris = vcl::earCut(f);
        for (uint i = 0; i < tris.size() && !b; i += 3) {
            b |= triangleBoxIntersect(
                TriangleWrapper(
                    f.vertex(tris[i])->coord(),
                    f.vertex(tris[i + 1])->coord(),
                    f.vertex(tris[i + 2])->coord()),
                box);
        }
        return b;
    }
}

/**
 * @brief Compute the intersection between a sphere and a face, that may be also
 * polygonal.
 *
 * If the face is a triangle, the triangleSphereIntersect function will be used.
 * If the face is polygonal, the face is first triangulated using an earcut
 * algorithm, and then for each triangle, the triangleSphereIntersect is
 * computed.
 *
 * @param[in] f: the input face
 * @param[in] sphere: the input sphere
 * @param[out] witness: the point on the triangle nearest to the center of the
 * sphere (even when there isn't intersection)
 * @param[out] res: if not null, in the first item is stored the minimum
 * distance between the face and the sphere, while in the second item is stored
 * the penetration depth
 * @return true iff there is an intersection between the sphere and the face
 */
template<FaceConcept FaceType, PointConcept PointType, typename SScalar>
bool faceSphereItersect(
    const FaceType&              f,
    const Sphere<SScalar>&       sphere,
    PointType&                   witness,
    std::pair<SScalar, SScalar>& res)
{
    if constexpr (TriangleFaceConcept<FaceType>) {
        return triangleSphereItersect(
            TriangleWrapper(
                f.vertex(0)->coord(),
                f.vertex(1)->coord(),
                f.vertex(2)->coord()),
            sphere,
            witness,
            res);
    }
    else {
        if (f.vertexNumber() == 3) {
            return triangleSphereItersect(
                TriangleWrapper(
                    f.vertex(0)->coord(),
                    f.vertex(1)->coord(),
                    f.vertex(2)->coord()),
                sphere,
                witness,
                res);
        }
        else {
            res.first = std::numeric_limits<SScalar>::max();
            std::pair<SScalar, SScalar> r;

            bool      b = false;
            PointType w;

            std::vector<uint> tris = vcl::earCut(f);
            for (uint i = 0; i < tris.size() && !b; i += 3) {
                b |= triangleSphereItersect(
                    TriangleWrapper(
                        f.vertex(tris[i])->coord(),
                        f.vertex(tris[i + 1])->coord(),
                        f.vertex(tris[i + 2])->coord()),
                    sphere,
                    w,
                    r);

                if (r.first < res.first) {
                    res     = r;
                    witness = w;
                }
            }
            return b;
        }
    }
}

/**
 * @brief Compute the intersection between a sphere and a face, that may be also
 * polygonal.
 *
 * If the face is a triangle, the triangleSphereIntersect function will be used.
 * If the face is polygonal, the face is first triangulated using an earcut
 * algorithm, and then for each triangle, the triangleSphereIntersect is
 * computed.
 *
 * @param[in] f: the input face
 * @param[in] sphere: the input sphere
 * @return true iff there is an intersection between the sphere and the face
 */
template<FaceConcept FaceType, typename SScalar>
bool faceSphereItersect(const FaceType& f, const Sphere<SScalar>& sphere)
{
    Point3<SScalar>             witness;
    std::pair<SScalar, SScalar> res;
    return faceSphereItersect(f, sphere, witness, res);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_INTERSECTION_ELEMENT_H
