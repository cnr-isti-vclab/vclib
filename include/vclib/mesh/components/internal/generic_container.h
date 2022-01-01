/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
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
#include <vector>

#include <vclib/iterators/range_iterator.h>
#include <vclib/misc/types.h>

namespace vcl::comp::internal {

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

	uint size() const;

	C&       at(uint i);
	const C& at(uint i) const;
	C&       atMod(int i);
	const C& atMod(int i) const;

	void set(const C& e, uint i);
	void set(const std::vector<C>& list);

	bool contains(const typename MakeConstPointer<C>::type& e) const;

	GCIterator find(const typename MakeConstPointer<C>::type& e);
	ConstGCIterator find(const typename MakeConstPointer<C>::type& e) const;

	int indexOf(const typename MakeConstPointer<C>::type& e) const;

	/** Member functions specific for vector **/

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) resize(uint n);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) pushBack(const C& v);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) insert(uint i, const C& v);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) erase(uint i);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) clear();

	/** Iterator Member functions **/

	GCIterator           begin();
	GCIterator           end();
	ConstGCIterator      begin() const;
	ConstGCIterator      end() const;
	GCRangeIterator      rangeIterator();
	ConstGCRangeIterator rangeIterator() const;

	Container container;
};

} // namespace vcl::comp::internal

#include "generic_container.cpp"

#endif // VCL_MESH_COMPONENTS_GENERIC_CONTAINER_H
