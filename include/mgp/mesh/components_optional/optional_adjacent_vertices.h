/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_OPTIONAL_ADJACENT_VERTICES_H
#define MGP_MESH_COMPONENTS_OPTIONAL_ADJACENT_VERTICES_H

#include <array>
#include <assert.h>
#include <type_traits>
#include <vector>

#include "optional_info.h"

#include "../iterators/range_iterator.h"

namespace mgp::mesh {
template<typename, typename>
class OptionalVertexReferencesVector;
}

namespace mgp::comp {

namespace internal {

template<int M, typename T>
using ReturnIfIsVector = typename std::enable_if<(M < 0), T>::type;
template<int M, typename T>
using ReturnIfIsArray = typename std::enable_if<(M >= 0), T>::type;

} // namespace internal

class OptionalAdjacentVerticesTriggerer
{
};

template<typename Vertex, int N, typename T>
class OptionalAdjacentVertices :
		public OptionalAdjacentVerticesTriggerer,
		public virtual OptionalInfo<T>
{
	template<typename, typename>
	friend class OptionalVertexReferencesVector;

private:
	using B = OptionalInfo<T>;

	// if we use the vector, the size of the array will be 0
	// actually the array will never be used and will not use memory, it's just for declaration
	static const int ARRAY_SIZE = N >= 0 ? N : 0;

public:
	// the AdjVertsContainer type will be array or vector, depending on N value
	using AdjVertsContainer = typename std::conditional<
		(N >= 0),
		typename std::array<Vertex*, ARRAY_SIZE>,
		typename std::vector<Vertex*>>::type;

	static const int ADJ_VERTEX_NUMBER = N;

	/** Iterator Types declaration **/

	// if using array, will be the array iterator, the vector iterator otherwise
	using VertexIterator = typename std::conditional<
		(N >= 0),
		typename std::array<Vertex*, ARRAY_SIZE>::iterator,
		typename std::vector<Vertex*>::iterator>::type;

	using ConstVertexIterator = typename std::conditional<
		(N >= 0),
		typename std::array<Vertex*, ARRAY_SIZE>::const_iterator,
		typename std::vector<Vertex*>::const_iterator>::type;

	using VertexRangeIterator = RangeIterator<OptionalAdjacentVertices, VertexIterator>;
	using ConstVertexRangeIterator =
		ConstRangeIterator<OptionalAdjacentVertices, ConstVertexIterator>;

	/** Constructor **/

	OptionalAdjacentVertices();

	/** Member functions **/

	unsigned int adjVerticesNumber() const;

	Vertex*&      adjVert(unsigned int i);
	const Vertex* adjVert(unsigned int i) const;
	Vertex*&      adjVertMod(int i);
	const Vertex* adjVertMod(int i) const;

	void setAdjVertex(Vertex* f, unsigned int i);
	void setAdjVertices(const std::vector<Vertex*>& list);

	bool containsAdjVertex(const Vertex* v) const;

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> resizeAdjVertices(unsigned int n);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushAdjVertex(Vertex* f);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> insertAdjVertex(unsigned int i, Vertex* f);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseAdjVertex(unsigned int i);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> clearAdjVertices();

	/** Iterator Member functions **/

	VertexIterator           adjVertexBegin();
	VertexIterator           adjVertexEnd();
	ConstVertexIterator      adjVertexBegin() const;
	ConstVertexIterator      adjVertexEnd() const;
	VertexRangeIterator      adjVertices();
	ConstVertexRangeIterator adjVertices() const;

protected:
	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase);

	void updateVertexReferencesAfterCompact(const Vertex* base, const std::vector<int>& newIndices);

private:
	unsigned int thisId() const { return ((T*) this)->id(); }
};

/**
 * Detector to check if a class has (inherits) OptionalVertexReferences
 */

template<typename T>
using hasOptionalAdjacentVerticesT = std::is_base_of<OptionalAdjacentVerticesTriggerer, T>;

template<typename U, typename T>
using ReturnIfHasOptionalAdjacentVertices =
	typename std::enable_if<hasOptionalAdjacentVerticesT<U>::value, T>::type;

template<typename T>
bool constexpr hasOptionalAdjacentVertices()
{
	return hasOptionalAdjacentVerticesT<T>::value;
}

} // namespace mgp::comp

#include "optional_adjacent_vertices.cpp"

#endif // MGP_MESH_COMPONENTS_OPTIONAL_ADJACENT_VERTICES_H
