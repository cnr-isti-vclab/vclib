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

#ifndef SPACE_H
#define SPACE_H

#include <vclib/mesh/tmp_meshes.h>

#include "space/array.h"
#include "space/box.h"
#include "space/color.h"
#include "space/image.h"
#include "space/matrix.h"
#include "space/point.h"

void spaceStaticAsserts()
{
    arrayStaticAsserts();
    boxStaticAsserts();
    colorStaticAsserts();
    imageStaticAsserts();
    matrixStaticAsserts();
    pointStaticAsserts();

    using namespace vcl;

    // bitset
    static_assert(
        Serializable<BitSet<char>>, "Bitset<char> is not serializable");

    // plane
    static_assert(
        PlaneConcept<Planef>, "Planef does not satisfy the PlaneConcept");
    static_assert(
        PlaneConcept<Planed>, "Planed does not satisfy the PlaneConcept");

    static_assert(
        Polygon2Concept<Polygon2f>,
        "Polygon2f does not satisfy the Polygon2Concept");
    static_assert(
        Polygon2Concept<Polygon2d>,
        "Polygon2d does not satisfy the Polygon2Concept");

    static_assert(
        Polygon3Concept<Polygon3f>,
        "Polygon3f does not satisfy the Polygon3Concept");
    static_assert(
        Polygon3Concept<Polygon3d>,
        "Polygon3d does not satisfy the Polygon3Concept");

    // point iterators
    static_assert(PointIteratorConcept<std::vector<Point2d>::iterator>, "");
    static_assert(
        PointIteratorConcept<std::vector<Point3d>::const_iterator>, "");

    // sampler
    static_assert(
        SamplerConcept<MeshSampler<detail::TMPSimplePolyMesh>>,
        "MeshSampler does not satisfy the SamplerConcept");

    static_assert(
        SamplerConcept<PointSampler<>>,
        "PointSampler does not satisfy the SamplerConcept");

    static_assert(
        SamplerConcept<VertexSampler<detail::TMPSimplePolyMesh::Vertex>>,
        "VertexSampler does not satisfy the SamplerConcept");
    static_assert(
        SamplerConcept<ConstVertexSampler<detail::TMPSimplePolyMesh::Vertex>>,
        "ConstVertexSampler does not satisfy the SamplerConcept");
    //     static_assert(
    //        std::ranges::range<VertexSampler<detail::TMPSimplePolyMesh::Vertex>>,
    //        "");

    //    VertexSampler<detail::TMPSimplePolyMesh::Vertex> v;
    //    auto it = std::ranges::begin(v);

    // sphere
    static_assert(
        SphereConcept<Spheref>, "Spheref does not satisfy the SphereConcept");
    static_assert(
        SphereConcept<Sphered>, "Sphered does not satisfy the SphereConcept");

    // texture
    static_assert(
        TextureConcept<Texture>, "Texture does not satisfy the TextureConcept");
    static_assert(Serializable<Texture>, "Texture is not serializable");

    // triangle
    static_assert(
        Triangle2Concept<Triangle2f>,
        "Triangle2f does not satisfy the Triangle2Concept");
    static_assert(
        Triangle2Concept<Triangle2d>,
        "Triangle2d does not satisfy the Triangle2Concept");
    static_assert(
        ConstTriangle2Concept<Triangle2f>,
        "Triangle2f does not satisfy the ConstTriangle2Concept");
    static_assert(
        ConstTriangle2Concept<Triangle2d>,
        "Triangle2d does not satisfy the ConstTriangle2Concept");

    static_assert(
        Triangle3Concept<Triangle3f>,
        "Triangle3f does not satisfy the Triangle3Concept");
    static_assert(
        Triangle3Concept<Triangle3d>,
        "Triangle3d does not satisfy the Triangle3Concept");
    static_assert(
        ConstTriangle3Concept<Triangle3f>,
        "Triangle3f does not satisfy the ConstTriangle3Concept");
    static_assert(
        ConstTriangle3Concept<Triangle3d>,
        "Triangle3d does not satisfy the ConstTriangle3Concept");

    static_assert(
        ConstTriangle2Concept<TriangleWrapper2f>,
        "TriangleWrapper2f does not satisfy the ConstTriangle2Concept");
    static_assert(
        ConstTriangle2Concept<TriangleWrapper2d>,
        "TriangleWrapper2d does not satisfy the ConstTriangle2Concept");

    static_assert(
        ConstTriangle3Concept<Triangle3f>,
        "Triangle3f does not satisfy the ConstTriangle3Concept");
    static_assert(
        ConstTriangle3Concept<Triangle3d>,
        "Triangle3d does not satisfy the ConstTriangle3Concept");
}

#endif // SPACE_H
