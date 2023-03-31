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

#include "face.h"

namespace vcl {

template<typename MeshType, typename... Args>
Face<MeshType, Args...>::Face()
{
}

/**
 * @brief Constructs a Face with the given set of vertices.
 *
 * Sets a list of Vertex references to the face. If the Face size is non-dcel dynamic, will
 * take care to update the also the number of adjacent faces and the number of wedge components, if
 * these components are part of the Face and if the size of the Face is changed. On the contrary, if
 * the Face size is static or it belongs to a Dcel mesh, the number of vertices of the list must be
 * equal to the size of the Face (the value returned by vertexNumber()).
 *
 * @note This constructor cannot be called if the Face type belongs to a Dcel mesh.
 *
 * @param[in] list: a container of vertex pointers in counterclockwise order that will be set as
 *                  vertices of the face.
 */
template<typename MeshType, typename... Args>
Face<MeshType, Args...>::Face(const std::vector<VertexType*>& list) // todo add requires
{
	setVertices(list);
}

template<typename MeshType, typename... Args>
uint Face<MeshType, Args...>::index() const
{
	return Element<MeshType, Args...>::template index<typename MeshType::FaceType>();
}

/**
 * @brief Sets a list of Vertex references to the face. If the Face size is non-dcel dynamic, will
 * take care to update the also the number of adjacent faces and the number of wedge components, if
 * these components are part of the Face and if the size of the Face is changed. On the contrary, if
 * the Face size is static or it belongs to a Dcel mesh, the number of arguments of this function
 * must be equal to the size of the Face (the value returned by vertexNumber()).
 *
 * @note This constructor cannot be called if the Face type belongs to a Dcel mesh.
 *
 * @param[in] args: a variable number of vertex pointers in counterclockwise order that will be set
 *                  as vertices of the face.
 */
template<typename MeshType, typename... Args>
template<typename... V>
Face<MeshType, Args...>::Face(V... args) // todo add requires
{
	setVertices({args...});
}

/**
 * @brief Sets a list of Vertex references to the face. If the Face size is non-dcel dynamic, will
 * take care to update the also the number of adjacent faces and the number of wedge components, if
 * these components are part of the Face and if the size of the Face is changed. On the contrary, if
 * the Face size is static or it belongs to a Dcel mesh, the number of vertices of the list must be
 * equal to the size of the Face (the value returned by vertexNumber()).
 *
 * @param[in] list: a container of vertex pointers in counterclockwise order that will be set as
 *                  vertices of the face.
 */
template<typename MeshType, typename... Args>
void Face<MeshType, Args...>::setVertices(const std::vector<VertexType*>& list)
{
	using F = Face<MeshType, TypeWrapper<Args...>>;

	VRefs::setVertices(list);

	if constexpr (comp::HasAdjacentEdges<F> && NonDcelPolygonFaceConcept<F>) {
		using T = typename F::AdjacentEdges;

		if (T::isAdjEdgesEnabled())
			T::resizeAdjEdges(list.size());
	}

	if constexpr (face::HasAdjacentFaces<F> && NonDcelPolygonFaceConcept<F>) {
		using T = typename F::AdjacentFaces;

		if (T::isAdjFacesEnabled())
			T::resizeAdjFaces(list.size());
	}

	if constexpr (face::HasWedgeColors<F> && NonDcelPolygonFaceConcept<F>) {
		using T = typename F::WedgeColors;

		if (T::isWedgeColorsEnabled())
			T::resizeWedgeColors(list.size());
	}

	if constexpr (face::HasWedgeTexCoords<F> && NonDcelPolygonFaceConcept<F>) {
		using T = typename F::WedgeTexCoords;

		if (T::isWedgeTexCoordsEnabled())
			T::resizeWedgeTexCoords(list.size());
	}
}

template<typename MeshType, typename... Args>
template<typename... V>
void Face<MeshType, Args...>::setVertices(V... args)
{
	setVertices({args...});
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
template<typename MeshType, typename... Args>
void Face<MeshType, Args...>::resizeVertices(uint n)
	requires NonDcelPolygonFaceConcept<Face<MeshType, Args...>>
{
	using F = Face<MeshType, TypeWrapper<Args...>>;

	VRefs::resizeVertices(n);

	if constexpr (face::HasAdjacentEdges<F>) {
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

template<typename MeshType, typename... Args>
void Face<MeshType, Args...>::pushVertex(VertexType* v) requires NonDcelPolygonFaceConcept<Face<MeshType, Args...>>
{
	using F = Face<MeshType, TypeWrapper<Args...>>;

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

template<typename MeshType, typename... Args>
void Face<MeshType, Args...>::insertVertex(uint i, VertexType* v) requires NonDcelPolygonFaceConcept<Face<MeshType, Args...>>
{
	using F = Face<MeshType, TypeWrapper<Args...>>;

	VRefs::insertVertex(i, v);

	if constexpr (face::HasAdjacentEdges<F>) {
		using T = typename F::AdjacentEdges;

		if (T::isAdjEdgesEnabled())
			T::insertAdjEdge(i, nullptr);
	}

	if constexpr (face::HasAdjacentFaces<F>) {
		using T = typename F::AdjacentFaces;

		if (T::isAdjFacesEnabled())
			T::insertAdjFace(i, nullptr);
	}

	if constexpr (face::HasWedgeColors<F>) {
		using T = typename F::WedgeColors;

		if (T::isWedgeColorsEnabled())
			T::insertWedgeColor(i, Color());
	}

	if constexpr (face::HasWedgeTexCoords<F>) {
		using S = typename F::WedgeTexCoordScalarType;
		using T = typename F::WedgeTexCoords;

		if (T::isWedgeTexCoordsEnabled())
			T::insertWedgeTexCoord(i, TexCoord<S>());
	}
}

template<typename MeshType, typename... Args>
void Face<MeshType, Args...>::eraseVertex(uint i) requires NonDcelPolygonFaceConcept<Face<MeshType, Args...>>
{
	using F = Face<MeshType, TypeWrapper<Args...>>;

	VRefs::eraseVertex(i);

	if constexpr (face::HasAdjacentEdges<F>) {
		using T = typename F::AdjacentEdges;

		if (T::isAdjEdgesEnabled())
			T::eraseAdjEdge(i);
	}

	if constexpr (face::HasAdjacentFaces<F>) {
		using T = typename F::AdjacentFaces;

		if (T::isAdjFacesEnabled())
			T::eraseAdjFace(i);
	}

	if constexpr (face::HasWedgeColors<F>) {
		using T = typename F::WedgeColors;

		if (T::isWedgeColorsEnabled())
			T::eraseWedgeColor(i);
	}

	if constexpr (face::HasWedgeTexCoords<F>) {
		using T = typename F::WedgeTexCoords;

		if (T::isWedgeTexCoordsEnabled())
			T::eraseWedgeTexCoord(i);
	}
}

template<typename MeshType, typename... Args>
void Face<MeshType, Args...>::clearVertices() requires NonDcelPolygonFaceConcept<Face<MeshType, Args...>>
{
	using F = Face<MeshType, TypeWrapper<Args...>>;

	VRefs::clearVertices();

	if constexpr (face::HasAdjacentEdges<F>) {
		using T = typename F::AdjacentEdges;

		if (T::isAdjEdgesEnabled())
			T::clearAdjEdges();
	}

	if constexpr (face::HasAdjacentFaces<F>) {
		using T = typename F::AdjacentFaces;

		if (T::isAdjFacesEnabled())
			T::clearAdjFaces();
	}

	if constexpr (face::HasWedgeColors<F>) {
		using T = typename F::WedgeColors;

		if (T::isWedgeColorsEnabled())
			T::clearWedgeColor();
	}

	if constexpr (face::HasWedgeTexCoords<F>) {
		using T = typename F::WedgeTexCoords;

		if (T::isWedgeTexCoordsEnabled())
			T::clearWedgeTexCoord();
	}
}

template<typename MeshType, typename... Args>
typename Face<MeshType, Args...>::VertexCoordIterator Face<MeshType, Args...>::vertexCoordBegin()
{
	return VertexCoordIterator(VRefs::vertexBegin());
}

template<typename MeshType, typename... Args>
typename Face<MeshType, Args...>::ConstVertexCoordIterator
Face<MeshType, Args...>::vertexCoordBegin() const
{
	return ConstVertexCoordIterator(VRefs::vertexBegin());
}

template<typename MeshType, typename... Args>
typename Face<MeshType, Args...>::VertexCoordIterator Face<MeshType, Args...>::vertexCoordEnd()
{
	return VertexCoordIterator(VRefs::vertexEnd());
}

template<typename MeshType, typename... Args>
typename Face<MeshType, Args...>::ConstVertexCoordIterator
Face<MeshType, Args...>::vertexCoordEnd() const
{
	return ConstVertexCoordIterator(VRefs::vertexEnd());
}

template<typename MeshType, typename... Args>
typename Face<MeshType, Args...>::VertexCoordView Face<MeshType, Args...>::vertexCoords()
{
	return VertexCoordView(vertexCoordBegin(), vertexCoordEnd());
}

template<typename MeshType, typename... Args>
typename Face<MeshType, Args...>::ConstVertexCoordView
Face<MeshType, Args...>::vertexCoords() const
{
	return ConstVertexCoordView(vertexCoordBegin(), vertexCoordEnd());
}

} // namespace vcl


