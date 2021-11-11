/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_OPTIONAL_VERTEX_REFERENCES_H
#define MGP_MESH_COMPONENTS_OPTIONAL_VERTEX_REFERENCES_H

#include <array>
#include <assert.h>
#include <type_traits>
#include <vector>

#include "optional_info.h"

#include "../iterators/range_iterator.h"

namespace mgp::mesh {
template <typename, typename>
class OptionalVertexReferencesVector;
}

namespace mgp::comp {

namespace internal {

template<int M, typename T>
using ReturnIfIsVector = typename std::enable_if<(M < 0), T>::type;
template<int M, typename T>
using ReturnIfIsArray = typename std::enable_if<(M >= 0), T>::type;

} // namespace internal

template<typename T>
class OptionalVertexReferencesTriggerer
{
};

template<class Vertex, int N, typename T>
class OptionalVertexReferences :
		public OptionalVertexReferencesTriggerer<T>,
		public virtual OptionalInfo<T>
{
	template <typename, typename>
	friend class OptionalVertexReferencesVector;

private:
	using B = OptionalInfo<T>;

	// if we use the vector, the size of the array will be 0
	// actually the array will never be used and will not use memory, it's just for declaration
	static const int ARRAY_SIZE = N >= 0 ? N : 0;

public:
	// the VertRefsContainer type will be array or vector, depending on N value
	using VertRefsContainer = typename std::conditional<
		(N >= 0),
		typename std::array<Vertex*, ARRAY_SIZE>,
		typename std::vector<Vertex*>>::type;

	static const int FACE_NUMBER = N;

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

	using VertexRangeIterator = RangeIterator<OptionalVertexReferences, VertexIterator>;
	using ConstVertexRangeIterator = ConstRangeIterator<OptionalVertexReferences, ConstVertexIterator>;

	/** Constructor **/

	OptionalVertexReferences();

	/** Member functions **/

	unsigned int vertexNumber() const;

	Vertex*&      v(unsigned int i);
	const Vertex* v(unsigned int i) const;
	Vertex*&      vMod(int i);
	const Vertex* vMod(int i) const;

	void setVertex(Vertex* f, unsigned int i);
	void setVertexs(const std::vector<Vertex*>& list);

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushVertex(Vertex* f);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> insertVertex(unsigned int i, Vertex* f);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseVertex(unsigned int i);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> clearVertexs();

	/** Iterator Member functions **/

	VertexIterator vertexBegin();
	VertexIterator vertexEnd();
	ConstVertexIterator vertexBegin() const;
	ConstVertexIterator vertexEnd() const;
	VertexRangeIterator vertexIterator();
	ConstVertexRangeIterator vertexIterator() const;

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
using hasOptionalVertexReferencesT = std::is_base_of<OptionalVertexReferencesTriggerer<T>, T>;

template<typename U, typename T>
using ReturnIfHasOptionalVertexReferences=
	typename std::enable_if<hasOptionalVertexReferencesT<U>::value, T>::type;

template<typename T>
bool constexpr hasOptionalVertexReferences()
{
	return hasOptionalVertexReferencesT<T>::value;
}

} // namespace mgp::comp

#include "optional_vertex_references.cpp"

#endif // MGP_MESH_COMPONENTS_OPTIONAL_VERTEX_REFERENCES_H
