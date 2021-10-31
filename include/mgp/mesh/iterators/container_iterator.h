/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MESH_ITERATORS_CONTAINER_ITERATOR_H
#define MESH_ITERATORS_CONTAINER_ITERATOR_H

#include <vector>

namespace mgp {

template<typename T>
/**
 * @brief The ContainerIterator class is an iterator designed to iterate along a Container of
 * Elements (e.g. the containers of Vertices, Faces, ...) that will compose a Mesh.
 *
 * Since these Containers can contain deleted Elements, this iterator is designed to jump
 * automatically deleted element, if the argument jumpDeleted of the constructor is true.
 */
class ContainerIterator
{
private:
	using VecIt = typename std::vector<T>::iterator;

public:
	using difference_type = ptrdiff_t;
	using value_type = T;
	using reference = T&;
	using pointer = T*;
	using iterator_category = std::forward_iterator_tag;

	ContainerIterator(VecIt it, const std::vector<T>& vec, bool jumpDeleted = true) :
			it(it), vec(vec), jumpDeleted(jumpDeleted)
	{
	}

	reference operator*() const { return *it; }

	pointer operator->() const { return &*it; }

	bool operator==(const ContainerIterator<T>& oi) const { return it == oi.it; }

	bool operator!=(const ContainerIterator<T>& oi) const { return it != oi.it; }

	ContainerIterator operator++()
	{
		do {
			++it;
		} while (it != vec.end() && jumpDeleted && it->isDeleted());
		return *this;
	}

	ContainerIterator operator++(int)
	{
		ContainerIterator old = *this;
		do {
			++it;
		} while (it != vec.end() && jumpDeleted && it->isDeleted());
		return old;
	}

public:
	VecIt                 it;          // the actual iterator
	const std::vector<T>& vec;         // needed to check for end when jumping elements
	bool                  jumpDeleted; // when true, deleted Elements will be jumped
};

/**
 * @brief The ConstContainerIterator class is the const alternative of the ContainerIterator, that
 * allows to iterate over a const Container and get only const Elements.
 */
template<typename T>
class ConstContainerIterator
{
private:
	using VecIt = typename std::vector<T>::const_iterator;

public:
	using difference_type = ptrdiff_t;
	using value_type = T;
	using reference = const T&;
	using pointer = const T*;
	using iterator_category = std::forward_iterator_tag;

	ConstContainerIterator(VecIt it, const std::vector<T>& vec, bool jumpDeleted = true) :
			it(it), vec(vec), jumpDeleted(jumpDeleted)
	{
	}
	ConstContainerIterator(const ContainerIterator<T>& it) :
			it(it.it), vec(it.vec), jumpDeleted(it.jumpDeleted)
	{
	}

	reference operator*() const { return *it; }

	pointer operator->() const { return &*it; }

	bool operator==(const ConstContainerIterator<T>& oi) const { return it == oi.it; }

	bool operator!=(const ConstContainerIterator<T>& oi) const { return it != oi.it; }

	ConstContainerIterator operator++()
	{
		do {
			++it;
		} while (it != vec.end() && jumpDeleted && it->isDeleted());
		return *this;
	}

	ConstContainerIterator operator++(int)
	{
		ConstContainerIterator old = *this;
		do {
			++it;
		} while (it != vec.end() && jumpDeleted && it->isDeleted());
		return old;
	}

public:
	VecIt                 it;          // the actual iterator
	const std::vector<T>& vec;         // needed to check for end when jumping elements
	bool                  jumpDeleted; // when true, deleted Elements will be jumped
};

} // namespace mgp

#endif // MESH_ITERATORS_CONTAINER_ITERATOR_H
