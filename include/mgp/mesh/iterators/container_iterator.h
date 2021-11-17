/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_ITERATORS_CONTAINER_ITERATOR_H
#define MGP_MESH_ITERATORS_CONTAINER_ITERATOR_H

#include <vector>

namespace mgp {

/**
 * @brief The ContainerIterator class is an iterator designed to iterate along a Container of
 * Elements (e.g. the containers of Vertices, Faces, ...) that will compose a Mesh.
 *
 * Since these Containers can contain deleted Elements, this iterator is designed to jump
 * automatically deleted element, if the argument jumpDeleted of the constructor is true.
 * If the jumpDeleted argument of the constructor is false, the increment and postIncrement
 * operators will be as fast as a normal increment, without any overhead for checking if the
 * iterator needs to jump elements.
 */
template<typename T>
class ContainerIterator
{
private:
	using VecIt = typename std::vector<T>::iterator;

public:
	using difference_type   = ptrdiff_t;
	using value_type        = T;
	using reference         = T&;
	using pointer           = T*;
	using iterator_category = std::forward_iterator_tag;

	ContainerIterator(VecIt it, const std::vector<T>& vec, bool jumpDeleted = true) :
			it(it), vec(vec)
	{
		if (jumpDeleted) {
			increment     = &ContainerIterator::incrementJump;
			postIncrement = &ContainerIterator::postIncrementJump;
		}
		else {
			increment     = &ContainerIterator::incrementFast;
			postIncrement = &ContainerIterator::incrementFast;
		}
	}

	reference operator*() const { return *it; }

	pointer operator->() const { return &*it; }

	bool operator==(const ContainerIterator<T>& oi) const { return it == oi.it; }

	bool operator!=(const ContainerIterator<T>& oi) const { return it != oi.it; }

	ContainerIterator operator++() { return (this->*increment)(); }

	ContainerIterator operator++(int) { return (this->*postIncrement)(); }

private:
	/**
	 * @brief Increment function that will be called if we need to jump deleted elements.
	 */
	ContainerIterator incrementJump()
	{
		do {
			++it;
		} while (it != vec.end() && it->isDeleted());
		return *this;
	}

	/**
	 * @brief Post increment function that will be called if we need to jump deleted elements.
	 */
	ContainerIterator postIncrementJump()
	{
		ContainerIterator old = *this;
		do {
			++it;
		} while (it != vec.end() && it->isDeleted());
		return old;
	}

	/**
	 * @brief Increment function that will be called if we don't need to jump deleted elements.
	 */
	ContainerIterator incrementFast()
	{
		++it;
		return *this;
	}

	/**
	 * @brief Post increment function that will be called if we don't need to jump deleted elements.
	 */
	ContainerIterator postIncrementFast()
	{
		ContainerIterator old = *this;
		++it;
		return old;
	}

	// pointer to increment function, assigned in the constructor
	ContainerIterator (ContainerIterator::*increment)();
	// pointer to post increment function, assigned in the constructor
	ContainerIterator (ContainerIterator::*postIncrement)();

	VecIt                 it;  // the actual iterator
	const std::vector<T>& vec; // needed to check for end when jumping elements
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
	using difference_type   = ptrdiff_t;
	using value_type        = T;
	using reference         = const T&;
	using pointer           = const T*;
	using iterator_category = std::forward_iterator_tag;

	ConstContainerIterator(VecIt it, const std::vector<T>& vec, bool jumpDeleted = true) :
			it(it), vec(vec)
	{
		if (jumpDeleted) {
			increment     = &ConstContainerIterator::incrementJump;
			postIncrement = &ConstContainerIterator::postIncrementJump;
		}
		else {
			increment     = &ConstContainerIterator::incrementFast;
			postIncrement = &ConstContainerIterator::incrementFast;
		}
	}
	ConstContainerIterator(const ContainerIterator<T>& it) :
			ConstContainerIterator(it.it, it.vec, it.jumpDeleted)
	{
	}

	reference operator*() const { return *it; }

	pointer operator->() const { return &*it; }

	bool operator==(const ConstContainerIterator<T>& oi) const { return it == oi.it; }

	bool operator!=(const ConstContainerIterator<T>& oi) const { return it != oi.it; }

	ConstContainerIterator operator++() { return (this->*increment)(); }

	ConstContainerIterator operator++(int) { return (this->*postIncrement)(); }

private:
	/**
	 * @brief Increment function that will be called if we need to jump deleted elements.
	 */
	ConstContainerIterator incrementJump()
	{
		do {
			++it;
		} while (it != vec.end() && it->isDeleted());
		return *this;
	}

	/**
	 * @brief Post increment function that will be called if we need to jump deleted elements.
	 */
	ConstContainerIterator postIncrementJump()
	{
		ConstContainerIterator old = *this;
		do {
			++it;
		} while (it != vec.end() && it->isDeleted());
		return old;
	}

	/**
	 * @brief Increment function that will be called if we don't need to jump deleted elements.
	 */
	ConstContainerIterator incrementFast()
	{
		++it;
		return *this;
	}

	/**
	 * @brief Post increment function that will be called if we don't need to jump deleted elements.
	 */
	ConstContainerIterator postIncrementFast()
	{
		ContainerIterator old = *this;
		++it;
		return old;
	}

	// pointer to increment function, assigned in the constructor
	ConstContainerIterator (ConstContainerIterator::*increment)();
	// pointer to post increment function, assigned in the constructor
	ConstContainerIterator (ConstContainerIterator::*postIncrement)();

	VecIt                 it;  // the actual iterator
	const std::vector<T>& vec; // needed to check for end when jumping elements
};

} // namespace mgp

#endif // MGP_MESH_ITERATORS_CONTAINER_ITERATOR_H
