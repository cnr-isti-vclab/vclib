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

#ifndef VCL_ITERATORS_GRID_STATIC_GRID_ITERATOR_H
#define VCL_ITERATORS_GRID_STATIC_GRID_ITERATOR_H

#include <vclib/misc/mark.h>
#include <vclib/misc/pair.h>
#include <vclib/space/point.h>

namespace vcl {

template<typename KeyType, typename ValueType, typename GridType>
class StaticGridIterator
{
	using VecIt = typename std::vector<std::pair<uint, vcl::Markable<ValueType>>>::iterator;

public:
	using T = KeyValueRefPair<KeyType, ValueType>;
	using value_type = T;

	class ArrowHelper
	{
		T value;

	public:
		ArrowHelper(T value) : value(value) {}
		const T* operator->() const { return &value; }
	};

	StaticGridIterator();
	StaticGridIterator(VecIt it, const GridType& g);

	value_type  operator*() const;
	ArrowHelper operator->() const;

	bool operator==(const StaticGridIterator& oi) const;
	bool operator!=(const StaticGridIterator& oi) const;

	StaticGridIterator operator++();
	StaticGridIterator operator++(int);

private:
	VecIt vecIt;
	const GridType* g = nullptr;
};

template<typename KeyType, typename ValueType, typename GridType>
class ConstStaticGridIterator
{
	using VecIt = typename std::vector<std::pair<uint, vcl::Markable<ValueType>>>::const_iterator;

public:
	using T = KeyValueRefPair<KeyType, const ValueType>;
	using value_type = T;

	class ArrowHelper
	{
		T value;

	public:
		ArrowHelper(T value) : value(value) {}
		const T* operator->() const { return &value; }
	};

	ConstStaticGridIterator();
	ConstStaticGridIterator(VecIt it, const GridType& g);

	value_type  operator*() const;
	ArrowHelper operator->() const;

	bool operator==(const ConstStaticGridIterator& oi) const;
	bool operator!=(const ConstStaticGridIterator& oi) const;

	ConstStaticGridIterator operator++();
	ConstStaticGridIterator operator++(int);

private:
	VecIt vecIt;
	const GridType* g = nullptr;
};

} // namespace vcl

#include "static_grid_iterator.cpp"

#endif // VCL_ITERATORS_GRID_STATIC_GRID_ITERATOR_H
