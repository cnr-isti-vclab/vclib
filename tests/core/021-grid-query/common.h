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

#ifndef COMMON_H
#define COMMON_H

#include <vclib/algorithms.h>
#include <vclib/meshes.h>
#include <vclib/space.h>
#include <vclib/views.h>

#include <random>

template<typename ValueType, typename ScalarType>
using HSGrid3 = vcl::HashTableGrid3<ValueType, ScalarType>;

template<vcl::Box3Concept BoxType>
auto randomPoints(
    vcl::uint      n,
    const BoxType& bbox,
    std::size_t    seed = std::random_device()())
{
    using ScalarType = BoxType::PointType::ScalarType;
    using DistrType  = std::uniform_real_distribution<ScalarType>;

    std::vector<vcl::Point3<ScalarType>> points(n);

    ScalarType ext = bbox.diagonal() * 0.1;

    std::cerr << "Random seed: " << seed << std::endl;

    std::mt19937 gen(seed);
    DistrType    disX(bbox.min().x() - ext, bbox.max().x() + ext);
    DistrType    disY(bbox.min().y() - ext, bbox.max().y() + ext);
    DistrType    disZ(bbox.min().z() - ext, bbox.max().z() + ext);

    for (vcl::uint i = 0; i < n; i++)
        points[i] = vcl::Point3<ScalarType>(disX(gen), disY(gen), disZ(gen));

    return points;
}

template<vcl::MeshConcept MeshType>
auto randomSpheres(
    vcl::uint       n,
    const MeshType& mesh,
    std::size_t     seed = std::random_device()())
{
    using PointType  = MeshType::VertexType::PositionType;
    using ScalarType = PointType::ScalarType;

    using VertDistrType   = std::uniform_int_distribution<vcl::uint>;
    using NoiseDistrType  = std::uniform_real_distribution<ScalarType>;
    using RadiusDistrType = std::normal_distribution<ScalarType>;

    std::vector<vcl::Sphere<ScalarType>> spheres(n);

    auto bbox = vcl::boundingBox(mesh);

    ScalarType ext = bbox.diagonal() * 0.01;

    std::cerr << "Random seed: " << seed << std::endl;

    std::mt19937 gen(seed);

    VertDistrType  disV(0, mesh.vertexNumber());
    NoiseDistrType disX(-ext, ext);
    NoiseDistrType disY(-ext, ext);
    NoiseDistrType disZ(-ext, ext);

    // parameters of normal distribution for radius of the sphere
    double mu    = bbox.diagonal() / 5;
    double sigma = mu / 3; // less probability that the radius is negative

    RadiusDistrType disR(mu, sigma);

    for (vcl::uint i = 0; i < n; i++) {
        // random vertex coordinate pertubated will be the center of the sphere
        PointType p = mesh.vertex(disV(gen)).coord();
        p += PointType(disX(gen), disY(gen), disZ(gen));

        // random radius with normal distribution
        ScalarType radius = disR(gen);
        if (radius < 0)
            radius = 0;

        spheres[i] = vcl::Sphere<ScalarType>(p, radius);
    }

    return spheres;
}

template<
    template<typename, typename>
    typename Grid,
    vcl::FaceMeshConcept MeshType>
auto computeGrid(const MeshType& mesh)
{
    using ScalarType = MeshType::VertexType::PositionType::ScalarType;
    using FaceType   = MeshType::FaceType;

    return Grid<const FaceType*, ScalarType>(
        mesh.faces() | vcl::views::constAddrOf);
}

#endif // COMMON_H
