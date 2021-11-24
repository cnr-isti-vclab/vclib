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

#ifndef VCL_MESH_ELEMENTS_FACE_H
#define VCL_MESH_ELEMENTS_FACE_H

#include "face_components.h"
#include "face_components_optional.h"

namespace vcl::mesh {

// FaceContainer class declaration
template<typename>
class FaceContainer;

} // namespace vcl::mesh

namespace vcl {

// Dummy class used to detect a Face regardless of its template arguments
class FaceTriggerer
{
};

template<typename... Args>
class Face : public FaceTriggerer, public Args...
{
	template<typename>
	friend class mesh::FaceContainer;

	// Vertex references component of the Face
	using VRefs = typename Face::VertexReferences;
	using VertexType = typename VRefs::VertexType;

	static const int NV = VRefs::VERTEX_NUMBER; // If dynamic, NV will be -1

public:
	Face() {}

	Face(const std::vector<VertexType*>& list);

	template<typename... V>
	Face(V... args);

	void setVertices(const std::vector<VertexType*>& list);

	template<typename... V>
	void setVertices(V... args);

	template<int M = NV>
	VCL_ENABLE_IF(M < 0, void) resizeVertices(unsigned int n);

	template<int M = NV>
	VCL_ENABLE_IF(M < 0, void) pushVertex(VertexType* v);

	template<int M = NV>
	VCL_ENABLE_IF(M < 0, void) insertVertex(unsigned int i, VertexType* v);

	template<int M = NV>
	VCL_ENABLE_IF(M < 0, void) eraseVertex(unsigned int i);

	template<int M = NV>
	VCL_ENABLE_IF(M < 0, void) clearVertices();
};

} // namespace vcl

#include "face.cpp"

#endif // VCL_MESH_ELEMENTS_FACE_H
