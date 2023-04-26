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
		conditional<(N >= 0), typename std::array<T, ARRAY_SIZE>, typename std::vector<T>>::type;

public:
	Vector();

	static const int CONTAINER_SIZE = N;

	/** Iterator Types declaration **/

	// if using array, will be the array iterator, the vector iterator otherwise
	using Iterator = typename std::conditional<
		(N >= 0),
		typename std::array<T, ARRAY_SIZE>::iterator,
		typename std::vector<T>::iterator>::type;

	using ConstIterator = typename std::conditional<
		(N >= 0),
		typename std::array<T, ARRAY_SIZE>::const_iterator,
		typename std::vector<T>::const_iterator>::type;

	uint size() const;

	T&       at(uint i);
	const T& at(uint i) const;
	T&       atMod(int i);
	const T& atMod(int i) const;

	void set(const T& e, uint i);

	template<Range Rng>
	void set(Rng&& r);

	void fill(const T& e);

	bool contains(const typename MakeConstPointer<T>::type& e) const;

	Iterator find(const typename MakeConstPointer<T>::type& e);
	ConstIterator find(const typename MakeConstPointer<T>::type& e) const;

	int indexOf(const typename MakeConstPointer<T>::type& e) const;

	/** Member functions specific for vector **/

	void resize(uint n) requires (N < 0);
	void pushBack(const T& v) requires (N < 0);
	void insert(uint i, const T& v) requires (N < 0);
	void erase(uint i) requires (N < 0);
	void clear() requires (N < 0);

	/** Iterator Member functions **/

	Iterator              begin();
	Iterator              end();
	ConstIterator         begin() const;
	ConstIterator         end() const;

protected:
	Container container;
};

} // namespace vcl

#include "vector.cpp"

#endif // VCL_SPACE_VECTOR_H
