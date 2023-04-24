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

#ifndef VCL_ALGORITHMS_POINT_SAMPLING_H
#define VCL_ALGORITHMS_POINT_SAMPLING_H

#include <vclib/mesh/requirements.h>
#include <vclib/space/sampler.h>

/**
 * @defgroup point_sampling Point Sampling Algorithms
 *
 * @ingroup algorithms
 *
 * @brief List of functions that compute point samplings.
 *
 * These functions compute saplings using various simple and complex methods, and return objects
 * that satisfy the @ref SamplerConcept. A Sampler is an object that allows to store points that
 * can be results of some geometric function (e.g. a point sample that is computed as the
 * barycentric coordinate of a triangle).
 *
 * You can access these algorithms by including `#include <vclib/algorithms/point_sampling.h>`
 */

namespace vcl {

template<SamplerConcept SamplerType, MeshConcept MeshType>
SamplerType allVerticesPointSampling(
	const MeshType&    m,
	std::vector<uint>& birthVertices,
	bool               onlySelected = false);

template<SamplerConcept SamplerType, MeshConcept MeshType>
SamplerType allVerticesPointSampling(const MeshType& m, bool onlySelected = false);

template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType allFacesPointSampling(
	const MeshType&    m,
	std::vector<uint>& birthFaces,
	bool               onlySelected = false);

template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType allFacesPointSampling(const MeshType& m, bool onlySelected = false);

template<SamplerConcept SamplerType, MeshConcept MeshType>
SamplerType vertexUniformPointSampling(
	const MeshType&    m,
	uint               nSamples,
	std::vector<uint>& birthVertices,
	bool               onlySelected  = false,
	bool               deterministic = false);

template<SamplerConcept SamplerType, MeshConcept MeshType>
SamplerType vertexUniformPointSampling(
	const MeshType&    m,
	uint               nSamples,
	bool               onlySelected  = false,
	bool               deterministic = false);

template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType faceUniformPointSampling(
	const MeshType&    m,
	uint               nSamples,
	std::vector<uint>& birthFaces,
	bool               onlySelected  = false,
	bool               deterministic = false);

template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType faceUniformPointSampling(
	const MeshType&    m,
	uint               nSamples,
	bool               onlySelected  = false,
	bool               deterministic = false);

template<SamplerConcept SamplerType, MeshConcept MeshType, typename ScalarType>
SamplerType vertexWeightedPointSampling(
	const MeshType&                m,
	const std::vector<ScalarType>& weights,
	uint                           nSamples,
	std::vector<uint>&             birthVertices,
	bool                           deterministic = false);

template<SamplerConcept SamplerType, MeshConcept MeshType, typename ScalarType>
SamplerType vertexWeightedPointSampling(
	const MeshType&                m,
	const std::vector<ScalarType>& weights,
	uint                           nSamples,
	bool                           deterministic = false);

template<SamplerConcept SamplerType, FaceMeshConcept MeshType, typename ScalarType>
SamplerType faceWeightedPointSampling(
	const MeshType&                m,
	const std::vector<ScalarType>& weights,
	uint                           nSamples,
	std::vector<uint>&             birthFaces,
	bool                           deterministic = false);

template<SamplerConcept SamplerType, FaceMeshConcept MeshType, typename ScalarType>
SamplerType faceWeightedPointSampling(
	const MeshType&                m,
	const std::vector<ScalarType>& weights,
	uint                           nSamples,
	bool                           deterministic = false);

template<SamplerConcept SamplerType, MeshConcept MeshType>
SamplerType vertexScalarWeightedPointSampling(
	const MeshType& m,
	uint nSamples,
	bool deterministic = false);

template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType vertexAreaWeightedPointSampling(
	const MeshType& m,
	uint nSamples,
	bool deterministic = false);

template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType faceAreaWeightedPointSampling(
	const MeshType& m,
	uint nSamples,
	bool deterministic = false);

template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType montecarloPointSampling(
	const MeshType&    m,
	uint               nSamples,
	std::vector<uint>& birthFaces,
	bool               deterministic = false);

template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType montecarloPointSampling(const MeshType& m, uint nSamples, bool deterministic = false);

template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType stratifiedMontecarloPointSampling(
	const MeshType& m,
	uint nSamples,
	bool deterministic = false);

template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType montecarloPoissonPointSampling(
	const MeshType& m,
	uint nSamples,
	bool deterministic = false);

template<SamplerConcept SamplerType, FaceMeshConcept MeshType, typename ScalarType>
SamplerType vertexWeightedMontecarloPointSampling(
	const MeshType& m,
	const std::vector<ScalarType>& weights,
	uint nSamples,
	double variance,
	bool deterministic = false);

template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType vertexScalarWeightedMontecarloPointSampling(
	const MeshType& m,
	uint nSamples,
	double variance,
	bool deterministic = false);

} // namespace vcl

#include "point_sampling.cpp"

#endif // VCL_ALGORITHMS_POINT_SAMPLING_H
