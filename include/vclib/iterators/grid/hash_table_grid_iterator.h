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

#ifndef VCL_ITERATORS_GRID_HASH_TABLE_GRID_ITERATOR_H
#define VCL_ITERATORS_GRID_HASH_TABLE_GRID_ITERATOR_H

#include <unordered_map>

#include <vclib/misc/mark.h>
#include <vclib/misc/pair.h>
#include <vclib/space/point.h>

namespace vcl {

template<typename GridType, typename ValueType>
class HashTableGrid;

template<typename KeyType, typename ValueType>
class HashTableGridIterator
{
	template<typename GridType, typename VT>
	friend class HashTableGrid;

private:
	using MapIt = typename std::unordered_multimap<KeyType, Markable<ValueType>>::const_iterator;

public:
	using T = KeyValueRefPair<const KeyType, const ValueType>;
	using value_type = T;

	class ArrowHelper
	{
		T value;

	public:
		ArrowHelper(T value) : value(value) {}
		const T* operator->() const { return &value; }
	};

	HashTableGridIterator();
	HashTableGridIterator(MapIt it);

	value_type  operator*() const;
	ArrowHelper operator->() const;

	bool operator==(const HashTableGridIterator& oi) const;
	bool operator!=(const HashTableGridIterator& oi) const;

	HashTableGridIterator operator++();
	HashTableGridIterator operator++(int);

private:
	MapIt mapIt;
};

} // namespace vcl

#include "hash_table_grid_iterator.cpp"

#endif // VCL_ITERATORS_GRID_HASH_TABLE_GRID_ITERATOR_H
