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

#ifndef VCL_ALGORITHMS_SMOOTH_H
#define VCL_ALGORITHMS_SMOOTH_H

#include <cmath>
#include <vector>

#include <vclib/mesh/requirements.h>
#include <vclib/space/kd_tree.h>

namespace vcl {

namespace internal {

template<typename CoordType>
struct LaplacianInfo
{
	using ScalarType = CoordType::ScalarType;
	CoordType  sum;
	ScalarType cnt;
};

template<typename MeshType, typename CoordType>
void accumulateLaplacianInfo(
	const MeshType&                        m,
	std::vector<LaplacianInfo<CoordType>>& data,
	bool                                   cotangentFlag = false)
{
	using ScalarType = CoordType::ScalarType;
	using VertexType = MeshType::VertexType;
	using FaceType   = MeshType::FaceType;

	ScalarType weight = 1.0f;

	for (const FaceType& f : m.faces()) {
		for (uint j = 0; j < f.vertexNumber(); ++j) {
			if (!f.edgeOnBorder(j)) {
				const VertexType& v0 = *f.vertex(j);
				const VertexType& v1 = *f.vertexMod(j + 1);
				const VertexType& v2 = *f.vertexMod(j + 2);
				const CoordType&  p0 = v0.coord();
				const CoordType&  p1 = v1.coord();
				const CoordType&  p2 = v2.coord();
				if (cotangentFlag) {
					ScalarType angle = (p1 - p2).angle(p0 - p2);
					weight           = std::tan((M_PI * 0.5) - angle);
				}

				data[m.index(v0)].sum += f.vertexMod(j + 1)->coord() * weight;
				data[m.index(v1)].sum += f.vertex(j)->coord() * weight;
				data[m.index(v0)].cnt += weight;
				data[m.index(v1)].cnt += weight;
			}
		}
	}
	// si azzaera i dati per i vertici di bordo
	for (const FaceType& f : m.faces()) {
		for (uint j = 0; j < f.vertexNumber(); ++j) {
			if (f.edgeOnBorder(j)) {
				const VertexType& v0 = *f.vertex(j);
				const VertexType& v1 = *f.vertexMod(j + 1);
				const CoordType&  p0 = v0.coord();
				const CoordType&  p1 = v1.coord();
				data[m.index(v0)].sum    = p0;
				data[m.index(v1)].sum    = p1;
				data[m.index(v0)].cnt    = 1;
				data[m.index(v1)].cnt    = 1;
			}
		}
	}

	// se l'edge j e' di bordo si deve mediare solo con gli adiacenti
	for (const FaceType& f : m.faces()) {
		for (uint j = 0; j < f.vertexNumber(); ++j) {
			if (f.edgeOnBorder(j)) {
				const VertexType& v0 = *f.vertex(j);
				const VertexType& v1 = *f.vertexMod(j + 1);
				const CoordType&  p0 = v0.coord();
				const CoordType&  p1 = v1.coord();
				data[m.index(v0)].sum += p1;
				data[m.index(v1)].sum += p0;
				++data[m.index(v0)].cnt;
				++data[m.index(v1)].cnt;
			}
		}
	}
}

} // namespace internal

/**
 * @brief vertexCoordLaplacianSmoothing the classical Laplacian smoothing. Each
 * vertex is moved onto the average of the adjacent vertices.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *   - Faces
 *
 * @param m: the mesh that will be smoothed
 * @param step
 * @param smoothSelected
 * @param cotangentWeight
 */
template<FaceMeshConcept MeshType>
void laplacianSmoothing(
	MeshType& m,
	uint      step,
	bool      smoothSelected  = false,
	bool      cotangentWeight = false /*, vcl::CallBackPos *cb*/)
{
	using VertexType = MeshType::VertexType;
	using CoordType  = VertexType::CoordType;

	const internal::LaplacianInfo<CoordType> lpz = {CoordType(0, 0, 0), 0};

	for (uint i = 0; i < step; ++i) {
		std::vector<internal::LaplacianInfo<CoordType>> laplData(
			m.vertexContainerSize(), lpz);
		internal::accumulateLaplacianInfo(m, laplData, cotangentWeight);
		for (VertexType& v : m.vertices()) {
			if (laplData[m.index(v)].cnt > 0) {
				if (!smoothSelected || v.selected()) {
					v.coord() = (v.coord() + laplData[m.index(v)].sum) /
								(laplData[m.index(v)].cnt + 1);
				}
			}
		}
	}
}

template<FaceMeshConcept MeshType>
void taubinSmoothing(
	MeshType& m,
	uint      step,
	float     lambda,
	float     mu,
	bool      smoothSelected = false /*, vcl::CallBackPos *cb*/)
{
	using VertexType = MeshType::VertexType;
	using CoordType  = VertexType::CoordType;

	const internal::LaplacianInfo<CoordType> lpz = {CoordType(0, 0, 0), 0};


	for (uint i = 0; i < step; ++i) {
		std::vector<internal::LaplacianInfo<CoordType>> laplData(
			m.vertexContainerSize(), lpz);
		internal::accumulateLaplacianInfo(m, laplData);
		for (VertexType& v : m.vertices()) {
			if (laplData[m.index(v)].cnt > 0) {
				if (!smoothSelected || v.selected()) {
					CoordType delta =
						laplData[m.index(v)].sum / laplData[m.index(v)].cnt -
						v.coord();
					v.coord() = v.coord() + delta * lambda;
				}
			}
		}
		std::fill(laplData.begin(), laplData.end(), lpz);
		internal::accumulateLaplacianInfo(m, laplData);
		for (VertexType& v : m.vertices()) {
			if (laplData[m.index(v)].cnt > 0) {
				if (!smoothSelected || v.selected()) {
					CoordType delta =
						laplData[m.index(v)].sum / laplData[m.index(v)].cnt -
						v.coord();
					v.coord() = v.coord() + delta * mu;
				}
			}
		}
	}
}

/**
 * @brief smoothPerVertexNormalsPointCloud
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Normal
 *
 * @param m
 * @param neighborNum
 * @param iterNum
 */
template<MeshConcept MeshType>
void smoothPerVertexNormalsPointCloud(
	MeshType& m,
	uint      neighborNum,
	uint      iterNum)
{
	using Scalar = MeshType::VertexType::CoordType::ScalarType;
	KDTree<Scalar> tree(m);
	//TODO
	updatePerVertexNormalsPointCloud(m, tree, neighborNum, iterNum);
}

/**
 * @brief smoothPerVertexNormalsPointCloud
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Normal
 *
 * @param m
 * @param tree
 * @param neighborNum
 * @param iterNum
 */
template<MeshConcept MeshType, PointConcept PointType>
void smoothPerVertexNormalsPointCloud(
	MeshType&             m,
	const KDTree<PointType>& tree,
	uint                  neighborNum,
	uint                  iterNum)
{
	vcl::requirePerVertexNormal(m);

	using Scalar     = PointType::ScalarType;
	using VertexType = MeshType::VertexType;
	using NormalType = VertexType::NormalType;

	std::vector<NormalType> TD(m.vertexContainerSize(), NormalType(0,0,0));

	for (uint ii = 0; ii < iterNum; ++ii){
		for (const VertexType& v : m.vertices()) {
			std::vector<Scalar> distances;

			std::vector<uint> neighbors = tree.kNearestNeighborsIndices(
				v.coord(), neighborNum, distances);

			for (uint nid : neighbors){
				if (m.vertex(nid).normal() * v.normal() > 0) {
					TD[m.index(v)] += m.vertex(nid).normal();
				}
				else {
					TD[m.index(v)] -= m.vertex(nid).normal();
				}
			}
		}
		for (VertexType& v : m.vertices()){
			v.normal() = TD[m.index(v)];
		}
	}
}

} // namespace vcl

#endif // VCL_ALGORITHMS_SMOOTH_H
