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

/**
 * @brief The Face class
 *
 * @ingroup elements
 */
template<typename MeshType, typename... Args>
class Face : public Element<MeshType, Args...>
{
	template<FaceConcept>
	friend class mesh::FaceContainer;

	// VertexPointers component of the Face
	using VPtrs = typename Face::VertexPointers;

	static const int NV = VPtrs::VERTEX_NUMBER; // If dynamic, NV will be -1

public:
	static const uint ELEMENT_TYPE = FACE;

	using VertexType = typename VPtrs::VertexType;

	Face();

	Face(const std::vector<VertexType*>& list); // todo add requires

	template<typename... V>
	Face(V... args); // todo add requires

	uint index() const;

	void setVertices(const std::vector<VertexType*>& list);

	template<typename... V>
	void setVertices(V... args);

	// TODO: move definition in face.cpp when Clang bug will be solved
	// https://stackoverflow.com/questions/72897153/outside-class-definition-of-member-function-enabled-with-concept
	void resizeVertices(uint n) requires NonDcelPolygonFaceConcept<Face>
	{
		using F = Face<MeshType, TypeWrapper<Args...>>;

		VPtrs::resizeVertices(n);

		if constexpr (comp::HasPolygonBitFlags<F>) {
			using T = typename F::PolygonBitFlags;
			T::resize(n);
		}

		if constexpr (face::HasAdjacentEdges<F>) {
			using T = typename F::AdjacentEdges;

			if (T::isAdjEdgesEnabled())
				T::resize(n);
		}

		if constexpr (face::HasAdjacentFaces<F>) {
			using T = typename F::AdjacentFaces;

			if (T::isAdjFacesEnabled())
				T::resize(n);
		}

		if constexpr (face::HasWedgeColors<F>) {
			using T = typename F::WedgeColors;

			if (T::isWedgeColorsEnabled())
				T::resize(n);
		}

		if constexpr (face::HasWedgeTexCoords<F>) {
			using T = typename F::WedgeTexCoords;

			if (T::isWedgeTexCoordsEnabled())
				T::resize(n);
		}
	}

	// TODO: move definition in face.cpp when Clang bug will be solved
	// https://stackoverflow.com/questions/72897153/outside-class-definition-of-member-function-enabled-with-concept
	void pushVertex(VertexType* v) requires NonDcelPolygonFaceConcept<Face>
	{
		using F = Face<MeshType, TypeWrapper<Args...>>;

		VPtrs::pushVertex(v);

		if constexpr (comp::HasPolygonBitFlags<F>) {
			using T = typename F::PolygonBitFlags;
			T::pushBack();
		}

		if constexpr (face::HasAdjacentEdges<F>) {
			using T = typename F::AdjacentEdges;

			if (T::isAdjEdgesEnabled())
				T::pushBack();
		}

		if constexpr (face::HasAdjacentFaces<F>) {
			using T = typename F::AdjacentFaces;

			if (T::isAdjFacesEnabled())
				T::pushBack();
		}

		if constexpr (face::HasWedgeColors<F>) {
			using T = typename F::WedgeColors;

			if (T::isWedgeColorsEnabled())
				T::pushBack();
		}

		if constexpr (face::HasWedgeTexCoords<F>) {
			using T = typename F::WedgeTexCoords;

			if (T::isWedgeTexCoordsEnabled())
				T::pushBack();
		}
	}

	// TODO: move definition in face.cpp when Clang bug will be solved
	// https://stackoverflow.com/questions/72897153/outside-class-definition-of-member-function-enabled-with-concept
	void insertVertex(uint i, VertexType* v) requires NonDcelPolygonFaceConcept<Face>
	{
		using F = Face<MeshType, TypeWrapper<Args...>>;

		VPtrs::insertVertex(i, v);

		if constexpr (comp::HasPolygonBitFlags<F>) {
			using T = typename F::PolygonBitFlags;
			T::insert(i);
		}

		if constexpr (face::HasAdjacentEdges<F>) {
			using T = typename F::AdjacentEdges;

			if (T::isAdjEdgesEnabled())
				T::insert(i);
		}

		if constexpr (face::HasAdjacentFaces<F>) {
			using T = typename F::AdjacentFaces;

			if (T::isAdjFacesEnabled())
				T::insert(i);
		}

		if constexpr (face::HasWedgeColors<F>) {
			using T = typename F::WedgeColors;

			if (T::isWedgeColorsEnabled())
				T::insert(i);
		}

		if constexpr (face::HasWedgeTexCoords<F>) {
			using T = typename F::WedgeTexCoords;

			if (T::isWedgeTexCoordsEnabled())
				T::insert(i);
		}
	}

	// TODO: move definition in face.cpp when Clang bug will be solved
	// https://stackoverflow.com/questions/72897153/outside-class-definition-of-member-function-enabled-with-concept
	void eraseVertex(uint i) requires NonDcelPolygonFaceConcept<Face>
	{
		using F = Face<MeshType, TypeWrapper<Args...>>;

		VPtrs::eraseVertex(i);

		if constexpr (comp::HasPolygonBitFlags<F>) {
			using T = typename F::PolygonBitFlags;
			T::erase(i);
		}

		if constexpr (face::HasAdjacentEdges<F>) {
			using T = typename F::AdjacentEdges;

			if (T::isAdjEdgesEnabled())
				T::erase(i);
		}

		if constexpr (face::HasAdjacentFaces<F>) {
			using T = typename F::AdjacentFaces;

			if (T::isAdjFacesEnabled())
				T::erase(i);
		}

		if constexpr (face::HasWedgeColors<F>) {
			using T = typename F::WedgeColors;

			if (T::isWedgeColorsEnabled())
				T::erase(i);
		}

		if constexpr (face::HasWedgeTexCoords<F>) {
			using T = typename F::WedgeTexCoords;

			if (T::isWedgeTexCoordsEnabled())
				T::erase(i);
		}
	}

	// TODO: move definition in face.cpp when Clang bug will be solved
	// https://stackoverflow.com/questions/72897153/outside-class-definition-of-member-function-enabled-with-concept
	void clearVertices() requires NonDcelPolygonFaceConcept<Face>
	{
		using F = Face<MeshType, TypeWrapper<Args...>>;

		VPtrs::clearVertices();

		if constexpr (comp::HasPolygonBitFlags<F>) {
			using T = typename F::PolygonBitFlags;
			T::clear();
		}

		if constexpr (face::HasAdjacentEdges<F>) {
			using T = typename F::AdjacentEdges;

			if (T::isAdjEdgesEnabled())
				T::clear();
		}

		if constexpr (face::HasAdjacentFaces<F>) {
			using T = typename F::AdjacentFaces;

			if (T::isAdjFacesEnabled())
				T::clear();
		}

		if constexpr (face::HasWedgeColors<F>) {
			using T = typename F::WedgeColors;

			if (T::isWedgeColorsEnabled())
				T::clear();
		}

		if constexpr (face::HasWedgeTexCoords<F>) {
			using T = typename F::WedgeTexCoords;

			if (T::isWedgeTexCoordsEnabled())
				T::clear();
		}
	}

private:
	template<typename Comp>
	void resizeComponent(uint n)
	{
		if constexpr (comp::IsTiedToVertexNumber<Comp>) {
			if (Comp::isEnabled())
				Comp::resize(n);
		}
	}
};

template<typename MeshType, typename... Args>
class Face<MeshType, TypeWrapper<Args...>> : public Face<MeshType, Args...>
{
};

} // namespace vcl

#include "face.cpp"

#endif // VCL_MESH_ELEMENTS_FACE_H
