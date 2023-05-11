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
 * @brief The Face class
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

	// TODO: move definition in face.cpp when Clang bug will be solved
	// https://stackoverflow.com/questions/72897153/outside-class-definition-of-member-function-enabled-with-concept
	void resizeVertices(uint n) requires NonDcelPolygonFaceConcept<Face>
	{
		VPtrs::resizeVertices(n);

		// Now I need to resize all the TTVN components
		(resizeTTVNComponent<Comps>(n), ...);
	}

	// TODO: move definition in face.cpp when Clang bug will be solved
	// https://stackoverflow.com/questions/72897153/outside-class-definition-of-member-function-enabled-with-concept
	void pushVertex(VertexType* v) requires NonDcelPolygonFaceConcept<Face>
	{
		VPtrs::pushVertex(v);

		// Now I need to pushBack in all the TTVN components
		(pushBackTTVNComponent<Comps>(), ...);
	}

	// TODO: move definition in face.cpp when Clang bug will be solved
	// https://stackoverflow.com/questions/72897153/outside-class-definition-of-member-function-enabled-with-concept
	void insertVertex(uint i, VertexType* v) requires NonDcelPolygonFaceConcept<Face>
	{
		VPtrs::insertVertex(i, v);

		// Now I need to insert in all the TTVN components
		(insertTTVNComponent<Comps>(i), ...);
	}

	// TODO: move definition in face.cpp when Clang bug will be solved
	// https://stackoverflow.com/questions/72897153/outside-class-definition-of-member-function-enabled-with-concept
	void eraseVertex(uint i) requires NonDcelPolygonFaceConcept<Face>
	{
		VPtrs::eraseVertex(i);

		// Now I need to erase in all the TTVN components
		(eraseTTVNComponent<Comps>(i), ...);
	}

	// TODO: move definition in face.cpp when Clang bug will be solved
	// https://stackoverflow.com/questions/72897153/outside-class-definition-of-member-function-enabled-with-concept
	void clearVertices() requires NonDcelPolygonFaceConcept<Face>
	{
		VPtrs::clearVertices();

		// Now I need to clear all the TTVN components
		(clearTTVNComponent<Comps>(), ...);
	}

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
