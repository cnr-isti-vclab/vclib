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

#ifndef VCL_MESH_ELEM_ALGORITHMS_INTERSECTION_H
#define VCL_MESH_ELEM_ALGORITHMS_INTERSECTION_H

#include <vclib/mesh/elements.h>

#include <vclib/algorithms/core.h>
#include <vclib/concepts.h>
#include <vclib/space/core.h>

namespace vcl {

/**
 * @brief Checks if a face intersects a box.
 *
 * The function uses the separating axis theorem to test the overlap between a
 * triangle and a box. If the face is a triangle, the function will use the
 * intersect function between a triangle and a box. If the face is polygonal,
 * the face is first triangulated using an earcut algorithm, and then for each
 * triangle, the triangle-box intersection is computed.
 *
 * @see vcl::intersect(const TriangleWrapper<Scalar>&, const Box<PointType>&)
 *
 * @tparam FaceType: A type that satisfies the FaceConcept.
 * @tparam PointType: A type that satisfies the PointConcept.
 *
 * @param[in] f: The input face.
 * @param[in] box: The input box.
 * @return True if the face intersects the box, false otherwise.
 *
 * @ingroup core_intersection
 */
template<FaceConcept FaceType, PointConcept PointType>
bool intersect(const FaceType& f, const Box<PointType>& box)
{
    if constexpr (TriangleFaceConcept<FaceType>) {
        return intersect(
            TriangleWrapper(
                f.vertex(0)->position(),
                f.vertex(1)->position(),
                f.vertex(2)->position()),
            box);
    }
    else {
        bool b = false;

        std::vector<uint> tris = earCut(f);
        for (uint i = 0; i < tris.size() && !b; i += 3) {
            b |= intersect(
                TriangleWrapper(
                    f.vertex(tris[i])->position(),
                    f.vertex(tris[i + 1])->position(),
                    f.vertex(tris[i + 2])->position()),
                box);
        }
        return b;
    }
}

/**
 * @copydoc intersect(const FaceType&, const Box<PointType>&)
 *
 * @ingroup core_intersection
 */
template<PointConcept PointType, FaceConcept FaceType>
bool intersect(const Box<PointType>& box, const FaceType& f)
{
    return intersect(f, box);
}

/**
 * @brief Compute the intersection between a sphere and a face, that may be also
 * polygonal.
 *
 * If the face is a triangle, the intersect function between triangle and sphere
 * will be used. If the face is polygonal, the face is first triangulated using
 * an earcut algorithm, and then for each triangle, the triangle-sphere
 * intersection is computed.
 *
 * @param[in] f: the input face
 * @param[in] sphere: the input sphere
 * @param[out] witness: the point on the triangle nearest to the center of the
 * sphere (even when there isn't intersection)
 * @param[out] res: if not null, in the first item is stored the minimum
 * distance between the face and the sphere, while in the second item is stored
 * the penetration depth
 * @return true iff there is an intersection between the sphere and the face
 *
 * @ingroup core_intersection
 */
template<FaceConcept FaceType, PointConcept PointType, typename SScalar>
bool intersect(
    const FaceType&              f,
    const Sphere<SScalar>&       sphere,
    PointType&                   witness,
    std::pair<SScalar, SScalar>& res)
{
    if constexpr (TriangleFaceConcept<FaceType>) {
        return intersect(
            TriangleWrapper(
                f.vertex(0)->position(),
                f.vertex(1)->position(),
                f.vertex(2)->position()),
            sphere,
            witness,
            res);
    }
    else {
        if (f.vertexNumber() == 3) {
            return intersect(
                TriangleWrapper(
                    f.vertex(0)->position(),
                    f.vertex(1)->position(),
                    f.vertex(2)->position()),
                sphere,
                witness,
                res);
        }
        else {
            res.first = std::numeric_limits<SScalar>::max();
            std::pair<SScalar, SScalar> r;

            bool      b = false;
            PointType w;

            std::vector<uint> tris = earCut(f);
            for (uint i = 0; i < tris.size() && !b; i += 3) {
                b |= intersect(
                    TriangleWrapper(
                        f.vertex(tris[i])->position(),
                        f.vertex(tris[i + 1])->position(),
                        f.vertex(tris[i + 2])->position()),
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
 * If the face is a triangle, the intersect function between triangle and sphere
 * will be used. If the face is polygonal, the face is first triangulated using
 * an earcut algorithm, and then for each triangle, the triangle-sphere
 * intersection is computed.
 *
 * @param[in] f: the input face
 * @param[in] sphere: the input sphere
 * @return true iff there is an intersection between the sphere and the face
 *
 * @ingroup core_intersection
 */
template<FaceConcept FaceType, typename SScalar>
bool intersect(const FaceType& f, const Sphere<SScalar>& sphere)
{
    Point3<SScalar>             witness;
    std::pair<SScalar, SScalar> res;
    return intersect(f, sphere, witness, res);
}

/**
 * @copydoc intersect(const FaceType&, const Sphere<SScalar>&)
 *
 * @ingroup core_intersection
 */
template<typename SScalar, FaceConcept FaceType>
bool intersect(const Sphere<SScalar>& sphere, const FaceType& f)
{
    return intersect(f, sphere);
}

} // namespace vcl

#endif // VCL_MESH_ELEM_ALGORITHMS_INTERSECTION_H
