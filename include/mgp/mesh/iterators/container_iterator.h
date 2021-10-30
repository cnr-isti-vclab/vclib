/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MESH_ITERATORS_CONTAINER_ITERATOR_H
#define MESH_ITERATORS_CONTAINER_ITERATOR_H

#include <vector>

namespace mgp {

template<typename T>
class ContainerIterator
{
private:
	typedef typename std::vector<T>::iterator VecIt;

public:
	typedef ptrdiff_t                 difference_type;
	typedef T                         value_type;
	typedef T&                        reference;
	typedef T*                        pointer;
	typedef std::forward_iterator_tag iterator_category;

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

template<typename T>
class ConstContainerIterator
{
private:
	typedef typename std::vector<T>::const_iterator VecIt;

public:
	typedef ptrdiff_t                 difference_type;
	typedef T                         value_type;
	typedef const T&                  reference;
	typedef const T*                  pointer;
	typedef std::forward_iterator_tag iterator_category;

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
