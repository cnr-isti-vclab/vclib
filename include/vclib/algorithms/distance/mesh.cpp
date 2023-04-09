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

#include "mesh.h"

#include <vclib/algorithms/point_sampling.h>
#include <vclib/space/grid.h>
#include <vclib/misc/parallel.h>
#include <vclib/views.h>

#include <mutex>

namespace vcl {

namespace internal {

template<MeshConcept MeshType, SamplerConcept SamplerType, typename GridType, LoggerConcept  LogType>
HausdorffDistResult hausdorffDist(
	const MeshType& m,
	const SamplerType& s,
	const GridType& g,
	LogType& log)
{
	using PointSampleType = typename SamplerType::PointType;

	HausdorffDistResult res;
	res.histogram = Histogramd(0, m.boundingBox().diagonal() / 100, 100);

	if constexpr (vcl::isLoggerValid<LogType>()) {
		log.log(5, "Computing distances for " + std::to_string(s.size()) + " samples...");
	}

	if constexpr (vcl::isLoggerValid<LogType>()) {
		log.startProgress("", s.size());
	}

	std::mutex mutex;

	uint ns = 0;
	uint i = 0;
	vcl::parallelFor(s, [&](const PointSampleType& sample){
//	for (const PointSampleType& sample : s) {
		double dist = std::numeric_limits<double>::max();
		const auto iter = g.closestValue(sample, dist);

		if (iter != g.end()) {
			mutex.lock();
			ns++;
			if (dist > res.maxDist)
				res.maxDist = dist;
			if (dist < res.minDist)
				res.minDist = dist;
			res.meanDist += dist;
			res.RMSDist += dist*dist;
			res.histogram.addValue(dist);
			mutex.unlock();
		}

		if constexpr (vcl::isLoggerValid<LogType>()) {
			mutex.lock();
			++i;
			log.progress(i);
			mutex.unlock();
		}
//	}
	});

	if constexpr (vcl::isLoggerValid<LogType>()) {
		log.endProgress();
		log.log(100, "Computed " + std::to_string(ns) + " distances.");
		if (ns != s.size()) {
			log.log(
				100,
				LogType::WARNING,
				std::to_string(s.size() - ns) +
					" samples were not counted because no closest vertex/face was found.");
		}
	}

	res.meanDist /= ns;
	res.RMSDist = std::sqrt(res.RMSDist / ns);

	return res;
}

template<MeshConcept MeshType, SamplerConcept SamplerType, LoggerConcept  LogType>
HausdorffDistResult samplerMeshHausdorff(
	const MeshType& m,
	const SamplerType& s,
	LogType& log)
	requires(!HasFaces<MeshType>)
{
	using VertexType = typename MeshType::VertexType;

	std::string meshName = "first mesh";
	if constexpr (HasName<MeshType>){
		meshName = m.name();
	}
	if constexpr (vcl::isLoggerValid<LogType>()) {
		log.log(0, "Building Grid on " + meshName + " vertices...");
	}

	vcl::StaticGrid3<const VertexType*> grid(m.vertices() | views::reference);
	grid.build();

	if constexpr (vcl::isLoggerValid<LogType>()) {
		log.log(5, "Grid built.");
	}

	return hausdorffDist(m, s, grid, log);
}

template<FaceMeshConcept MeshType, SamplerConcept SamplerType, LoggerConcept  LogType>
HausdorffDistResult samplerMeshHausdorff(
	const MeshType& m,
	const SamplerType& s,
	LogType& log)
{
	using VertexType = typename MeshType::VertexType;
	using FaceType   = typename MeshType::FaceType;

	std::string meshName = "first mesh";
	if constexpr (HasName<MeshType>){
		meshName = m.name();
	}
	if (m.faceNumber() == 0) {
		if constexpr (vcl::isLoggerValid<LogType>()) {
			log.log(0, "Building Grid on " + meshName + " vertices...");
		}

		vcl::StaticGrid3<const VertexType*> grid(m.vertices() | views::reference);
		grid.build();

		if constexpr (vcl::isLoggerValid<LogType>()) {
			log.log(5, "Grid built.");
		}

		return hausdorffDist(m, s, grid, log);
	}
	else {
		if constexpr (vcl::isLoggerValid<LogType>()) {
			log.log(0, "Building Grid on " + meshName + " faces...");
		}

		vcl::StaticGrid3<const FaceType*> grid(m.faces() | views::reference);
		grid.build();

		if constexpr (vcl::isLoggerValid<LogType>()) {
			log.log(5, "Grid built.");
		}

		return hausdorffDist(m, s, grid, log);
	}
}

template<
	uint METHOD,
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
	LogType& log)
{
	std::string meshName1 = "first mesh";
	std::string meshName2 = "second mesh";
	if constexpr (HasName<MeshType1>) {
		meshName1 = m1.name();
	}
	if constexpr (HasName<MeshType2>) {
		meshName2 = m2.name();
	}

	if constexpr (vcl::isLoggerValid<LogType>()) {
		log.log(0, "Sampling " + meshName2 + " with " + std::to_string(nSamples) + " samples...");
	}

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

	if constexpr (vcl::isLoggerValid<LogType>()) {
		log.log(5, meshName2 + " sampled.");
		log.startNewTask(5, 100, "Computing distance between samples and " + meshName1 + "...");
	}

	auto res = samplerMeshHausdorff(m1, sampler, log);

	if constexpr (vcl::isLoggerValid<LogType>()) {
		log.endTask("Distance between samples and " + meshName1 + " computed.");
	}

	return res;
}

} // namespace vcl::internal

template<MeshConcept MeshType1, MeshConcept MeshType2, LoggerConcept LogType>
HausdorffDistResult hausdorffDistance(
	const MeshType1& m1,
	const MeshType2& m2,
	LogType& log,
	HausdorffSamplingMethod sampMethod,
	uint nSamples,
	bool deterministic)
{
	if (nSamples == 0)
		nSamples = m2.vertexNumber();

	std::vector<uint> birth;

	switch (sampMethod) {
	case HAUSDORFF_VERTEX_UNIFORM: {
		ConstVertexSampler<typename MeshType2::VertexType> sampler;

		return internal::hausdorffDistance<HAUSDORFF_VERTEX_UNIFORM>(
			m1, m2, nSamples, deterministic, sampler, birth, log);
	}

	case HAUSDORFF_EDGE_UNIFORM: {
		// todo
		return HausdorffDistResult();
	}
	case HAUSDORFF_MONTECARLO: {
		PointSampler<typename MeshType2::VertexType::CoordType> sampler;

		return internal::hausdorffDistance<HAUSDORFF_MONTECARLO>(
			m1, m2, nSamples, deterministic, sampler, birth, log);
	}
	default:
		assert(0);
		return HausdorffDistResult();
	}
}

} // namespace vcl
