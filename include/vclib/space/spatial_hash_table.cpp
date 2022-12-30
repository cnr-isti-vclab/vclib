/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#include "spatial_hash_table.h"

#include <vclib/algorithms/stat/bounding_box.h>

namespace vcl {

template<typename GridType, typename ValueType>
SpatialHashTable<GridType, ValueType>::SpatialHashTable()
{

}

template<typename GridType, typename ValueType>
bool SpatialHashTable<GridType, ValueType>::empty() const
{
	return map.empty();
}

template<typename GridType, typename ValueType>
template<typename BoxType>
SpatialHashTable<GridType, ValueType>::SpatialHashTable(const BoxType& bbox, const KeyType& size) :
		GridType(bbox, size)
{
}

template<typename GridType, typename ValueType>
std::size_t SpatialHashTable<GridType, ValueType>::size() const
{
	return map.size();
}

template<typename GridType, typename ValueType>
bool SpatialHashTable<GridType, ValueType>::cellEmpty(const KeyType& k) const
{
	return map.find(k) == map.end();
}

template<typename GridType, typename ValueType>
std::size_t SpatialHashTable<GridType, ValueType>::cellSize(const KeyType& k) const
{
	return map.count(k);
}

template<typename GridType, typename ValueType>
std::set<typename SpatialHashTable<GridType, ValueType>::KeyType>
SpatialHashTable<GridType, ValueType>::cells() const
{
	std::set<KeyType> keys;
	for (const auto& p : map)
		keys.insert(p.first);
	return keys;
}

template<typename GridType, typename ValueType>
void SpatialHashTable<GridType, ValueType>::clear()
{
	map.clear();
}

template<typename GridType, typename ValueType>
void SpatialHashTable<GridType, ValueType>::insert(const KeyType &k, const ValueType &v)
{
	map.insert(MapValueType(k, v));
}

template<typename GridType, typename ValueType>
void SpatialHashTable<GridType, ValueType>::insert(const ValueType& v) requires (GridType::DIM == 2)
{
	typename GridType::BBoxType bb = vcl::boundingBox(v);

	typename GridType::CellCoord bmin = GridType::cell(bb.min);
	typename GridType::CellCoord bmax = GridType::cell(bb.max);

	for (uint i = bmin(0); i < bmax(0); ++i) {
		for (uint j = bmin(1); j < bmax(1); ++j) {
			insert(GridType::CellCoord(i,j), v);
		}
	}
}

template<typename GridType, typename ValueType>
void SpatialHashTable<GridType, ValueType>::insert(const ValueType& v) requires (GridType::DIM == 3)
{
	typename GridType::BBoxType bb = vcl::boundingBox(v);

	typename GridType::CellCoord bmin = GridType::cell(bb.min);
	typename GridType::CellCoord bmax = GridType::cell(bb.max);

	for (uint i = bmin(0); i < bmax(0); ++i) {
		for (uint j = bmin(1); j < bmax(1); ++j) {
			for (uint k = bmin(2); k < bmax(2); ++k) {
				insert(GridType::CellCoord(i,j,k), v);
			}
		}
	}
}

template<typename GridType, typename ValueType>
void SpatialHashTable<GridType, ValueType>::insert(const ValueType& v)
{
	typename GridType::BBoxType bb = vcl::boundingBox(v);

	typename GridType::CellCoord bmin = GridType::cell(bb.min);
	typename GridType::CellCoord bmax = GridType::cell(bb.max);

	// todo
}

template<typename GridType, typename ValueType>
typename SpatialHashTable<GridType, ValueType>::iterator
SpatialHashTable<GridType, ValueType>::begin()
{
	return map.begin();
}

template<typename GridType, typename ValueType>
typename SpatialHashTable<GridType, ValueType>::iterator
SpatialHashTable<GridType, ValueType>::end()
{
	return map.end();
}

template<typename GridType, typename ValueType>
typename SpatialHashTable<GridType, ValueType>::const_iterator
SpatialHashTable<GridType, ValueType>::begin() const
{
	return map.begin();
}

template<typename GridType, typename ValueType>
typename SpatialHashTable<GridType, ValueType>::const_iterator
SpatialHashTable<GridType, ValueType>::end() const
{
	return map.end();
}

} // namespace vcl
