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

#include "selection.h"

#include "../clean.h"

#include <vclib/concepts/range.h>

namespace vcl {

namespace internal {

template<vcl::Range Rng>
void clearSelection(Rng&& r)
{
	for (auto& e : r) {
		e.unsetSelected();
	}
}

} // namespace vcl::internal

template<MeshConcept MeshType>
void clearVertexSelection(MeshType& m)
{
	internal::clearSelection(m.vertices());
}

template<FaceMeshConcept MeshType>
void clearFaceSelection(MeshType& m)
{
	internal::clearSelection(m.faces());
}

template<EdgeMeshConcept MeshType>
void clearEdgeSelection(MeshType& m)
{
	internal::clearSelection(m.edges());
}

template<DcelMeshConcept MeshType>
void clearHalfEdgeSelection(MeshType& m)
{
	internal::clearSelection(m.halfEdges());
}

template<FaceMeshConcept MeshType>
void selectNonManifoldVertices(MeshType& m, bool clearSelectionFirst)
{
	std::vector<bool> nonManifoldVertices = internal::nonManifoldVerticesVectorBool(m);

	using VertexType = typename MeshType::VertexType;

	for (VertexType& v : m.vertices()) {
		if (nonManifoldVertices[m.index(v)]) {
			v.setSelected();
		}
		else if (clearSelectionFirst) {
			v.unsetSelected();
		}
	}
}

} // namespace vcl
