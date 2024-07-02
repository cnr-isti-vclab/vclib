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

#ifndef VCL_ALGORITHMS_MESH_DISTANCE_H
#define VCL_ALGORITHMS_MESH_DISTANCE_H

#include <vclib/algorithms/mesh/point_sampling.h>
#include <vclib/math/histogram.h>
#include <vclib/mesh/requirements.h>
#include <vclib/misc/logger.h>
#include <vclib/misc/parallel.h>
#include <vclib/space/grid.h>

namespace vcl {

struct HausdorffDistResult
{
    double            minDist  = std::numeric_limits<double>::max();
    double            maxDist  = std::numeric_limits<double>::lowest();
    double            meanDist = 0;
    double            RMSDist  = 0;
    Histogram<double> histogram;
};

enum HausdorffSamplingMethod {
    HAUSDORFF_VERTEX_UNIFORM = 0,
    HAUSDORFF_EDGE_UNIFORM,
    HAUSDORFF_MONTECARLO
};

namespace detail {

template<
    MeshConcept    MeshType,
    SamplerConcept SamplerType,
    typename GridType,
    LoggerConcept LogType>
HausdorffDistResult hausdorffDist(
    const MeshType&    m,
    const SamplerType& s,
    const GridType&    g,
    LogType&           log)
{
    using PointSampleType = SamplerType::PointType;
    using ScalarType      = PointSampleType::ScalarType;

    HausdorffDistResult res;
    res.histogram = Histogramd(0, m.boundingBox().diagonal() / 100, 100);

    log.log(
        5,
        "Computing distances for " + std::to_string(s.size()) + " samples...");

    log.startProgress("", s.size());

    std::mutex mutex;

    uint ns = 0;
    uint i  = 0;
    vcl::parallelFor(s, [&](const PointSampleType& sample) {
        //    for (const PointSampleType& sample : s) {
        ScalarType dist = std::numeric_limits<ScalarType>::max();
        const auto iter = g.closestValue(sample, dist);

        if (iter != g.end()) {
            mutex.lock();
            ns++;
            if (dist > res.maxDist)
                res.maxDist = dist;
            if (dist < res.minDist)
                res.minDist = dist;
            res.meanDist += dist;
            res.RMSDist += dist * dist;
            res.histogram.addValue(dist);
            mutex.unlock();
        }

        log.progress(++i);
        //    }
    });

    log.endProgress();
    log.log(100, "Computed " + std::to_string(ns) + " distances.");
    if (ns != s.size()) {
        log.log(
            100,
            LogType::WARNING,
            std::to_string(s.size() - ns) +
                " samples were not counted because no closest vertex/face "
                "was found.");
    }

    res.meanDist /= ns;
    res.RMSDist = std::sqrt(res.RMSDist / ns);

    return res;
}

template<
    MeshConcept    MeshType,
    SamplerConcept SamplerType,
    LoggerConcept  LogType>
HausdorffDistResult samplerMeshHausdorff(
    const MeshType&    m,
    const SamplerType& s,
    LogType&           log) requires (!HasFaces<MeshType>)
{
    using VertexType = MeshType::VertexType;

    std::string meshName = "first mesh";
    if constexpr (HasName<MeshType>) {
        meshName = m.name();
    }

    log.log(0, "Building Grid on " + meshName + " vertices...");

    vcl::StaticGrid3<const VertexType*> grid(m.vertices() | views::addrOf);
    grid.build();

    log.log(5, "Grid built.");

    return hausdorffDist(m, s, grid, log);
}

template<
    FaceMeshConcept MeshType,
    SamplerConcept  SamplerType,
    LoggerConcept   LogType>
HausdorffDistResult samplerMeshHausdorff(
    const MeshType&    m,
    const SamplerType& s,
    LogType&           log)
{
    using VertexType = MeshType::VertexType;
    using FaceType   = MeshType::FaceType;
    using ScalarType = VertexType::CoordType::ScalarType;

    std::string meshName = "first mesh";
    if constexpr (HasName<MeshType>) {
        meshName = m.name();
    }
    if (m.faceNumber() == 0) {
        log.log(0, "Building Grid on " + meshName + " vertices...");

        vcl::StaticGrid3<const VertexType*, ScalarType> grid(
            m.vertices() | views::addrOf);
        grid.build();

        log.log(5, "Grid built.");

        return hausdorffDist(m, s, grid, log);
    }
    else {
        log.log(0, "Building Grid on " + meshName + " faces...");

        vcl::StaticGrid3<const FaceType*, ScalarType> grid(
            m.faces() | views::addrOf);
        grid.build();

        log.log(5, "Grid built.");

        return hausdorffDist(m, s, grid, log);
    }
}

template<
    uint           METHOD,
    MeshConcept    MeshType1,
    MeshConcept    MeshType2,
    SamplerConcept SamplerType,
    LoggerConcept  LogType>
HausdorffDistResult hausdorffDistance(
    const MeshType1&   m1,
    const MeshType2&   m2,
    uint               nSamples,
    bool               deterministic,
    SamplerType&       sampler,
    std::vector<uint>& birth,
    LogType&           log)
{
    std::string meshName1 = "first mesh";
    std::string meshName2 = "second mesh";
    if constexpr (HasName<MeshType1>) {
        meshName1 = m1.name();
    }
    if constexpr (HasName<MeshType2>) {
        meshName2 = m2.name();
    }

    log.log(
        0,
        "Sampling " + meshName2 + " with " + std::to_string(nSamples) +
            " samples...");

    if constexpr (METHOD == HAUSDORFF_VERTEX_UNIFORM) {
        sampler = vcl::vertexUniformPointSampling<SamplerType>(
            m2, nSamples, birth, false, deterministic);
    }
    else if constexpr (METHOD == HAUSDORFF_EDGE_UNIFORM) {
        // todo
    }
    else {
        sampler = vcl::montecarloPointSampling<SamplerType>(
            m2, nSamples, birth, deterministic);
    }

    log.log(5, meshName2 + " sampled.");
    log.startNewTask(
        5, 100, "Computing distance between samples and " + meshName1 + "...");

    auto res = samplerMeshHausdorff(m1, sampler, log);

    log.endTask("Distance between samples and " + meshName1 + " computed.");

    return res;
}

} // namespace detail

template<
    MeshConcept   MeshType1,
    MeshConcept   MeshType2,
    LoggerConcept LogType = NullLogger>
HausdorffDistResult hausdorffDistance(
    const MeshType1&        m1,
    const MeshType2&        m2,
    LogType&                log           = nullLogger,
    HausdorffSamplingMethod sampMethod    = HAUSDORFF_VERTEX_UNIFORM,
    uint                    nSamples      = 0,
    bool                    deterministic = false)
{
    if (nSamples == 0)
        nSamples = m2.vertexNumber();

    std::vector<uint> birth;

    switch (sampMethod) {
    case HAUSDORFF_VERTEX_UNIFORM: {
        ConstVertexSampler<typename MeshType2::VertexType> sampler;

        return detail::hausdorffDistance<HAUSDORFF_VERTEX_UNIFORM>(
            m1, m2, nSamples, deterministic, sampler, birth, log);
    }

    case HAUSDORFF_EDGE_UNIFORM: {
        // todo
        return HausdorffDistResult();
    }
    case HAUSDORFF_MONTECARLO: {
        PointSampler<typename MeshType2::VertexType::CoordType> sampler;

        return detail::hausdorffDistance<HAUSDORFF_MONTECARLO>(
            m1, m2, nSamples, deterministic, sampler, birth, log);
    }
    default: assert(0); return HausdorffDistResult();
    }
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_DISTANCE_H
