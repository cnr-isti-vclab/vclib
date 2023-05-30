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

#include "element.h"

namespace vcl {

/**
 * @brief The Face class represents an Face element of the vcl::Mesh class.
 *
 * Using the FaceContainer class, it is possible to add a vector of Face elements to a mesh, and
 * manage them with the member functions exposed by the FaceContainer. Each Face element exposes
 * all the member functions of its Component types.
 *
 * @tparam MeshType The type of the parent mesh.
 * @tparam Comps The types of the components of the element.
 *
 * @ingroup elements
 */
template<typename MeshType, typename... Comps>
class Face : public Element<FACE, MeshType, Comps...>
{
	// VertexPointers component of the Face
	using VPtrs = typename Face::VertexPointers;

	static const int NV = VPtrs::VERTEX_NUMBER; // If dynamic, NV will be -1

public:
	using VertexType = typename VPtrs::VertexType;

	Face();

	Face(const std::vector<VertexType*>& list); // todo add requires

	template<typename... V>
	Face(V... args); // todo add requires

	void setVertices(const std::vector<VertexType*>& list);

	template<typename... V>
	void setVertices(V... args);

	void resizeVertices(uint n) requires PolygonFaceConcept<Face>;

	void pushVertex(VertexType* v) requires PolygonFaceConcept<Face>;

	void insertVertex(uint i, VertexType* v) requires PolygonFaceConcept<Face>;

	void eraseVertex(uint i) requires PolygonFaceConcept<Face>;

	void clearVertices() requires PolygonFaceConcept<Face>;

private:
	template<typename Comp>
	void resizeTTVNComponent(uint n);

	template<typename Comp>
	void pushBackTTVNComponent();

	template<typename Comp>
	void insertTTVNComponent(uint i);

	template<typename Comp>
	void eraseTTVNComponent(uint i);

	template<typename Comp>
	void clearTTVNComponent();
};

template<typename MeshType, typename... Comps>
class Face<MeshType, TypeWrapper<Comps...>> : public Face<MeshType, Comps...>
{
};

} // namespace vcl

#include "face.cpp"

#endif // VCL_MESH_ELEMENTS_FACE_H
