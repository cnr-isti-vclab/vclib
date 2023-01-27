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

#ifndef VCL_MESH_ELEMENTS_FACE_H
#define VCL_MESH_ELEMENTS_FACE_H

#include "face_concept.h"

namespace vcl::mesh {

// FaceContainer class declaration
template<FaceConcept>
class FaceContainer;

template<typename>
class ElementContainer;

} // namespace vcl::mesh

namespace vcl {

template<typename... Args>
class Face : public Args...
{
	template<FaceConcept>
	friend class mesh::FaceContainer;

	template<typename>
	friend class mesh::ElementContainer;

	// Vertex references component of the Face
	using VRefs = typename Face::VertexReferences;

	static const int NV = VRefs::VERTEX_NUMBER; // If dynamic, NV will be -1

public:
	using VertexType = typename VRefs::VertexType;

	Face();

	Face(const std::vector<VertexType*>& list); // todo add requires

	template<typename... V>
	Face(V... args); // todo add requires

	void setVertices(const std::vector<VertexType*>& list);

	template<typename... V>
	void setVertices(V... args);

	template<typename Element>
	void importFrom(const Element& f);

	void resizeVertices(uint n) requires PolygonFaceConcept<Face>;

	void pushVertex(VertexType* v) requires PolygonFaceConcept<Face>;

	void insertVertex(uint i, VertexType* v) requires PolygonFaceConcept<Face>;

	void eraseVertex(uint i) requires PolygonFaceConcept<Face>;

	void clearVertices() requires PolygonFaceConcept<Face>;
};

} // namespace vcl

#include "face.cpp"

#endif // VCL_MESH_ELEMENTS_FACE_H
