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

/**
 * @brief Empty constructor.
 *
 * Calls automatically all the empty constructors of all the components
 * available in the Face (for all the components non-available, their empty
 * constructor is called only when they become available).
 */
template<typename MeshType, typename... Comps>
Face<MeshType, Comps...>::Face()
{
}

/**
 * @brief Constructs a Face with the given set of vertex pointers.
 *
 * Sets a list of Vertex pointers to the face.
 * If the Face size is static, the number of vertices of the list must be equal
 * to the size of the Face (the value returned by vertexNumber()). If the Face
 * size is dynamic, it will take care to update the also the size of the
 * components tied to the vertex number of the face.
 *
 * @todo do proper checks on the number of vertices at compile time and at
 * runtime
 *
 * @param[in] list: a container of vertex pointers in counterclockwise order
 * that will be set as vertices of the face.
 */
template<typename MeshType, typename... Comps>
Face<MeshType, Comps...>::Face(const std::vector<VertexType*>& list) // TODO add requires
{
	setVertices(list);
}

/**
 * @brief Creates a new Face, setting a list of Vertex pointers to it.
 *
 * If the Face size is static, the number of vertices of the list must be equal
 * to the size of the Face (the value returned by vertexNumber()). If the Face
 * size is dynamic, it will take care to update the also the size of the
 * components tied to the vertex number of the face.
 *
 * @param[in] args: a variable number of vertex pointers in counterclockwise
 * order that will be set as vertices of the face.
 */
template<typename MeshType, typename... Comps>
template<typename... V>
Face<MeshType, Comps...>::Face(V... args) // TODO add requires
{
	setVertices({args...});
}

/**
 * @brief Sets a list of Vertex pointers to the face.
 *
 * If the Face size is static, the number of vertices of the list must be equal
 * to the size of the Face (the value returned by vertexNumber()). If the Face
 * size is dynamic, it will take care to update the also the size of the
 * components tied to the vertex number of the face.
 *
 * @param[in] list: a container of vertex pointers in counterclockwise order
 * that will be set as vertices of the face.
 */
template<typename MeshType, typename... Comps>
void Face<MeshType, Comps...>::setVertices(const std::vector<VertexType*>& list)
// TODO - use a view instead of vector of vertex pointers
{
	using F = Face<MeshType, TypeWrapper<Comps...>>;
	
	VPtrs::setVertices(list);

	// if polygonal, I need to resize all the TTVN components
	if constexpr (NV < 0) {
		(resizeTTVNComponent<Comps>(list.size()), ...);
	}
}

/**
 * @brief Sets a list of Vertex pointers to the face.
 *
 * If the Face size is static, the number of vertices of the list must be equal
 * to the size of the Face (the value returned by vertexNumber()). If the Face
 * size is dynamic, it will take care to update the also the size of the
 * components tied to the vertex number of the face.
 *
 * @param[in] args: a variable number of vertex pointers in counterclockwise
 * order that will be set as vertices of the face.
 */
template<typename MeshType, typename... Comps>
template<typename... V>
void Face<MeshType, Comps...>::setVertices(V... args)
{
	setVertices({args...});
}

/**
 * @brief Resize the number of Vertex Pointers of the Face, taking care of
 * updating also the other components of the Face that are tied to that number.
 *
 * If n is greater than the old number of vertex pointers, n vertex pointers
 * (and relative tied components) will be added. If n is lower than the old
 * number of vertex pointers, the difference of vertex pointers (and relative
 * tied components) will be removed.
 *
 * This member function is available only **if the face is polygonal (its size
 * is dynamic, N < 0)**.
 *
 * @param n: the new number of vertices.
 */
template<typename MeshType, typename... Comps>
void Face<MeshType, Comps...>::resizeVertices(uint n) requires PolygonFaceConcept<Face>
{
	VPtrs::resizeVertices(n);

	// Now I need to resize all the TTVN components
	(resizeTTVNComponent<Comps>(n), ...);
}

template<typename MeshType, typename... Comps>
void Face<MeshType, Comps...>::pushVertex(VertexType* v) requires PolygonFaceConcept<Face>
{
	VPtrs::pushVertex(v);

	// Now I need to pushBack in all the TTVN components
	(pushBackTTVNComponent<Comps>(), ...);
}

template<typename MeshType, typename... Comps>
void Face<MeshType, Comps...>::insertVertex(uint i, VertexType* v) requires PolygonFaceConcept<Face>
{
	VPtrs::insertVertex(i, v);

	// Now I need to insert in all the TTVN components
	(insertTTVNComponent<Comps>(i), ...);
}

template<typename MeshType, typename... Comps>
void Face<MeshType, Comps...>::eraseVertex(uint i) requires PolygonFaceConcept<Face>
{
	VPtrs::eraseVertex(i);

	// Now I need to erase in all the TTVN components
	(eraseTTVNComponent<Comps>(i), ...);
}

template<typename MeshType, typename... Comps>
void Face<MeshType, Comps...>::clearVertices() requires PolygonFaceConcept<Face>
{
	VPtrs::clearVertices();

	// Now I need to clear all the TTVN components
	(clearTTVNComponent<Comps>(), ...);
}

/**
 * Calls the resize(n) on all the component containers that are tied to the vertex number
 */
template<typename MeshType, typename... Comps>
template<typename Comp>
void Face<MeshType, Comps...>::resizeTTVNComponent(uint n)
{
	if constexpr (comp::IsTiedToVertexNumber<Comp>) {
		if (Comp::isAvailable())
			Comp::resize(n);
	}
}

/**
 * Calls the pushBack() on all the component containers that are tied to the
 * vertex number
 */
template<typename MeshType, typename... Comps>
template<typename Comp>
void Face<MeshType, Comps...>::pushBackTTVNComponent()
{
	if constexpr (comp::IsTiedToVertexNumber<Comp>) {
		if (Comp::isAvailable())
			Comp::pushBack();
	}
}

/**
 * Calls the insert(i) on all the component containers that are tied to the
 * vertex number
 */
template<typename MeshType, typename... Comps>
template<typename Comp>
void Face<MeshType, Comps...>::insertTTVNComponent(uint i)
{
	if constexpr (comp::IsTiedToVertexNumber<Comp>) {
		if (Comp::isAvailable())
			Comp::insert(i);
	}
}

/**
 * Calls the erase(i) on all the component containers that are tied to the
 * vertex number
 */
template<typename MeshType, typename... Comps>
template<typename Comp>
void Face<MeshType, Comps...>::eraseTTVNComponent(uint i)
{
	if constexpr (comp::IsTiedToVertexNumber<Comp>) {
		if (Comp::isAvailable())
			Comp::erase(i);
	}
}

/**
 * Calls the clear() on all the component containers that are tied to the vertex
 * number
 */
template<typename MeshType, typename... Comps>
template<typename Comp>
void Face<MeshType, Comps...>::clearTTVNComponent()
{
	if constexpr (comp::IsTiedToVertexNumber<Comp>) {
		if (Comp::isAvailable())
			Comp::clear();
	}
}

template<typename MeshType, typename... Comps>
template<typename ElType>
void Face<MeshType, Comps...>::importFrom(const ElType& v)
{
	if constexpr (comp::HasVertexPointers<ElType> && NV < 0) {
		VPtrs::resizeVertices(v.vertexNumber());
		// Now I need to resize all the TTVN components
		(resizeTTVNComponent<Comps>(v.vertexNumber()), ...);
	}

	Base::importFrom(v);
}

} // namespace vcl


