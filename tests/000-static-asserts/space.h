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

#include <vclib/meshes/detail/tmp_meshes.h>
#include <vclib/space.h>

void spaceStaticAsserts()
{
    using namespace vcl;

    // array
    static_assert(
        ArrayConcept<Array<int, 2>>, "Array does not satisfy the ArrayConcept");

    // array2
    static_assert(
        Array2Concept<Array2<int>>,
        "Array2i does not satisfy the Array2Concept");
    static_assert(
        Array2Concept<Array2<float>>,
        "Array2f does not satisfy the Array2Concept");
    static_assert(
        Array2Concept<Array2<double>>,
        "Array2d does not satisfy the Array2Concept");
    static_assert(Serializable<Array2<double>>, "Array2d is not serializable");

    // array3
    static_assert(
        Array3Concept<Array3<int>>,
        "Array3i does not satisfy the Array3Concept");
    static_assert(
        Array3Concept<Array3<float>>,
        "Array3f does not satisfy the Array3Concept");
    static_assert(
        Array3Concept<Array3<double>>,
        "Array3d does not satisfy the Array3Concept");
    static_assert(Serializable<Array3<double>>, "Array3d is not serializable");

    // array4
    static_assert(
        Array4Concept<Array4<int>>,
        "Array4i does not satisfy the Array4Concept");
    static_assert(
        Array4Concept<Array4<float>>,
        "Array4f does not satisfy the Array4Concept");
    static_assert(
        Array4Concept<Array4<double>>,
        "Array4d does not satisfy the Array4Concept");
    static_assert(Serializable<Array4<double>>, "Array4d is not serializable");

    // bitset
    static_assert(
        Serializable<BitSet<char>>, "Bitset<char> is not serializable");

    // box2
    static_assert(Box2Concept<Box2i>, "Box2i does not satisfy the Box2Concept");
    static_assert(Box2Concept<Box2f>, "Box2f does not satisfy the Box2Concept");
    static_assert(Box2Concept<Box2d>, "Box2d does not satisfy the Box2Concept");
    static_assert(Serializable<Box2d>, "Box2d is not serializable");

    // box3
    static_assert(Box3Concept<Box3i>, "Box3i does not satisfy the Box3Concept");
    static_assert(Box3Concept<Box3f>, "Box3f does not satisfy the Box3Concept");
    static_assert(Box3Concept<Box3d>, "Box3d does not satisfy the Box3Concept");
    static_assert(Serializable<Box3d>, "Box3d is not serializable");

    // color
    static_assert(Serializable<Color>, "Color is not serializable");

    // plane
    static_assert(
        PlaneConcept<Planef>, "Planef does not satisfy the PlaneConcept");
    static_assert(
        PlaneConcept<Planed>, "Planed does not satisfy the PlaneConcept");

    // point2
    static_assert(
        Point2Concept<Point2i>, "Point2i does not satisfy the Point2Concept");
    static_assert(
        Point2Concept<Point2f>, "Point2f does not satisfy the Point2Concept");
    static_assert(
        Point2Concept<Point2d>, "Point2d does not satisfy the Point2Concept");
    static_assert(Serializable<Point2d>, "Point2d is not serializable");

    // point3
    static_assert(
        Point3Concept<Point3i>, "Point3i does not satisfy the Point3Concept");
    static_assert(
        Point3Concept<Point3f>, "Point3f does not satisfy the Point3Concept");
    static_assert(
        Point3Concept<Point3d>, "Point3d does not satisfy the Point3Concept");
    static_assert(Serializable<Point3d>, "Point3d is not serializable");

    static_assert(
        Point4Concept<Point4i>, "Point4i does not satisfy the Point4Concept");
    static_assert(
        Point4Concept<Point4f>, "Point4f does not satisfy the Point4Concept");
    static_assert(
        Point4Concept<Point4d>, "Point4d does not satisfy the Point4Concept");
    static_assert(Serializable<Point4d>, "Point4d is not serializable");

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

    static_assert(
        SphereConcept<Spheref>, "Spheref does not satisfy the SphereConcept");
    static_assert(
        SphereConcept<Sphered>, "Sphered does not satisfy the SphereConcept");

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
