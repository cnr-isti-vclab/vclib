/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "component_references.h"

namespace mgp::components {

template<class Component, int N>
ComponentReferences<Component, N>::ComponentReferences()
{
	if constexpr (N >= 0) {
		// I'll use the array, N is >= 0.
		// There will be a static number of references.
		compRefs = std::array<Component*, N> {nullptr};
	}
	else {
		// I'll use the vector, because N is < 0.
		// There will be a dynamic number of references.
		compRefs = std::vector<Component*>();
	}
}

template<class Component, int N>
unsigned int ComponentReferences<Component, N>::componentNumber() const
{
	if constexpr (N >= 0) {
		return N;
	}
	else {
		return compRefs.size();
	}
}

template<class Component, int N>
unsigned int ComponentReferences<Component, N>::sizeMod(unsigned int i) const
{
	return i % componentNumber();
}

template<class Component, int N>
Component*& ComponentReferences<Component, N>::c(unsigned int i)
{
	assert(i < componentNumber());
	return compRefs[i];
}

template<class Component, int N>
const Component* ComponentReferences<Component, N>::c(unsigned int i) const
{
	assert(i < componentNumber());
	return compRefs[i];
}

template<class Component, int N>
void ComponentReferences<Component, N>::setComponent(Component* v, unsigned int i)
{
	assert(i < componentNumber());
	compRefs[i] = v;
}

template<class Component, int N>
void ComponentReferences<Component, N>::setComponents(const std::vector<Component*>& list)
{
	if constexpr (N >= 0) {
		assert(list.size() == N);
		unsigned int i = 0;
		for (const auto& v : list) {
			setComponent(v, i);
			++i;
		}
	}
	else {
		compRefs = list;
	}
}

template<class Component, int N>
template<int U>
internal::ReturnIfIsVector<U, void> ComponentReferences<Component, N>::pushComponent(Component* v)
{
	compRefs.push_back(v);
}

template<class Component, int N>
template<int U>
internal::ReturnIfIsVector<U, void>
ComponentReferences<Component, N>::insertComponent(unsigned int i, Component* v)
{
	assert(i < componentNumber());
	compRefs.insert(compRefs.begin() + i, v);
}

template<class Component, int N>
template<int U>
internal::ReturnIfIsVector<U, void> ComponentReferences<Component, N>::eraseComponent(unsigned int i)
{
	assert(i < componentNumber());
	compRefs.erase(compRefs.begin() + i);
}

template<class Component, int N>
template<int U>
internal::ReturnIfIsVector<U, void> ComponentReferences<Component, N>::clearComponents()
{
	compRefs.clear();
}

template<class Component, int N>
typename ComponentReferences<Component, N>::ComponentIterator ComponentReferences<Component, N>::componentBegin()
{
	return compRefs.begin();
}

template<class Component, int N>
typename ComponentReferences<Component, N>::ComponentIterator ComponentReferences<Component, N>::componentEnd()
{
	return compRefs.end();
}

template<class Component, int N>
typename ComponentReferences<Component, N>::ConstComponentIterator
ComponentReferences<Component, N>::componentBegin() const
{
	return compRefs.begin();
}

template<class Component, int N>
typename ComponentReferences<Component, N>::ConstComponentIterator
ComponentReferences<Component, N>::componentEnd() const
{
	return compRefs.end();
}

template<class Component, int N>
typename ComponentReferences<Component, N>::ComponentRangeIterator
ComponentReferences<Component, N>::componentIterator()
{
	return ComponentRangeIterator(*this, &ComponentReferences::componentBegin, &ComponentReferences::componentEnd);
}

template<class Component, int N>
typename ComponentReferences<Component, N>::ConstComponentRangeIterator
ComponentReferences<Component, N>::componentIterator() const
{
	return ConstComponentRangeIterator(
		*this, &ComponentReferences::componentBegin, &ComponentReferences::componentEnd);
}

template<class Component, int N>
void ComponentReferences<Component, N>::updateComponentReferences(
	const Component* oldBase,
	const Component* newBase)
{
	for (unsigned int j = 0; j < componentNumber(); ++j) {
		if (c(j) != nullptr) {
			size_t diff = c(j) - oldBase;
			c(j)        = (Component*) newBase + diff;
		}
	}
}

template<class Component, int N>
void ComponentReferences<Component, N>::updateComponentReferencesAfterCompact(
	const Component* base,
	const std::vector<int>& newIndices)
{
	for (unsigned int j = 0; j < componentNumber(); ++j) {
		if (c(j) != nullptr) {
			size_t diff = c(j) - base;
			if (newIndices[diff] < 0){ // element has been removed
				c(j) = nullptr;
			}
			else { // the new pointer will be base + newIndices[diff]
				c(j) = base + newIndices[diff];
			}
		}
	}
}

}
