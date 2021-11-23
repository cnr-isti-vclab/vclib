/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#ifndef VCL_MESH_MESH_H
#define VCL_MESH_MESH_H

#include "mesh_argument.h"
#include "mesh_components.h"

namespace vcl {

/*
 * The Mesh class represents a generic 3D mesh. A mesh is composed of a generic number of
 * containers of Elements (which can be vertices, faces, edges...), plus some other components.
 *
 * The Mesh class will expose all the public members of its containers, and its role is to implement
 * all the functionalities that allow these containers to comunicate (e.g. an operation on the
 * vertex container that requires to update also some face information).
 *
 * Therefore, in this class there is only the implementation of all the functions that in some way
 * need to modify the status of more than one Container of the Mesh. Other functions are inherited
 * by the Container classes of the Elements of the Mesh, or from its Components, and depend on all
 * the templates that compose a specific Mesh definition.
 */
template<typename... Args>
class Mesh : public mesh::Argument<Args>...
{
public:
	Mesh();
	Mesh(const Mesh& oth);
	Mesh(Mesh&& oth);

	void clear();

	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, unsigned int>
	index(const typename Mesh::VertexType& v) const;

	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, unsigned int>
	index(const typename Mesh::VertexType* v) const;

	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, unsigned int> addVertex();

	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, unsigned int>
	addVertex(const typename Mesh::VertexType::CoordType& p);

	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, unsigned int> addVertices(unsigned int n);

	template<typename... VC, typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, unsigned int> addVertices(const VC&... v);

	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, void> reserveVertices(unsigned int n);

	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, void> compactVertices();

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, unsigned int>
	index(const typename Mesh::FaceType& f) const;

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, unsigned int>
	index(const typename Mesh::FaceType* f) const;

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, unsigned int> addFace();

	template<typename U = Mesh, typename... V>
	mesh::ReturnIfHasFaceContainer<U, unsigned int> addFace(V... args);

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, unsigned int>
	addFace(const std::vector<typename Mesh::VertexType*>& v);

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, unsigned int> addFaces(unsigned int n);

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, void> reserveFaces(unsigned int n);

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, void> compactFaces();

	/// @private
	template<typename... A>
	friend void swap(Mesh<A...>& m1, Mesh<A...>& m2);

	void swap(Mesh& m2);

	Mesh& operator=(Mesh oth);

protected:
	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, void> updateVertexReferences(
		const typename U::VertexType* oldBase,
		const typename U::VertexType* newBase);

	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, void> updateVertexReferencesAfterCompact(
		const typename U::VertexType* base,
		const std::vector<int>&       newIndices);

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, void>
	updateFaceReferences(const typename U::FaceType* oldBase, const typename U::FaceType* newBase);

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, void> updateFaceReferencesAfterCompact(
		const typename U::FaceType* base,
		const std::vector<int>&     newIndices);

	void updateAllOptionalContainerReferences();
};

template<typename... A>
inline void swap(Mesh<A...>& m1, Mesh<A...>& m2);

} // namespace vcl

#include "mesh.cpp"

#endif // VCL_MESH_MESH_H
