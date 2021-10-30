/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "smooth.h"

#include <cmath>
#include <vector>

#include <mgp/mesh/requirements.h>

namespace mgp {

namespace internal {

template<typename CoordType>
struct LaplacianInfo
{
	using ScalarType = typename CoordType::ScalarType;
	CoordType  sum;
	ScalarType cnt;
};

template<typename MeshType, typename CoordType>
void accumulateLaplacianInfo(
	const MeshType&                        m,
	std::vector<LaplacianInfo<CoordType>>& data,
	bool                                   cotangentFlag = false)
{
	using ScalarType = typename CoordType::ScalarType;
	using VertexType = typename MeshType::Vertex;
	using FaceType   = typename MeshType::Face;

	ScalarType weight = 1.0f;

	for (const FaceType& f : m.faceIterator()) {
		for (int j = 0; j < f.vertexNumber(); ++j) {
			if (!f.isEdgeOnBorder(j)) {
				const VertexType& v0 = *f.v(j);
				const VertexType& v1 = *f.v(f.sizeMod(j + 1));
				const VertexType& v2 = *f.v(f.sizeMod(j + 2));
				const CoordType&  p0 = v0.coordinate();
				const CoordType&  p1 = v1.coordinate();
				const CoordType&  p2 = v2.coordinate();
				if (cotangentFlag) {
					ScalarType angle = (p1 - p2).angle(p0 - p2);
					weight           = std::tan((M_PI * 0.5) - angle);
				}

				data[v0.id()].sum += f.v(f.sizeMod(j + 1))->coordinate() * weight;
				data[v1.id()].sum += f.v(j)->coordinate() * weight;
				data[v0.id()].cnt += weight;
				data[v1.id()].cnt += weight;
			}
		}
	}
	// si azzaera i dati per i vertici di bordo
	for (const FaceType& f : m.faceIterator()) {
		for (int j = 0; j < f.vertexNumber(); ++j) {
			if (f.isEdgeOnBorder(j)) {
				const VertexType& v0 = *f.v(j);
				const VertexType& v1 = *f.v(f.sizeMod(j + 1));
				const CoordType&  p0 = v0.coordinate();
				const CoordType&  p1 = v1.coordinate();
				data[v0.id()].sum    = p0;
				data[v1.id()].sum    = p1;
				data[v0.id()].cnt    = 1;
				data[v1.id()].cnt    = 1;
			}
		}
	}

	// se l'edge j e' di bordo si deve mediare solo con gli adiacenti
	for (const FaceType& f : m.faceIterator()) {
		for (int j = 0; j < f.vertexNumber(); ++j) {
			if (f.isEdgeOnBorder(j)) {
				const VertexType& v0 = *f.v(j);
				const VertexType& v1 = *f.v(f.sizeMod(j + 1));
				const CoordType&  p0 = v0.coordinate();
				const CoordType&  p1 = v1.coordinate();
				data[v0.id()].sum += p1;
				data[v1.id()].sum += p0;
				++data[v0.id()].cnt;
				++data[v1.id()].cnt;
			}
		}
	}
}

} // namespace internal

template<typename MeshType>
void vertexCoordLaplacianSmoothing(
	MeshType&    m,
	unsigned int step,
	bool         smoothSelected,
	bool         cotangentWeight /*, mgp::CallBackPos *cb*/)
{
	mgp::requireVertices(m);
	mgp::requireFaces(m);

	using VertexType                                    = typename MeshType::Vertex;
	using CoordType                                     = typename VertexType::CoordinateType;
	internal::LaplacianInfo<CoordType>              lpz = {CoordType(0, 0, 0), 0};
	std::vector<internal::LaplacianInfo<CoordType>> laplData(m.vertexContainerSize());

	for (unsigned int i = 0; i < step; ++i) {
		std::fill(laplData.begin(), laplData.end(), lpz);
		internal::accumulateLaplacianInfo(m, laplData, cotangentWeight);
		for (VertexType& v : m.vertexIterator()) {
			if (laplData[v.id()].cnt > 0) {
				if (!smoothSelected || v.isSelected()) {
					v.coordinate() =
						(v.coordinate() + laplData[v.id()].sum) / (laplData[v.id()].cnt + 1);
				}
			}
		}
	}
}

} // namespace mgp
