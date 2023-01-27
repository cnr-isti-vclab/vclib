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

#include "hash_table_grid_iterator.h"

namespace vcl {

template<typename KeyType, typename ValueType>
HashTableGridIterator<KeyType, ValueType>::HashTableGridIterator()
{
}

template<typename KeyType, typename ValueType>
HashTableGridIterator<KeyType, ValueType>::HashTableGridIterator(MapIt it) :
		mapIt(it)
{
}

template<typename KeyType, typename ValueType>
const Markable<ValueType>& HashTableGridIterator<KeyType, ValueType>::markableValue() const
{
	return *mapIt->second;
}

template<typename KeyType, typename ValueType>
Markable<ValueType>& HashTableGridIterator<KeyType, ValueType>::markableValue()
{
	return *mapIt->second;
}

template<typename KeyType, typename ValueType>
typename HashTableGridIterator<KeyType, ValueType>::value_type
HashTableGridIterator<KeyType, ValueType>::operator *() const
{
	return value_type(mapIt->first, mapIt->second->get());
}

template<typename KeyType, typename ValueType>
typename HashTableGridIterator<KeyType, ValueType>::ArrowHelper
HashTableGridIterator<KeyType, ValueType>::operator->() const
{
	return **this;
}

template<typename KeyType, typename ValueType>
bool HashTableGridIterator<KeyType, ValueType>::operator==(const HashTableGridIterator& oi) const
{
	return mapIt == oi.mapIt;
}

template<typename KeyType, typename ValueType>
bool HashTableGridIterator<KeyType, ValueType>::operator!=(const HashTableGridIterator& oi) const
{
	return mapIt != oi.mapIt;
}

template<typename KeyType, typename ValueType>
HashTableGridIterator<KeyType, ValueType> HashTableGridIterator<KeyType, ValueType>::operator++()
{
	++mapIt;
	return *this;
}

template<typename KeyType, typename ValueType>
HashTableGridIterator<KeyType, ValueType> HashTableGridIterator<KeyType, ValueType>::operator++(int)
{
	HashTableGridIterator old = *this;
	++mapIt;
	return old;
}

template<typename KeyType, typename ValueType>
ConstHashTableGridIterator<KeyType, ValueType>::ConstHashTableGridIterator()
{
}

template<typename KeyType, typename ValueType>
ConstHashTableGridIterator<KeyType, ValueType>::ConstHashTableGridIterator(MapIt it) :
		mapIt(it)
{
}

template<typename KeyType, typename ValueType>
const Markable<ValueType>& ConstHashTableGridIterator<KeyType, ValueType>::markableValue() const
{
	return *mapIt->second;
}

template<typename KeyType, typename ValueType>
typename ConstHashTableGridIterator<KeyType, ValueType>::value_type
ConstHashTableGridIterator<KeyType, ValueType>::operator *() const
{
	return value_type(mapIt->first, mapIt->second->get());
}

template<typename KeyType, typename ValueType>
typename ConstHashTableGridIterator<KeyType, ValueType>::ArrowHelper
ConstHashTableGridIterator<KeyType, ValueType>::operator->() const
{
	return **this;
}

template<typename KeyType, typename ValueType>
bool ConstHashTableGridIterator<KeyType, ValueType>::operator==(const ConstHashTableGridIterator& oi) const
{
	return mapIt == oi.mapIt;
}

template<typename KeyType, typename ValueType>
bool ConstHashTableGridIterator<KeyType, ValueType>::operator!=(const ConstHashTableGridIterator& oi) const
{
	return mapIt != oi.mapIt;
}

template<typename KeyType, typename ValueType>
ConstHashTableGridIterator<KeyType, ValueType> ConstHashTableGridIterator<KeyType, ValueType>::operator++()
{
	++mapIt;
	return *this;
}

template<typename KeyType, typename ValueType>
ConstHashTableGridIterator<KeyType, ValueType> ConstHashTableGridIterator<KeyType, ValueType>::operator++(int)
{
	ConstHashTableGridIterator old = *this;
	++mapIt;
	return old;
}

} // namespace vcl
