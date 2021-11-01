/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENT_REFERENCES_H
#define MGP_MESH_COMPONENT_REFERENCES_H

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

template<class Component, int N>
class ComponentReferences
{
private:
	// if we use the vector, the size of the array will be 0
	// actually the array will never be used and will not use memory, it's just for declaration
	static const int ARRAY_SIZE = N >= 0 ? N : 0;

	// the Container type will be array or vector, depending on N value
	using Container = typename std::conditional<
		(N >= 0),
		typename std::array<Component*, ARRAY_SIZE>,
		typename std::vector<Component*>>::type;

public:
	static const int COMPONENT_NUMBER = N;

	/** Iterator Types declaration **/

	// if using array, will be the array iterator, the vector iterator otherwise
	using ComponentIterator = typename std::conditional<
		(N >= 0),
		typename std::array<Component*, ARRAY_SIZE>::iterator,
		typename std::vector<Component*>::iterator>::type;

	using ConstComponentIterator = typename std::conditional<
		(N >= 0),
		typename std::array<Component*, ARRAY_SIZE>::const_iterator,
		typename std::vector<Component*>::const_iterator>::type;

	using ComponentRangeIterator = RangeIterator<ComponentReferences, ComponentIterator>;
	using ConstComponentRangeIterator =
		ConstRangeIterator<ComponentReferences, ConstComponentIterator>;

	/** Constructor **/

	ComponentReferences();

	/** Member functions **/

	unsigned int componentNumber() const;
	unsigned int sizeMod(unsigned int i) const;

	Component*&      c(unsigned int i);
	const Component* c(unsigned int i) const;

	void setComponent(Component* v, unsigned int i);
	void setComponents(const std::vector<Component*>& list);

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushComponent(Component* v);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> insertComponent(unsigned int i, Component* v);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseComponent(unsigned int i);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> clearComponents();

	/** Iterator Member functions **/

	ComponentIterator           componentBegin();
	ComponentIterator           componentEnd();
	ConstComponentIterator      componentBegin() const;
	ConstComponentIterator      componentEnd() const;
	ComponentRangeIterator      componentIterator();
	ConstComponentRangeIterator componentIterator() const;

protected:
	Container compRefs;

	void updateComponentReferences(const Component* oldBase, const Component* newBase);

	void updateComponentReferencesAfterCompact(const Component* base, const std::vector<int>& newIndices);
};

} // namespace mgp::components

#include "component_references.cpp"

#endif // MGP_MESH_COMPONENT_REFERENCES_H
