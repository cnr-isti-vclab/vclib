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

#include "filter.h"

namespace vcl {

/**
 * @brief Generates and returns a new mesh that is composed of the vertices of
 * the input mesh `m` filtered using the `vertexFilterRng` range.
 *
 * Only the vertices having the corresponding boolean in `vertexFilterRng`
 * evaluated to `true` will be put in the output mesh. The order of the vertices
 * in the output mesh is preserved.
 *
 * By default, the type of the output mesh will be the same of the input mesh
 * type.
 *
 * @tparam InMeshType: type of the input mesh. It must satisfy the
 * `MeshConcept`.
 * @tparam OutMeshType: type of the output mesh. It must satisfy the
 * `MeshConcept`. By default, it is the same of the input mesh type.
 *
 * @param[in] m: input mesh
 * @param[in] vertexFilterRng: range of values that are evaluated as booleans,
 * one for each vertex of the input mesh.
 * @param[in] saveBirthIndicesInCustomComponent: if `true` (default), and if the
 * output mesh type has the per vertex CustomComponents component, will set a
 * per vertex custom component of type `uint` in the output mesh telling, for
 * each vertex, the index of its birth vertex in the input mesh. The name of the
 * custom component is `"birthVertex"`.
 *
 * @return A new Mesh created by filtering the vertices of the input mesh `m`.
 */
template<MeshConcept InMeshType, MeshConcept OutMeshType>
OutMeshType perVertexMeshFilter(
	const InMeshType& m,
	Range auto&& vertexFilterRng,
	bool saveBirthIndicesInCustomComponent)
{
	OutMeshType res;
	res.enableSameOptionalComponentsOf(m);

	// enable the custom component birthVertex
	if constexpr (vcl::HasPerVertexCustomComponents<OutMeshType>) {
		if (saveBirthIndicesInCustomComponent) {
			res.template addPerVertexCustomComponent<uint>("birthVertex");
		}
	}

	for (const auto& [birthV, filter] : c9::zip(m.vertices(), vertexFilterRng))
	{
		if (filter) {
			uint v = res.addVertex();
			// import all the components from the input mesh
			res.vertex(v).importFrom(birthV);
			// set the birth vertex
			if constexpr (vcl::HasPerVertexCustomComponents<OutMeshType>) {
				if (saveBirthIndicesInCustomComponent) {
					res.vertex(v).template customComponent<uint>(
						"birthVertex") = m.index(birthV);
				}
			}
		}
	}

	return res;
}

} // namespace vcl
