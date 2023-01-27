/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

template<DcelMeshConcept MeshType>
bool isPerHalfEdgeMarkEnabled(const MeshType& m)
{
	if constexpr (vcl::hedge::HasOptionalMark<typename MeshType::HalfEdgeType>) {
		return m.isPerHalfEdgeMarkEnabled();
	}
	else {
		return vcl::hedge::HasMark<typename MeshType::HalfEdgeType>;
	}
}

template<DcelMeshConcept MeshType>
bool enableIfPerHalfEdgeMarkOptional(MeshType& m)
{
	if constexpr (HasPerHalfEdgeMark<MeshType>) {
		if constexpr(vcl::hedge::HasOptionalMark<typename MeshType::HalfEdgeType>) {
			m.enablePerHalfEdgeMark();
		}
		return true;
	}
	else {
		return false;
	}
}

template<DcelMeshConcept MeshType>
bool isPerHalfEdgeScalarEnabled(const MeshType& m)
{
	if constexpr (vcl::hedge::HasOptionalScalar<typename MeshType::HalfEdgeType>) {
		return m.isPerHalfEdgeScalarEnabled();
	}
	else {
		return vcl::hedge::HasScalar<typename MeshType::HalfEdgeType>;
	}
}

template<DcelMeshConcept MeshType>
bool enableIfPerHalfEdgeScalarOptional(MeshType& m)
{
	if constexpr (HasPerHalfEdgeScalar<MeshType>) {
		if constexpr(vcl::hedge::HasOptionalScalar<typename MeshType::HalfEdgeType>) {
			m.enablePerHalfEdgeScalar();
		}
		return true;
	}
	else {
		return false;
	}
}

template<DcelMeshConcept MeshType>
bool isPerHalfEdgeTexCoordEnabled(const MeshType& m)
{
	if constexpr (vcl::hedge::HasOptionalTexCoord<typename MeshType::HalfEdgeType>) {
		return m.isPerHalfEdgeTexCoordEnabled();
	}
	else {
		return vcl::hedge::HasTexCoord<typename MeshType::HalfEdgeType>;
	}
}

template<DcelMeshConcept MeshType>
bool enableIfPerHalfEdgeTexCoordOptional(MeshType& m)
{
	if constexpr (HasPerHalfEdgeTexCoord<MeshType>) {
		if constexpr(vcl::hedge::HasOptionalTexCoord<typename MeshType::HalfEdgeType>) {
			m.enablePerHalfEdgeTexCoord();
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
		throw vcl::MissingComponentException("HalfEdge color not enabled.");
}

template<DcelMeshConcept MeshType>
void requirePerHalfEdgeMark(const MeshType& m)
	requires HasPerHalfEdgeMark<MeshType>
{
	if (!isPerHalfEdgeMarkEnabled(m))
		throw vcl::MissingComponentException("HalfEdge mark not enabled.");
}

template<DcelMeshConcept MeshType>
void requirePerHalfEdgeScalar(const MeshType& m)
	requires HasPerHalfEdgeScalar<MeshType>
{
	if (!isPerHalfEdgeScalarEnabled(m))
		throw vcl::MissingComponentException("HalfEdge scalar not enabled.");
}

template<DcelMeshConcept MeshType>
void requirePerHalfEdgeTexCoord(const MeshType& m)
	requires HasPerHalfEdgeTexCoord<MeshType>
{
	if (!isPerHalfEdgeTexCoordEnabled(m))
		throw vcl::MissingComponentException("HalfEdge TexCoord not enabled.");
}

} // namespace vcl
