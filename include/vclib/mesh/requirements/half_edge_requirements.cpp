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

#include "vertex_requirements.h"

#include <vclib/exception/mesh_exception.h>

#include "../containers/half_edge_container.h"

namespace vcl {

/*************************
 * is/enableIf functions *
 *************************/

/**
 * @brief Returns `true` if the given mesh has its HalfEdge Container compact.
 * Returns `false` if the mesh has no HalfEdge Container.
 * @param[in] m: input mesh on which test if the its HalfEdge Container is compact.
 * @return `true` if the HalfEdge Container of the mesh is compact.
 */
template<DcelMeshConcept MeshType>
bool isHalfEdgeContainerCompact(const MeshType& m)
{
	if constexpr (HasHalfEdges<MeshType>) {
		return (m.halfEdgeNumber() == m.halfEdgeContainerSize());
	}
	else {
		return false;
	}
}

template<DcelMeshConcept MeshType>
bool isPerHalfEdgeColorEnabled(const MeshType& m)
{
	if constexpr (vcl::hedge::HasOptionalColor<typename MeshType::HalfEdgeType>) {
		return m.isPerHalfEdgeColorEnabled();
	}
	else {
		return vcl::hedge::HasColor<typename MeshType::HalfEdgeType>;
	}
}

template<DcelMeshConcept MeshType>
bool enableIfPerHalfEdgeColorOptional(MeshType& m)
{
	if constexpr (HasPerHalfEdgeColor<MeshType>) {
		if constexpr(vcl::hedge::HasOptionalColor<typename MeshType::HalfEdgeType>) {
			m.enablePerHalfEdgeColor();
		}
		return true;
	}
	else {
		return false;
	}
}

/*********************
 * require functions *
 *********************/

template<DcelMeshConcept MeshType>
void requireHalfEdgeContainerCompactness(const MeshType& m)
{
	if (!isHalfEdgeContainerCompact(m))
		throw vcl::MissingCompactnessException("HalfEdge Container of the Mesh is not compact.");
}

template<DcelMeshConcept MeshType>
void requirePerHalfEdgeColor(const MeshType& m)
	requires HasPerHalfEdgeColor<MeshType>
{
	if (!isPerHalfEdgeColorEnabled(m))
		throw vcl::MissingComponentException("HalfEdge colors not enabled.");
}

} // namespace vcl
