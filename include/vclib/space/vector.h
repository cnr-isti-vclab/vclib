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

#ifndef VCL_SPACE_VECTOR_H
#define VCL_SPACE_VECTOR_H

#include <algorithm>
#include <array>
#include <assert.h>
#include <vector>

#include <vclib/concepts/ranges/range.h>
#include <vclib/types.h>

namespace vcl {

/**
 * @brief The Vector class is a generic container of objects of type T, that could have fixed or
 * dynamic size, depending on the templated size N.
 *
 * The Vector class is a container that can hold objects of type T. The size of the container can be
 * either fixed or dynamic, depending on the value of the template parameter N. If N is greater than
 * or equal to zero, the container will have a fixed size of N elements, and it will use an array to
 * store the elements. If N is less than zero, the container will have a dynamic size, and it will
 * use a vector to store the elements. The Vector class provides several member functions to
 * manipulate the elements of the container, such as getting and setting individual elements,
 * filling the container with a value, checking if the container contains a certain element, and
 * finding the index of a certain element.
 *
 * @tparam T: the type of the objects stored in the container.
 * @tparam N: the size of the container. If N is greater than or equal to zero, the container will
 * have a fixed size of N elements, and it will use an array to store the elements. If N is less
 * than zero, the container will have a dynamic size, and it will use a vector to store the
 * elements.
 *
 * @ingroup space
 */
template<typename T, int N>
class Vector
{
private:
	// if we use the vector, the size of the array will be 0
	// actually the array will never be used and will not use memory, it's just for declaration
	static const int ARRAY_SIZE = N >= 0 ? N : 0;

	// the Container type will be array or vector, depending on N value
	using Container = typename std::
		conditional_t<(N >= 0), typename std::array<T, ARRAY_SIZE>, typename std::vector<T>>;

public:
	/** The type of the elements stored in the Vector. */
	using ValueType = typename Container::value_type;

	/** A const reference to the type of the elements stored in the Vector. */
	using ConstReference = typename Container::const_reference;

	/** A reference to the type of the elements stored in the Vector. */
	using Reference = typename Container::reference;

	/** A const pointer to the type of the elements stored in the Vector. */
	using ConstPointer = typename Container::const_pointer;

	/** A pointer to the type of the elements stored in the Vector. */
	using Pointer = typename Container::pointer;

	/** An iterator to the elements of the Vector. */
	using Iterator = typename Container::iterator;

	/** A const iterator to the elements of the Vector. */
	using ConstIterator = typename Container::const_iterator;

	/** Size of the vector at compile time. It will be -1 if the Vector has dynamic size. */
	static const int SIZE = N;

	Vector();

	uint size() const;

	Reference      at(uint i);
	ConstReference at(uint i) const;
	Reference      atMod(int i);
	ConstReference atMod(int i) const;

	void set(const T& e, uint i);

	template<Range Rng>
	void set(Rng&& r);

	void fill(const T& e);

	bool contains(const typename MakeConstPointer<T>::type& e) const;

	Iterator      find(const typename MakeConstPointer<T>::type& e);
	ConstIterator find(const typename MakeConstPointer<T>::type& e) const;

	int indexOf(const typename MakeConstPointer<T>::type& e) const;

	/** Member functions specific for vector **/

	void resize(uint n)
		requires(N < 0);
	void pushBack(const T& v)
		requires(N < 0);
	void insert(uint i, const T& v)
		requires(N < 0);
	void erase(uint i)
		requires(N < 0);
	void clear()
		requires(N < 0);

	/** Operators **/
	Reference operator[](uint i);
	ConstReference operator[](uint i) const;

	/** Iterator Member functions **/

	Iterator      begin();
	Iterator      end();
	ConstIterator begin() const;
	ConstIterator end() const;

protected:
	Container container;
};

} // namespace vcl

#include "vector.cpp"

#endif // VCL_SPACE_VECTOR_H
