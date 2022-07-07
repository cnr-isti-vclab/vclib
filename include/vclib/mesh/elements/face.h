/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
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

	Face(const std::vector<VertexType*>& list);

	template<typename... V>
	Face(V... args);

	void setVertices(const std::vector<VertexType*>& list);

	template<typename... V>
	void setVertices(V... args);

	template<typename Element>
	void importFrom(const Element& f);

	// TODO: move definition in face.cpp when Clang bug will be solved
	// https://stackoverflow.com/questions/72897153/outside-class-definition-of-member-function-enabled-with-concept
	void resizeVertices(uint n) requires PolygonFaceConcept<Face>
	{
		using F = Face<Args...>;

		VRefs::resizeVertices(n);

		if constexpr (comp::HasAdjacentEdges<F>) {
			using T = typename F::AdjacentEdges;

			if (T::isAdjEdgesEnabled())
				T::resizeAdjEdges(n);
		}

		if constexpr (face::HasAdjacentFaces<F>) {
			using T = typename F::AdjacentFaces;

			if (T::isAdjFacesEnabled())
				T::resizeAdjFaces(n);
		}

		if constexpr (face::HasWedgeColors<F>) {
			using T = typename F::WedgeColors;

			if (T::isWedgeColorsEnabled())
				T::resizeWedgeColors(n);
		}

		if constexpr (face::HasWedgeTexCoords<F>) {
			using T = typename F::WedgeTexCoords;

			if (T::isWedgeTexCoordsEnabled())
				T::resizeWedgeTexCoords(n);
		}
	}

	void pushVertex(VertexType* v) requires PolygonFaceConcept<Face>
	{
		using F = Face<Args...>;

		VRefs::pushVertex(v);

		if constexpr (face::HasAdjacentEdges<F>) {
			using T = typename F::AdjacentEdges;

			if (T::isAdjEdgesEnabled())
				T::pushAdjEdge(nullptr);
		}

		if constexpr (face::HasAdjacentFaces<F>) {
			using T = typename F::AdjacentFaces;

			if (T::isAdjFacesEnabled())
				T::pushAdjFace(nullptr);
		}

		if constexpr (face::HasWedgeColors<F>) {
			using T = typename F::WedgeColors;

			if (T::isWedgeColorsEnabled())
				T::pushWedgeTexColors(Color());
		}

		if constexpr (face::HasWedgeTexCoords<F>) {
			using S = typename F::WedgeTexCoordScalarType;
			using T = typename F::WedgeTexCoords;

			if (T::isWedgeTexCoordsEnabled())
				T::pushWedgeTexCoord(TexCoord<S>());
		}
	}

	template<int M = NV>
	VCL_ENABLE_IF(M < 0, void) insertVertex(uint i, VertexType* v);

	template<int M = NV>
	VCL_ENABLE_IF(M < 0, void) eraseVertex(uint i);

	template<int M = NV>
	VCL_ENABLE_IF(M < 0, void) clearVertices();
};

} // namespace vcl

#include "face.cpp"

#endif // VCL_MESH_ELEMENTS_FACE_H
