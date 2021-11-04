/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_ELEMENT_REFERENCES_H
#define MGP_MESH_COMPONENTS_ELEMENT_REFERENCES_H

#include <array>
#include <assert.h>
#include <type_traits>
#include <vector>

#include "../iterators/range_iterator.h"

namespace mgp::components {

namespace internal {

template<int M, typename T>
using ReturnIfIsVector = typename std::enable_if<(M < 0), T>::type;
template<int M, typename T>
using ReturnIfIsArray = typename std::enable_if<(M >= 0), T>::type;

} // namespace internal

template<class Elem, int N>
class ElementReferences
{
private:
	// if we use the vector, the size of the array will be 0
	// actually the array will never be used and will not use memory, it's just for declaration
	static const int ARRAY_SIZE = N >= 0 ? N : 0;

	// the Container type will be array or vector, depending on N value
	using Container = typename std::conditional<
		(N >= 0),
		typename std::array<Elem*, ARRAY_SIZE>,
		typename std::vector<Elem*>>::type;

public:
	static const int COMPONENT_NUMBER = N;

	/** Iterator Types declaration **/

	// if using array, will be the array iterator, the vector iterator otherwise
	using ComponentIterator = typename std::conditional<
		(N >= 0),
		typename std::array<Elem*, ARRAY_SIZE>::iterator,
		typename std::vector<Elem*>::iterator>::type;

	using ConstComponentIterator = typename std::conditional<
		(N >= 0),
		typename std::array<Elem*, ARRAY_SIZE>::const_iterator,
		typename std::vector<Elem*>::const_iterator>::type;

	using ComponentRangeIterator = RangeIterator<ElementReferences, ComponentIterator>;
	using ConstComponentRangeIterator =
		ConstRangeIterator<ElementReferences, ConstComponentIterator>;

	/** Constructor **/

	ElementReferences();

	/** Member functions **/

	unsigned int elementNumber() const;
	unsigned int sizeMod(unsigned int i) const;

	Elem*&      element(unsigned int i);
	const Elem* element(unsigned int i) const;
	Elem*&      elementMod(int i);
	const Elem* elementMod(int i) const;

	void setElement(Elem* v, unsigned int i);
	void setElements(const std::vector<Elem*>& list);

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushElement(Elem* v);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> insertElement(unsigned int i, Elem* v);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseElement(unsigned int i);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> clearElements();

	/** Iterator Member functions **/

	ComponentIterator           elementBegin();
	ComponentIterator           elementEnd();
	ConstComponentIterator      elementBegin() const;
	ConstComponentIterator      elementEnd() const;
	ComponentRangeIterator      elementIterator();
	ConstComponentRangeIterator elementIterator() const;

protected:
	Container elemRefs;

	void updateElementReferences(const Elem* oldBase, const Elem* newBase);

	void updateElementReferencesAfterCompact(const Elem* base, const std::vector<int>& newIndices);
};

} // namespace mgp::components

#include "element_references.cpp"

#endif // MGP_MESH_COMPONENTS_ELEMENT_REFERENCES_H
