/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#include "point_sampling.h"

#include <vclib/algorithms/shuffle.h>
#include <vclib/algorithms/stat.h>
#include <vclib/math/random.h>

namespace vcl {

template<SamplerConcept SamplerType, MeshConcept MeshType>
SamplerType allVerticesPointSampling(const MeshType &m, bool onlySelected)
{
	using VertexType = typename MeshType::VertexType;

	SamplerType sampler;
	for (const VertexType& v : m.vertices()) {
		if (!onlySelected || v.isSelected())
			sampler.addVertex(v, m);
	}
	return sampler;
}

template<FaceSamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType allFacesPointSampling(const MeshType& m, bool onlySelected)
{
	using FaceType = typename MeshType::FaceType;

	SamplerType sampler;
	for (const FaceType& f : m.faces()) {
		if (!onlySelected || f.isSelected())
			sampler.addFace(f, m);
	}
	return sampler;
}

template<SamplerConcept SamplerType, MeshConcept MeshType>
SamplerType vertexUniformPointSampling(
	const MeshType &m,
	uint nSamples,
	bool onlySelected,
	bool deterministic)
{
	uint vn = onlySelected ? vcl::vertexSelectionNumber(m) : m.vertexNumber();

	if (nSamples >= vn) {
		return allVerticesPointSampling<SamplerType>(m, onlySelected);
	}

	SamplerType ps;

	std::uniform_int_distribution<uint> dist(0, m.vertexContainerSize() - 1);

	std::random_device rd;
	std::mt19937 gen(rd());
	if (deterministic)
		gen = std::mt19937(0);

	std::vector<bool> visited(m.vertexContainerSize(), false);
	uint nVisited = 0;

	while (nVisited < nSamples) {
		uint vi = dist(gen);
		if (!m.vertex(vi).isDeleted() && !visited[vi] &&
			(!onlySelected || m.vertex(vi).isSelected())) {
			visited[vi] = true;
			nVisited++;
			ps.addVertex(m.vertex(vi), m);
		}
	}

	return ps;
}

template<FaceSamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType faceUniformPointSampling(
	const MeshType& m,
	uint            nSamples,
	bool            onlySelected,
	bool            deterministic)
{
	uint fn = onlySelected ? vcl::faceSelectionNumber(m) : m.faceNumber();

	if (nSamples >= fn) {
		return allFacesPointSampling<SamplerType>(m, onlySelected);
	}

	SamplerType ps;

	std::uniform_int_distribution<uint> dist(0, m.faceContainerSize() - 1);

	std::random_device rd;
	std::mt19937 gen(rd());
	if (deterministic)
		gen = std::mt19937(0);

	std::vector<bool> visited(m.faceContainerSize(), false);
	uint nVisited = 0;

	while (nVisited < nSamples) {
		uint fi = dist(gen);
		if (!m.face(fi).isDeleted() && !visited[fi] &&
			(!onlySelected || m.face(fi).isSelected())) {
			visited[fi] = true;
			nVisited++;
			ps.addFace(m.face(fi), m);
		}
	}

	return ps;
}

/**
 * @brief Samples the vertices in a weighted way, using the per vertex weights given as input.
 * Each vertex has a probability of being chosen that is proportional to its weight.
 *
 * @param m
 * @param weights: a vector of scalars having the i-th entry associated to the vertex having index i.
 *                 Note: weights.size() == m.vertexContainerSize().
 * @param nSamples
 * @return A Sampler, that is a collection of samples selected from the input mesh.
 */
template<SamplerConcept SamplerType, MeshConcept MeshType, typename ScalarType>
SamplerType vertexWeightedPointSampling(
	const MeshType& m,
	const std::vector<ScalarType>& weights,
	uint nSamples,
	bool deterministic)
{
	if (nSamples >= m.vertexNumber()) {
		return allVerticesPointSampling<SamplerType>(m);
	}

	SamplerType ps;

	std::discrete_distribution<> dist(std::begin(weights), std::end(weights));

	std::random_device rd;
	std::mt19937 gen(rd());
	if (deterministic)
		gen = std::mt19937(0);

	std::vector<bool> visited(m.vertexContainerSize(), false);
	uint nVisited = 0;

	while (nVisited < nSamples) {
		uint vi = dist(gen);
		if (vi < m.vertexContainerSize() && !m.vertex(vi).isDeleted() && !visited[vi]) {
			visited[vi] = true;
			nVisited++;
			ps.addVertex(m.vertex(vi), m);
		}
	}

	return ps;
}

/**
 * @brief Samples the barycenter faces in a weighted way, using the per face weights given as input.
 * Each face has a probability of being chosen that is proportional to its weight.
 *
 * @param m
 * @param weights: a vector of scalars having the i-th entry associated to the face having index i.
 *                 Note: weights.size() == m.faceContainerSize().
 * @param nSamples
 * @return A Sampler, that is a collection of samples selected from the input mesh.
 */
template<FaceSamplerConcept SamplerType, FaceMeshConcept MeshType, typename ScalarType>
SamplerType faceWeightedPointSampling(
	const MeshType&                m,
	const std::vector<ScalarType>& weights,
	uint                           nSamples,
	bool                           deterministic)
{
	if (nSamples >= m.faceNumber()) {
		return allFacesPointSampling<SamplerType>(m);
	}

	SamplerType ps;

	std::discrete_distribution<> dist(std::begin(weights), std::end(weights));

	std::random_device rd;
	std::mt19937 gen(rd());
	if (deterministic)
		gen = std::mt19937(0);

	std::vector<bool> visited(m.faceContainerSize(), false);
	uint nVisited = 0;

	while (nVisited < nSamples) {
		uint fi = dist(gen);
		if (fi < m.faceContainerSize() && !m.face(fi).isDeleted() && !visited[fi]) {
			visited[fi] = true;
			nVisited++;
			ps.addFace(m.face(fi), m);
		}
	}

	return ps;
}

/**
 * @brief Samples the vertices in a weighted way, using the per vertex Scalar component. Each vertex
 * has a probability of being chosen that is proportional to its scalar value.
 *
 * @param m
 * @param nSamples
 * @return
 */
template<SamplerConcept SamplerType, MeshConcept MeshType>
SamplerType vertexScalarWeightedPointSampling(const MeshType& m, uint nSamples, bool deterministic)
{
	vcl::requirePerVertexScalar(m);

	using VertexType = typename MeshType::VertexType;
	using ScalarType = typename VertexType::ScalarType;

	std::vector<ScalarType> weights;
	weights.resize(m.vertexContainerSize(), 0);
	for (const VertexType& v : m.vertices()) {
		weights[m.index(v)] = v.scalar();
	}

	return vertexWeightedPointSampling<SamplerType>(m, weights, nSamples, deterministic);
}

/**
 * @brief Samples the faces in a weighted way, using the per face Scalar component. Each face
 * has a probability of being chosen that is proportional to its scalar value.
 *
 * @param m
 * @param nSamples
 * @return
 */
template<FaceSamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType faceScalarWeightedPointSampling(const MeshType& m, uint nSamples, bool deterministic)
{
	vcl::requirePerFaceScalar(m);

	using FaceType = typename MeshType::FaceType;
	using ScalarType = typename FaceType::ScalarType;

	std::vector<ScalarType> weights;
	weights.resize(m.faceContainerSize(), 0);
	for (const FaceType& f : m.faces()) {
		weights[m.index(f)] = f.scalar();
	}

	return faceWeightedPointSampling<SamplerType>(m, weights, nSamples, deterministic);
}

template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType vertexAreaWeightedPointSampling(const MeshType& m, uint nSamples, bool deterministic)
{
	using VertexType = typename MeshType::VertexType;
	using ScalarType = typename VertexType::ScalarType;
	using FaceType = typename MeshType::FaceType;

	std::vector<ScalarType> weights(m.vertexContainerSize(), 0);
	std::vector<uint> cnt(m.vertexContainerSize(), 0);

	// for each vertex, store in weights the adjacent faces area and their number
	for (const FaceType& f : m.faces()) {
		ScalarType area = vcl::polygonArea(f);
		for (const VertexType* v : f.vertices()) {
			weights[m.index(v)] += area;
			cnt[m.index(v)]++;
		}
	}

	// divide each area sum by the number of adjacent faces
	for (uint i = 0; i < weights.size(); i++) {
		if (cnt[i] > 0)
			weights[i] /= cnt[i];
	}

	// use these weights to create a sapler
	return vertexWeightedPointSampling<SamplerType>(m, weights, nSamples, deterministic);
}

template<FaceSamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType faceAreaWeightedPointSampling(
	const MeshType& m,
	uint nSamples,
	bool deterministic)
{
	using FaceType = typename MeshType::FaceType;

	std::vector<double> weights(m.faceContainerSize());

	for (const FaceType& f : m.faces()) {
		weights[m.index(f)] =  vcl::polygonArea(f);
	}

	return faceWeightedPointSampling<SamplerType>(m, weights, nSamples, deterministic);
}

template<FaceSamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType stratifiedMontecarloPointSampling(const MeshType& m, uint nSamples, bool deterministic)
{
	using FaceType = typename MeshType::FaceType;
	using ScalarType = typename SamplerType::ScalarType;

	SamplerType ps;

	std::random_device rd;
	std::mt19937 gen(rd());
	if (deterministic)
		gen = std::mt19937(0);

	double area = vcl::surfaceArea(m);
	double samplePerAreaUnit = nSamples / area;
	// Montecarlo sampling.
	double  floatSampleNum = 0.0;

	for(const FaceType& f : m.faces()) {
		// compute # samples in the current face (taking into account of the remainders)
		floatSampleNum += vcl::polygonArea(f) * samplePerAreaUnit;
		int faceSampleNum   = (int) floatSampleNum;
		// for every sample p_i in T...
		for(int i=0; i < faceSampleNum; i++)
			ps.addFace(f, m, vcl::randomPolygonBarycentricCoordinate<ScalarType>(f.vertexNumber(), gen));
		floatSampleNum -= (double) faceSampleNum;
	}

	return ps;
}

} // namespace vcl

