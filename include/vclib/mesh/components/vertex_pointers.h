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

#ifndef VCL_MESH_COMPONENTS_VERTEX_POINTERS_H
#define VCL_MESH_COMPONENTS_VERTEX_POINTERS_H

#include <vclib/concepts/mesh/components/vertex_pointers.h>
#include <vclib/views/view.h>

#include "bases/pointers_container_component.h"

namespace vcl::comp {

/**
 * @brief The VertexPointers class represents a component that stores a
 * container of pointers to vertices that will be part of an Element (e.g.
 * Face, Edge, Tetrahedron, etc.).
 *
 * The container of Vertex pointers is a random access container that could have
 * static or dynamic size, depending on the value of the template argument N
 * (a negative number means dynamic).
 *
 * For example, if a Face element should represent only triangles, N could be
 * 3. In this case, the container will have static size and it can't be resized
 * in any way. If N is -1, the container will have dynamic size and it will be
 * possible to resize it (e.g. if the Face element should represent a polygon).
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component.
 *
 * For example, if you have a Face Element `f` that has the VertexPointers
 * component, you'll be able to access to this component member functions from
 * `f`:
 *
 * @code{.cpp}
 * uint n = f.vertexNumber();
 * auto* v = f.vertex(0);
 * @endcode
 *
 * @note This component is usually the main component of an Element, and
 * therefore it cannot be optional.
 *
 * @note Several components are *Tied To Vertex Number*: in other words, they
 * are composed by a container that has the same size of the VertexPointers
 * and, when the VertexPointers container is resized, also the container of
 * these components is resized automatically.
 *
 * @tparam Vertex The type of the vertices.
 * @tparam N: The size of the container, that will represent the number of
 * storable vertex pointers. If negative, the container is dynamic.
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 *
 * @ingroup components
 */
template<typename Vertex, int N, typename ElementType = void>
class VertexPointers :
		public PointersContainerComponent<
			VertexPointers<Vertex, N, ElementType>,
			VERTEX_PTRS,
			Vertex,
			N,
			ElementType,
			false,
			false>
{
	using Base = PointersContainerComponent<
		VertexPointers<Vertex, N, ElementType>,
		VERTEX_PTRS,
		Vertex,
		N,
		ElementType,
		false,
		false>;

public:
	/**
	 * @brief Expose the type of the Vertex.
	 */
	using VertexType = Vertex;

	static const int VERTEX_NUMBER = Base::SIZE;

	/* Iterator Types declaration */

	using VertexIterator      = typename Base::Iterator;
	using ConstVertexIterator = typename Base::ConstIterator;

	/* Constructor and isAvailable */

	void init();

	bool isAvailable();

	/* Member functions */

	uint vertexNumber() const;

	Vertex*&      vertex(uint i);
	const Vertex* vertex(uint i) const;
	Vertex*&      vertexMod(int i);
	const Vertex* vertexMod(int i) const;

	void setVertex(Vertex* v, uint i);
	void setVertices(const std::vector<Vertex*>& list);

	bool containsVertex(const Vertex* v) const;

	VertexIterator findVertex(const Vertex* v);
	ConstVertexIterator findVertex(const Vertex* v) const;

	uint indexOfVertex(const Vertex* v) const;
	uint indexOfEdge(const Vertex* v1, const Vertex* v2) const;

	/* Member functions specific for vector of pointers */

	void resizeVertices(uint n) requires (N < 0);
	void pushVertex(Vertex* v) requires (N < 0);
	void insertVertex(uint i, Vertex* v) requires (N < 0);
	void eraseVertex(uint i) requires (N < 0);
	void clearVertices() requires (N < 0);

	/* Iterator Member functions */

	VertexIterator           vertexBegin();
	VertexIterator           vertexEnd();
	ConstVertexIterator      vertexBegin() const;
	ConstVertexIterator      vertexEnd() const;
	auto                     vertices();
	auto                     vertices() const;

protected:
	// Component interface function
	template<typename Element>
	void importFrom(const Element& e);

	// PointersComponent interface functions
	template<typename Element, typename ElVType>
	void importPointersFrom(const Element& e, Vertex* base, const ElVType* ebase);

	void updatePointers(const Vertex* oldBase, const Vertex* newBase);

	void updatePointers(const Vertex* base, const std::vector<uint>& newIndices);

private:
	template<typename Element, typename ElVType>
	void importPtrsFrom(const Element& e, Vertex* base, const ElVType* ebase);
};

} // namespace vcl::comp

#include "vertex_pointers.cpp"

#endif // VCL_MESH_COMPONENTS_VERTEX_POINTERS_H
