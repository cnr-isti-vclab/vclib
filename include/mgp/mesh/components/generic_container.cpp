/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "generic_container.h"

namespace mgp::comp {

/**
 * @brief Empty constructor of the container. If the container is dynamic, is size is 0. When the
 * container is static, the size is N and its elements are initialized with their empty constructor.
 */
template<typename C, int N>
GenericContainer<C, N>::GenericContainer()
{
}

/**
 * @brief Returns the size of the container, which is N if the container is static.
 * @return the size of the container.
 */
template<typename C, int N>
unsigned int GenericContainer<C, N>::size() const
{
	if constexpr (N >= 0) {
		return N;
	}
	else {
		return container.size();
	}
}

/**
 * @brief Access the specified element with bounds checking.
 * @param i: the position of the element.
 * @return a reference of the element at position i.
 */
template<typename C, int N>
C& GenericContainer<C, N>::at(unsigned int i)
{
	assert(i < size());
	return container[i];
}

template<typename C, int N>
const C& GenericContainer<C, N>::at(unsigned int i) const
{
	assert(i < size());
	return container[i];
}

/**
 * @brief Access the specified element, computing first the module of the position w.r.t. the size
 * of the container.
 *
 * Takes into account negative indices: atMod(-1) will access to the last element of the container.
 *
 * @param i: the position of the element.
 * @return a reference of the element at position i % size().
 */
template<typename C, int N>
C& GenericContainer<C, N>::atMod(int i)
{
	unsigned int n = size();
	return container[(i % n + n) % n];
}

template<typename C, int N>
const C& GenericContainer<C, N>::atMod(int i) const
{
	unsigned int n = size();
	return container[(i % n + n) % n];
}

template<typename C, int N>
void GenericContainer<C, N>::set(const C& e, unsigned int i)
{
	assert(i < size());
	container[i] = e;
}

template<typename C, int N>
void GenericContainer<C, N>::set(const std::vector<C>& list)
{
	if constexpr (N >= 0) {
		assert(list.size() == N);
		unsigned int i = 0;
		for (const auto& v : list) {
			set(v, i);
			++i;
		}
	}
	else {
		container = list;
	}
}

template<typename C, int N>
bool GenericContainer<C, N>::contains(const C& e) const
{
	return std::find(container.begin(), container.end(), e) != container.end();
}

template<typename C, int N>
template<int U>
internal::ReturnIfIsVector<U, void> GenericContainer<C, N>::resize(unsigned int n)
{
	container.resize(n);
}

template<typename C, int N>
template<int U>
internal::ReturnIfIsVector<U, void> GenericContainer<C, N>::pushBack(const C& v)
{
	container.push_back(v);
}

template<typename C, int N>
template<int U>
internal::ReturnIfIsVector<U, void> GenericContainer<C, N>::insert(unsigned int i, const C& v)
{
	assert(i < size());
	container.insert(container.begin() + i, v);
}

template<typename C, int N>
template<int U>
internal::ReturnIfIsVector<U, void> GenericContainer<C, N>::erase(unsigned int i)
{
	assert(i < size());
	container.erase(container.begin() + i);
}

template<typename C, int N>
template<int U>
internal::ReturnIfIsVector<U, void> GenericContainer<C, N>::clear()
{
	container.clear();
}

template<typename C, int N>
typename GenericContainer<C, N>::GCIterator GenericContainer<C, N>::begin()
{
	return container.begin();
}

template<typename C, int N>
typename GenericContainer<C, N>::GCIterator GenericContainer<C, N>::end()
{
	return container.end();
}

template<typename C, int N>
typename GenericContainer<C, N>::ConstGCIterator GenericContainer<C, N>::begin() const
{
	return container.begin();
}

template<typename C, int N>
typename GenericContainer<C, N>::ConstGCIterator GenericContainer<C, N>::end() const
{
	return container.end();
}

template<typename C, int N>
typename GenericContainer<C, N>::GCRangeIterator GenericContainer<C, N>::rangeIterator()
{
	return GCRangeIterator(*this, &GenericContainer::begin, &GenericContainer::end);
}

template<typename C, int N>
typename GenericContainer<C, N>::ConstGCRangeIterator GenericContainer<C, N>::rangeIterator() const
{
	return ConstGCRangeIterator(*this, &GenericContainer::begin, &GenericContainer::end);
}

} // namespace mgp::comp
