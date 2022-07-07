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

#include "face.h"

namespace vcl {

template<typename... Args>
Face<Args...>::Face()
{
}

template<typename... Args>
Face<Args...>::Face(const std::vector<VertexType*>& list)
{
	setVertices(list);
}

template<typename... Args>
template<typename... V>
Face<Args...>::Face(V... args)
{
	setVertices({args...});
}

/**
 * @brief Sets a list of Vertex references to the face. If the Face size is dynamic, will take care
 * to update the also the number of adjacent faces and the number of wedge components, if these
 * components are part of the Face and if the size of the Face is changed. On the contrary, if the
 * Face size is static, the number of vertices of the list must be equal to the size of the Face.
 *
 * @param list
 */
template<typename... Args>
void Face<Args...>::setVertices(const std::vector<VertexType*>& list)
{
	using F = Face<Args...>;

	VRefs::setVertices(list);

	static const int VN = F::VERTEX_NUMBER;
	if constexpr (VN < 0) {
		if constexpr (comp::hasAdjacentEdges<F>()) {
			using T = typename F::AdjacentEdges;

			if (T::isAdjEdgesEnabled())
				T::resizeAdjEdges(list.size());
		}

		if constexpr (face::hasAdjacentFaces<F>()) {
			using T = typename F::AdjacentFaces;

			if (T::isAdjFacesEnabled())
				T::resizeAdjFaces(list.size());
		}

		if constexpr (face::hasWedgeColors<F>()) {
			using T = typename F::WedgeColors;

			if (T::isWedgeColorsEnabled())
				T::resizeWedgeColors(list.size());
		}

		if constexpr (face::hasWedgeTexCoords<F>()) {
			using T = typename F::WedgeTexCoords;

			if (T::isWedgeTexCoordsEnabled())
				T::resizeWedgeTexCoords(list.size());
		}
	}
}

template<typename... Args>
template<typename... V>
void Face<Args...>::setVertices(V... args)
{
	setVertices({args...});
}

template<typename... Args>
template<typename Element>
void Face<Args...>::importFrom(const Element& f)
{
	(Args::importFrom(f), ...);
}

/**
 * @brief Resize the number of Vertex References of the Face, taking care of updating also the
 * number of adjacent faces and the number of wedge components of the Face, if these components
 * are part of the Face.
 *
 * If n is greater than the old number of vertex references, n vertex references (and relative
 * wedge components) will be added. If n is lower than the old number of vertex references, the
 * difference of vertex references (and relative wedge components) will be removed.
 *
 * This member function is available only **if the face is polygonal (its size is dynamic, N < 0)**.
 *
 * @param n: the new number of vertices.
 */
//TODO: Clang bug. Move definition here when it will be solved
//template<typename... Args>
//void Face<Args...>::resizeVertices(uint n) requires PolygonFaceConcept<Face<Args...>>
//{

//}

//TODO: Clang bug. Move definition here when it will be solved
//template<typename... Args>
//void Face<Args...>::pushVertex(VertexType* v) requires PolygonFaceConcept<Face<Args...>>
//{

//}

//TODO: Clang bug. Move definition here when it will be solved
//template<typename... Args>
//void Face<Args...>::insertVertex(uint i, VertexType* v) requires PolygonFaceConcept<Face<Args...>>
//{

//}

//TODO: Clang bug. Move definition here when it will be solved
//template<typename... Args>
//void Face<Args...>::eraseVertex(uint i) requires PolygonFaceConcept<Face<Args...>>
//{

//}

//TODO: Clang bug. Move definition here when it will be solved
//template<typename... Args>
//void Face<Args...>::clearVertices() requires PolygonFaceConcept<Face<Args...>>
//{

//}

} // namespace vcl
