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

#include "generate.h"

namespace vcl {

/**
 * @brief Returns a vector of boolean values. For each ith vertex in the mesh, the corresponding
 * value in the vector will be true if the vertex is selected, false otherwise.
 *
 * @param m: input mesh.
 * @return
 */
template<MeshConcept MeshType>
std::vector<bool> boolVectorFromVertexSelection(const MeshType& m)
{
	using VertexType = typename MeshType::VertexType;

	std::vector<bool> res(m.vertexContainerSize(), false);
	for (const VertexType& v : m.vertices()) {
		if (v.isSelected())
			res[m.index(v)] = true;
	}
	return res;
}

/**
 * @brief Returns a vector of boolean values. For each ith face in the mesh, the corresponding
 * value in the vector will be true if the face is selected, false otherwise.
 * @param m
 * @return
 */
template<FaceMeshConcept MeshType>
std::vector<bool> boolVectorFromFaceSelection(const MeshType& m)
{
	using FaceType = typename MeshType::FaceType;

	std::vector<bool> res(m.faceContainerSize(), false);
	for (const FaceType& f : m.faces()) {
		if (f.isSelected())
			res[m.index(f)] = true;
	}
	return res;
}

/**
 * @brief Generates and returns a new mesh that is composed of the vertices of the input mesh `m`
 * having their value in the input vector of booleans `vec` set to true.
 *
 * By default, the type of the output mesh will be the same of the input mesh type.
 * @param[in] m: input mesh
 * @param[in] vec: vector of booleans that will tell which vertices put in the output mesh.
 * @param[in] saveBirthVertexInCustomComponent: if `true` (default), and if the output mesh type has
 * the per vertex CustomComponents component, will set a per vertex custom component of type `uint`
 * in the output mesh telling, for each vertex, the index of its birth vertex in the input mesh.
 * @return
 */
template<MeshConcept InMeshType, MeshConcept OutMeshType>
OutMeshType generateMeshFromVertexBoolVector(
	const InMeshType&  m,
	std::vector<bool>& vec,
	bool               saveBirthVertexInCustomComponent)
{
	using InVertexType = typename InMeshType::VertexType;
	using OutVertexType = typename OutMeshType::VertexType;

	assert(vec.size() == m.vertexContainerSize());

	OutMeshType res;
	res.enableSameOptionalComponentsOf(m);

	// enable the custom component birthVertex
	if constexpr (vcl::HasPerVertexCustomComponents<OutMeshType>) {
		if (saveBirthVertexInCustomComponent) {
			res.template addPerVertexCustomComponent<uint>("birthVertex");
		}
	}

	for (uint i = 0; i < vec.size(); ++i) {
		if (vec[i]) {
			uint v = res.addVertex();
			res.vertex(v).importFrom(m.vertex(i)); // import all the components from the input mesh
			if constexpr (vcl::HasPerVertexCustomComponents<OutMeshType>) { // set the birth vertex
				if (saveBirthVertexInCustomComponent) {
					res.vertex(v).template customComponent<uint>("birthVertex") = i;
				}
			}
		}
	}

	return res;
}

} // namespace vcl
