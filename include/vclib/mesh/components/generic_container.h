/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#ifndef VCL_MESH_COMPONENTS_GENERIC_CONTAINER_H
#define VCL_MESH_COMPONENTS_GENERIC_CONTAINER_H

#include <algorithm>
#include <array>
#include <assert.h>
#include <type_traits>
#include <vector>

#include "../iterators/range_iterator.h"

namespace vcl::comp {

namespace internal {

template<int M, typename T>
using ReturnIfIsVector = typename std::enable_if<(M < 0), T>::type;
template<int M, typename T>
using ReturnIfIsArray = typename std::enable_if<(M >= 0), T>::type;

} // namespace internal

/**
 * @brief The GenericContainer class is a generic container class of values, that could be static
 * or dynamic depending on the templated size N. This class is meant to be inherited by some other
 * Component that needs to store a number of values of the same type. All its members are protected.
 *
 * This class is mainly used by:
 * - Element references (Vertex references, Face references, Adjacencies in general)
 * - Wedge components (Wedge TexCoords, Wedge Colors, ...)
 */
template<typename C, int N>
class GenericContainer
{
private:
	// if we use the vector, the size of the array will be 0
	// actually the array will never be used and will not use memory, it's just for declaration
	static const int ARRAY_SIZE = N >= 0 ? N : 0;

	// the Container type will be array or vector, depending on N value
	using Container = typename std::
		conditional<(N >= 0), typename std::array<C, ARRAY_SIZE>, typename std::vector<C>>::type;

public:
	GenericContainer();

protected:
	static const int CONTAINER_SIZE = N;

	/** Iterator Types declaration **/

	// if using array, will be the array iterator, the vector iterator otherwise
	using GCIterator = typename std::conditional<
		(N >= 0),
		typename std::array<C, ARRAY_SIZE>::iterator,
		typename std::vector<C>::iterator>::type;

	using ConstGCIterator = typename std::conditional<
		(N >= 0),
		typename std::array<C, ARRAY_SIZE>::const_iterator,
		typename std::vector<C>::const_iterator>::type;

	using GCRangeIterator      = RangeIterator<GenericContainer, GCIterator>;
	using ConstGCRangeIterator = ConstRangeIterator<GenericContainer, ConstGCIterator>;

	unsigned int size() const;

	C&       at(unsigned int i);
	const C& at(unsigned int i) const;
	C&       atMod(int i);
	const C& atMod(int i) const;

	void set(const C& e, unsigned int i);
	void set(const std::vector<C>& list);

	bool contains(const C& e) const;

	GCIterator find(const C& e);
	ConstGCIterator find(const C& e) const;

	int indexOf(const C& e) const;

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> resize(unsigned int n);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushBack(const C& v);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> insert(unsigned int i, const C& v);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> erase(unsigned int i);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> clear();

	/** Iterator Member functions **/

	GCIterator           begin();
	GCIterator           end();
	ConstGCIterator      begin() const;
	ConstGCIterator      end() const;
	GCRangeIterator      rangeIterator();
	ConstGCRangeIterator rangeIterator() const;

	Container container;
};

} // namespace vcl::comp

#include "generic_container.cpp"

#endif // VCL_MESH_COMPONENTS_GENERIC_CONTAINER_H
