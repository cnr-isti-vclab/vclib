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

#include "vertex_pointers.h"

namespace vcl::comp {

/**
 * @private
 * @brief Initializes the container of vertices to nullptr, or an empty
 * container if the container is dynamic.
 *
 * It is made in the init function for sake of compatibility with the other
 * components.
 *
 * This member function is hidden by the element that inherits this class.
 */
template<typename Vertex, int N, typename El>
void VertexPointers<Vertex, N, El>::init()
{
	Base::init(this);
}

/**
 * @private
 * @brief Returns `true` if the component is enabled, `false` otherwise.
 * This member function can return `false` only if the component is optional
 * (impossible in this case).
 *
 * This member function is hidden by the element that inherits this class.
 *
 * @return `true` if the component is enabled, `false` otherwise.
 */
template<typename Vertex, int N, typename El>
bool VertexPointers<Vertex, N, El>::isEnabled()
{
	return Base::isEnabled(this);
}

/**
 * @brief Returns the number of vertices of the element.
 * @return The number of vertices of the element.
 */
template<typename Vertex, int N, typename El>
uint VertexPointers<Vertex, N, El>::vertexNumber() const
{
	return Base::container(this).size();
}

/**
 * @brief Returns a reference of the pointer to the i-th vertex of the element.
 *
 * You can use this function to set the i-th vertex:
 *
 * @code{.cpp}
 * f.vertex(2) = &m.vertex(k); // the second vertex of f will point to
 *                             // the k-th vertex of the mesh m.
 * @endcode
 *
 * @param[in] i: the position of the required vertex in the container.
 * @return The pointer i-th vertex of the element.
 */
template<typename Vertex, int N, typename El>
Vertex*& VertexPointers<Vertex, N, El>::vertex(uint i)
{
	return Base::container(this).at(i);
}

/**
 * @brief Returns a const pointer to the i-th vertex of the element.
 * @param[in] i: the position of the required vertex in the container;
 * the value must be between 0 and the number of vertices.
 * @return The pointer to the i-th vertex of the element.
 */
template<typename Vertex, int N, typename El>
const Vertex* VertexPointers<Vertex, N, El>::vertex(uint i) const
{
	return Base::container(this).at(i);
}

/**
 * @brief Returns a reference of the pointer to the i-th vertex of the element,
 * but using as index the module between i and the number of vertices.
 * You can use this function if you need to get the "next vertex after position
 * k", without check if it is less than the number of vertices. Works also for
 * negative numbers:
 *
 * @code{.cpp}
 * k = pos; // some position of a vertex
 * auto* next = e.vertexMod(k+1); // the vertex next to k, that may also
 *                                // be at pos 0
 * auto* last = e.vertexMod(-1); // the vertex in position vertexNumber()-1
 * @endcode
 *
 * @param[in] i: the position of the required vertex in the container,
 * w.r.t. the position 0; value is modularized on vertexNumber().
 * @return The pointer to the required vertex of the element.
 */
template<typename Vertex, int N, typename El>
Vertex*& VertexPointers<Vertex, N, El>::vertexMod(int i)
{
	return Base::container(this).atMod(i);
}

/**
 * @brief Same of vertexMod, but returns a const pointer to the vertex.
 * @param[in] i: the position of the required vertex in the container,
 * w.r.t. the position 0; value is modularized on vertexNumber().
 * @return The pointer to the required vertex of the element.
 */
template<typename Vertex, int N, typename El>
const Vertex* VertexPointers<Vertex, N, El>::vertexMod(int i) const
{
	return Base::container(this).atMod(i);
}

template<typename Vertex, int N, typename El>
void VertexPointers<Vertex, N, El>::setVertex(Vertex* v, uint i)
{
	Base::container(this).set(v, i);
}

template<typename Vertex, int N, typename El>
void VertexPointers<Vertex, N, El>::setVertices(const std::vector<Vertex*>& list)
{
	Base::container(this).set(list);
}

template<typename Vertex, int N, typename El>
bool VertexPointers<Vertex, N, El>::containsVertex(const Vertex* v) const
{
	return Base::container(this).contains(v);
}

template<typename Vertex, int N, typename El>
typename VertexPointers<Vertex, N, El>::VertexIterator
VertexPointers<Vertex, N, El>::findVertex(const Vertex* v)
{
	return Base::container(this).find(v);
}

template<typename Vertex, int N, typename El>
typename VertexPointers<Vertex, N, El>::ConstVertexIterator
VertexPointers<Vertex, N, El>::findVertex(const Vertex* v) const
{
	return Base::container(this).find(v);
}

template<typename Vertex, int N, typename El>
uint VertexPointers<Vertex, N, El>::indexOfVertex(const Vertex* v) const
{
	return Base::container(this).indexOf(v);
}

template<typename Vertex, int N, typename El>
uint VertexPointers<Vertex, N, El>::indexOfEdge(const Vertex* v1, const Vertex* v2) const
{
	uint vid = indexOfVertex(v1);
	if (vid == UINT_NULL) {
		return UINT_NULL;
	}
	else if (vertexMod(vid+1) == v2) {
		return vid;
	}
	else if (vertexMod((int)vid-1) == v2) {
		uint n = vertexNumber();
		return ((vid-1) % n + n) % n;
	}
	else {
		return UINT_NULL;
	}
}

template<typename Vertex, int N, typename El>
void VertexPointers<Vertex, N, El>::resizeVertices(uint n) requires (N < 0)
{
	Base::container(this).resize(n);
}

template<typename Vertex, int N, typename El>
void VertexPointers<Vertex, N, El>::pushVertex(Vertex* v) requires (N < 0)
{
	Base::container(this).pushBack(v);
}

template<typename Vertex, int N, typename El>
void VertexPointers<Vertex, N, El>::insertVertex(uint i, Vertex* v) requires (N < 0)
{
	Base::container(this).insert(i, v);
}

template<typename Vertex, int N, typename El>
void VertexPointers<Vertex, N, El>::eraseVertex(uint i) requires (N < 0)
{
	Base::container(this).erase(i);
}

template<typename Vertex, int N, typename El>
void VertexPointers<Vertex, N, El>::clearVertices() requires (N < 0)
{
	Base::container(this).clear();
}

template<typename Vertex, int N, typename El>
typename VertexPointers<Vertex, N, El>::VertexIterator VertexPointers<Vertex, N, El>::vertexBegin()
{
	return Base::container(this).begin();
}

template<typename Vertex, int N, typename El>
typename VertexPointers<Vertex, N, El>::VertexIterator VertexPointers<Vertex, N, El>::vertexEnd()
{
	return Base::container(this).end();
}

template<typename Vertex, int N, typename El>
typename VertexPointers<Vertex, N, El>::ConstVertexIterator
VertexPointers<Vertex, N, El>::vertexBegin() const
{
	return Base::container(this).begin();
}

template<typename Vertex, int N, typename El>
typename VertexPointers<Vertex, N, El>::ConstVertexIterator
VertexPointers<Vertex, N, El>::vertexEnd() const
{
	return Base::container(this).end();
}

template<typename Vertex, int N, typename El>
auto VertexPointers<Vertex, N, El>::vertices()
{
	return View(vertexBegin(), vertexEnd());
}

template<typename Vertex, int N, typename El>
auto VertexPointers<Vertex, N, El>::vertices() const
{
	return View(vertexBegin(), vertexEnd());
}

template<typename Vertex, int N, typename El>
template<typename Element>
void VertexPointers<Vertex, N, El>::importFrom(const Element&)
{
}

template<typename Vertex, int N, typename El>
template<typename Element, typename ElVType>
void VertexPointers<Vertex, N, El>::importPointersFrom(
	const Element& e,
	Vertex* base,
	const ElVType* ebase)
{
	if constexpr (HasVertexPointers<Element>) {
		if constexpr(N > 0) {
			// same size non-polygonal faces
			if constexpr (N == Element::VERTEX_NUMBER) {
				importPtrsFrom(e, base, ebase);
			}
			// from polygonal to fixed size, but the polygon size == the fixed face size
			else if constexpr (Element::VERTEX_NUMBER < 0){
				if (e.vertexNumber() == N) {
					importPtrsFrom(e, base, ebase);
				}
			}
			else {
				// do not import in this case: cannot import from a face of different size
			}
		}
		else {
			// from fixed to polygonal size: need to resize first, then import
			resizeVertices(e.vertexNumber());
			importPtrsFrom(e, base, ebase);
		}
	}
}

template<typename Vertex, int N, typename El>
void VertexPointers<Vertex, N, El>::updatePointers(
	const Vertex* oldBase, const Vertex* newBase)
{
	Base::updateElementPointers(oldBase, newBase, this);
}

template<typename Vertex, int N, typename El>
void VertexPointers<Vertex, N, El>::updatePointers(
	const Vertex*           base,
	const std::vector<uint>& newIndices)
{
	Base::updateElementPointers(base, newIndices, this);
}

template<typename Vertex, int N, typename El>
template<typename Element, typename ElVType>
void VertexPointers<Vertex, N, El>::importPtrsFrom(
	const Element& e,
	Vertex* base,
	const ElVType* ebase)
{
	if (ebase != nullptr && base != nullptr) {
		for (uint i = 0; i < e.vertexNumber(); ++i){
			if (e.vertex(i) != nullptr){
				vertex(i) = base + (e.vertex(i) - ebase);
			}
		}
	}
}

} // namespace vcl::comp
