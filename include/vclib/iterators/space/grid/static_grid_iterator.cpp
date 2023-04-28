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

#include "static_grid_iterator.h"

namespace vcl {

template<typename KeyType, typename ValueType, typename GridType>
StaticGridIterator<KeyType, ValueType, GridType>::StaticGridIterator()
{
}

template<typename KeyType, typename ValueType, typename GridType>
StaticGridIterator<KeyType, ValueType, GridType>::StaticGridIterator(
	VecIt           it,
	const GridType& g) :
		vecIt(it), g(&g)
{

}

template<typename KeyType, typename ValueType, typename GridType>
typename StaticGridIterator<KeyType, ValueType, GridType>::value_type
StaticGridIterator<KeyType, ValueType, GridType>::operator*() const
{
	KeyType cell = g->cellOfIndex(vecIt->first);
	return value_type(cell, vecIt->second);
}

template<typename KeyType, typename ValueType, typename GridType>
typename StaticGridIterator<KeyType, ValueType, GridType>::ArrowHelper
StaticGridIterator<KeyType, ValueType, GridType>::operator->() const
{
	return **this;
}

template<typename KeyType, typename ValueType, typename GridType>
bool StaticGridIterator<KeyType, ValueType, GridType>::operator==(
	const StaticGridIterator& oi) const
{
	return vecIt == oi.vecIt;
}

template<typename KeyType, typename ValueType, typename GridType>
bool StaticGridIterator<KeyType, ValueType, GridType>::operator!=(
	const StaticGridIterator& oi) const
{
	return vecIt != oi.vecIt;
}

template<typename KeyType, typename ValueType, typename GridType>
StaticGridIterator<KeyType, ValueType, GridType>
StaticGridIterator<KeyType, ValueType, GridType>::operator++()
{
	++vecIt;
	return *this;
}

template<typename KeyType, typename ValueType, typename GridType>
StaticGridIterator<KeyType, ValueType, GridType>
StaticGridIterator<KeyType, ValueType, GridType>::operator++(int)
{
	StaticGridIterator old = *this;
	++vecIt;
	return old;
}

template<typename KeyType, typename ValueType, typename GridType>
ConstStaticGridIterator<KeyType, ValueType, GridType>::ConstStaticGridIterator()
{
}

template<typename KeyType, typename ValueType, typename GridType>
ConstStaticGridIterator<KeyType, ValueType, GridType>::ConstStaticGridIterator(
	VecIt           it,
	const GridType& g) :
		vecIt(it), g(&g)
{

}

template<typename KeyType, typename ValueType, typename GridType>
typename ConstStaticGridIterator<KeyType, ValueType, GridType>::value_type
ConstStaticGridIterator<KeyType, ValueType, GridType>::operator*() const
{
	KeyType cell = g->cellOfIndex(vecIt->first);
	return value_type(cell, vecIt->second);
}

template<typename KeyType, typename ValueType, typename GridType>
typename ConstStaticGridIterator<KeyType, ValueType, GridType>::ArrowHelper
ConstStaticGridIterator<KeyType, ValueType, GridType>::operator->() const
{
	return **this;
}

template<typename KeyType, typename ValueType, typename GridType>
bool ConstStaticGridIterator<KeyType, ValueType, GridType>::operator==(
	const ConstStaticGridIterator& oi) const
{
	return vecIt == oi.vecIt;
}

template<typename KeyType, typename ValueType, typename GridType>
bool ConstStaticGridIterator<KeyType, ValueType, GridType>::operator!=(
	const ConstStaticGridIterator& oi) const
{
	return vecIt != oi.vecIt;
}

template<typename KeyType, typename ValueType, typename GridType>
ConstStaticGridIterator<KeyType, ValueType, GridType>
ConstStaticGridIterator<KeyType, ValueType, GridType>::operator++()
{
	++vecIt;
	return *this;
}

template<typename KeyType, typename ValueType, typename GridType>
ConstStaticGridIterator<KeyType, ValueType, GridType>
ConstStaticGridIterator<KeyType, ValueType, GridType>::operator++(int)
{
	ConstStaticGridIterator old = *this;
	++vecIt;
	return old;
}

} // namespace vcl
