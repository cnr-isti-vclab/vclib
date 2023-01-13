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

#include <vclib/misc/markable_vector.h>
#include <vclib/space/point.h>

namespace vcl {

template<typename KeyType, typename ValueType>
class HashTableGridIterator
{
private:
	using MapIt = typename std::unordered_multimap<KeyType, ValueType>::const_iterator;

public:
	using T =
		std::pair<std::reference_wrapper<const KeyType>, std::reference_wrapper<const ValueType>>;
	using value_type = T;
	using reference         = T&;
	using pointer           = T*;

	HashTableGridIterator();
	HashTableGridIterator(MapIt it, const vcl::MarkableVector<ValueType>& vec);

	reference operator*() const;
	pointer   operator->() const;

	bool operator==(const HashTableGridIterator& oi) const;
	bool operator!=(const HashTableGridIterator& oi) const;

	HashTableGridIterator operator++();
	HashTableGridIterator operator++(int);

private:
	MapIt it;
	const vcl::MarkableVector<ValueType>* vec = nullptr;
};

} // namespace vcl

#include "hash_table_grid_iterator.cpp"

#endif // VCL_ITERATORS_GRID_HASH_TABLE_GRID_ITERATOR_H
