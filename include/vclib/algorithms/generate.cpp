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
 * @param[in] saveBirthIndicesInCustomComponent: if `true` (default), and if the output mesh type
 * has the per vertex CustomComponents component, will set a per vertex custom component of type
 * `uint` in the output mesh telling, for each vertex, the index of its birth vertex in the input
 * mesh. The name of the custom component is `"birthVertex"`.
 * @return
 */
template<MeshConcept InMeshType, MeshConcept OutMeshType>
OutMeshType generateMeshFromVertexBoolVector(
	const InMeshType&  m,
	std::vector<bool>& vec,
	bool               saveBirthIndicesInCustomComponent)
{
	assert(vec.size() == m.vertexContainerSize());

	OutMeshType res;
	res.enableSameOptionalComponentsOf(m);

	// enable the custom component birthVertex
	if constexpr (vcl::HasPerVertexCustomComponents<OutMeshType>) {
		if (saveBirthIndicesInCustomComponent) {
			res.template addPerVertexCustomComponent<uint>("birthVertex");
		}
	}

	for (uint i = 0; i < vec.size(); ++i) {
		if (vec[i]) {
			uint v = res.addVertex();
			res.vertex(v).importFrom(m.vertex(i)); // import all the components from the input mesh
			if constexpr (vcl::HasPerVertexCustomComponents<OutMeshType>) { // set the birth vertex
				if (saveBirthIndicesInCustomComponent) {
					res.vertex(v).template customComponent<uint>("birthVertex") = i;
				}
			}
		}
	}

	return res;
}

/**
 * @brief Generates and returns a new mesh that is composed of the faces of the input mesh `m`
 * having their value in the input vector of booleans `vec` set to true. Only vertices belonging to
 * the imported faces will be imported in the output mesh.
 *
 * By default, the type of the output mesh will be the same of the input mesh type.
 * @param[in] m: input mesh
 * @param[in] vec: vector of booleans that will tell which faces put in the output mesh.
 * @param[in] saveBirthIndicesInCustomComponent: if `true` (default), and if the output mesh type
 * has the per vertex and/or per face CustomComponents component, will set a per vertex/per face
 * custom component of type `uint` in the output mesh telling, for each vertex/face, the index of
 * its birth vertex/birth face in the input mesh. The names of the custom components are
 * `"birthVertex"` and `"birthFace"`.
 * @return
 */
template<MeshConcept InMeshType, MeshConcept OutMeshType>
OutMeshType generateMeshFromFaceBoolVector(
	const InMeshType& m,
	std::vector<bool>& vec,
	bool saveBirthIndicesInCustomComponent)
{
	using InVertexType = typename InMeshType::VertexType;

	assert(vec.size() == m.faceContainerSize());

	OutMeshType res;
	res.enableSameOptionalComponentsOf(m);

	// enable the custom component birthVertex
	if constexpr (vcl::HasPerVertexCustomComponents<OutMeshType>) {
		if (saveBirthIndicesInCustomComponent) {
			res.template addPerVertexCustomComponent<uint>("birthVertex");
		}
	}

	// enable the custom component birthFace
	if constexpr (vcl::HasPerFaceCustomComponents<OutMeshType>) {
		if (saveBirthIndicesInCustomComponent) {
			res.template addPerFaceCustomComponent<uint>("birthFace");
		}
	}

	std::vector<uint> vertexMapping(m.vertexContainerSize(), UINT_NULL);

	for (uint i = 0; i < vec.size(); ++i) {
		if (vec[i]) {
			std::vector<uint> verts(m.face(i).vertexNumber(), UINT_NULL);
			uint vi = 0; // incremented with vertices of the face
			// set all the vertex indices in the verts vector
			// two cases here:
			// - the ith vertex of the face has been already added, we need just to take its id in
			//   the out mesh from the vertexMapping vector
			// - the ith vertex of the face has not been added: we need to add it and import all
			//   its components, and update the vertexMappingVector
			for (const InVertexType* v : m.face(i).vertices()) {
				if (vertexMapping[m.index(v)] == UINT_NULL) { // the vertex has not already added
					// add the vertex to the out mesh
					uint ov = res.addVertex();
					res.vertex(ov).importFrom(*v); // import all the components from the input mesh
					if constexpr (vcl::HasPerVertexCustomComponents<OutMeshType>) { // set the birth vertex
						if (saveBirthIndicesInCustomComponent) {
							res.vertex(ov).template customComponent<uint>("birthVertex") = m.index(v);
						}
					}
					vertexMapping[m.index(v)] = ov;
					verts[vi] = ov;
				}
				else {
					verts[vi] = vertexMapping[m.index(v)];
				}
				++vi;
			}

			// now all the vertices of the face are in the out mesh, we can add the actual face
			uint f = res.addFace(verts.begin(), verts.end());
			res.face(f).importFrom(m.face(i)); // import all the components from the input mesh
			if constexpr (vcl::HasPerFaceCustomComponents<OutMeshType>) { // set the birth face
				if (saveBirthIndicesInCustomComponent) {
					res.face(f).template customComponent<uint>("birthFace") = i;
				}
			}
		}
	}
	return res;
}

} // namespace vcl
