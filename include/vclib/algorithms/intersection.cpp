/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include "intersection.h"

namespace vcl {

template<EdgeMeshConcept EdgeMesh, FaceMeshConcept MeshType, typename PlaneType>
void meshPlaneIntersection(EdgeMesh& em, const MeshType& m, const PlaneType& pl)
{
	using VertexType = typename MeshType::VertexType;
	using FaceType   = typename MeshType::FaceType;
	using CoordType  = typename VertexType::CoordType;

	std::vector<double> qH(m.vertexContainerSize());

	for (const VertexType& v : m.vertices())
		qH[m.index(v)] = pl.dist(v.coord());

	for (const FaceType& f : m.faces()) {
		std::vector<CoordType>  ptVec;
		std::vector<CoordType> nmVec;
		for (uint j = 0; j < f.vertexNumber(); ++j) {
			if (qH[m.index(f.vertex(j))] == 0) {
				ptVec.push_back(f.vertex(j)->coord());
				if constexpr(HasPerVertexNormal<MeshType>) {
					if (isPerVertexNormalEnabled(m)) {
						nmVec.push_back(f.vertex(j)->normal());
					}
				}
			}
			else if ((qH[m.index(f.vertex(j))] * qH[m.index(f.vertexMod(j + 1))]) < 0) {
				const CoordType&  p0 = f.vertex(j)->coord();
				const CoordType&  p1 = f.vertexMod(j + 1)->coord();
				float             q0 = qH[m.index(f.vertex(j))];
				float             q1 = qH[m.index(f.vertexMod(j + 1))];

				std::pair<CoordType, CoordType> seg(p0, p1);
				CoordType                       pp = pl.segmentIntersection(seg);
				ptVec.push_back(pp);
				if constexpr(HasPerVertexNormal<MeshType>) {
					if (isPerVertexNormalEnabled(m)) {
						using NormalType = typename VertexType::NormalType;
						const NormalType& n0 = f.vertex(j)->normal();
						const NormalType& n1 = f.vertexMod(j + 1)->normal();
						CoordType nn = (n0 * fabs(q1) + n1 * fabs(q0)) / fabs(q0 - q1);
						nmVec.push_back(nn);
					}
				}
			}
		}
		if (ptVec.size() >= 2) {
			uint eid               = em.addEdge();
			uint v0                = em.addVertices(2);
			uint v1                = v0 + 1;
			em.vertex(v0).coord()  = ptVec[0];
			em.vertex(v1).coord()  = ptVec[1];
			em.edge(eid).vertex(0) = &em.vertex(v0);
			em.edge(eid).vertex(1) = &em.vertex(v1);
			if constexpr (HasPerVertexNormal<MeshType> && HasPerVertexNormal<EdgeMesh>) {
				if (isPerVertexNormalEnabled(m) && isPerVertexNormalEnabled(em)) {
					em.vertex(v0).normal() = nmVec[0];
					em.vertex(v1).normal() = nmVec[1];
				}
			}
		}
	}

//	// Clean-up: Remove duplicate vertex
//	tri::Clean<EdgeMeshType>::RemoveDuplicateVertex(em);

//	// Clean-up: Sort edges ensuring orientation coherence
//	for (size_t j = 1; j < em.edge.size(); j++) {
//		auto& n = em.edge[j - 1].V(1);
//		for (size_t i = j; i < em.edge.size(); i++) {
//			auto& ei = em.edge[i];
//			if (ei.V(1) == n)
//				std::swap(ei.V(0), ei.V(1));
//			if (ei.V(0) == n) {
//				std::swap(em.edge[j], em.edge[i]);
//				break;
//			}
//		}
	//	}
}

} // namespace vcl
