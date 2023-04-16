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

#ifndef VCL_MESH_ELEMENTS_FACE_H
#define VCL_MESH_ELEMENTS_FACE_H

#include <vclib/concepts/mesh/elements/face.h>
#include <vclib/views/view.h>
#include <vclib/space/tex_coord.h>

#include "element.h"

namespace vcl::mesh {

// FaceContainer class declaration
template<FaceConcept>
class FaceContainer;

} // namespace vcl::mesh

namespace vcl {

template<typename MeshType, typename... Args>
class Face : public Element<MeshType, Args...>
{
	template<FaceConcept>
	friend class mesh::FaceContainer;

	// Vertex references component of the Face
	using VRefs = typename Face::VertexReferences;

	static const int NV = VRefs::VERTEX_NUMBER; // If dynamic, NV will be -1

public:
	static const uint ELEMENT_TYPE = FACE;

	using VertexType = typename VRefs::VertexType;

	Face();

	Face(const std::vector<VertexType*>& list); // todo add requires

	template<typename... V>
	Face(V... args); // todo add requires

	uint index() const;

	void setVertices(const std::vector<VertexType*>& list);

	template<typename... V>
	void setVertices(V... args);

	void resizeVertices(uint n) requires NonDcelPolygonFaceConcept<Face>;

	void pushVertex(VertexType* v) requires NonDcelPolygonFaceConcept<Face>;

	void insertVertex(uint i, VertexType* v) requires NonDcelPolygonFaceConcept<Face>;

	void eraseVertex(uint i) requires NonDcelPolygonFaceConcept<Face>;

	void clearVertices() requires NonDcelPolygonFaceConcept<Face>;
};

template<typename MeshType, typename... Args>
class Face<MeshType, TypeWrapper<Args...>> : public Face<MeshType, Args...>
{
};

} // namespace vcl

#include "face.cpp"

#endif // VCL_MESH_ELEMENTS_FACE_H
