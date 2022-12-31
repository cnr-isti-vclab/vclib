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
SpatialHashTable<GridType, ValueType>::SpatialHashTable(const GridType& grid) :
		GridType(grid)
{
}

template<typename GridType, typename ValueType>
template<typename PointType>
SpatialHashTable<GridType, ValueType>::SpatialHashTable(
	const PointType& min,
	const PointType& max,
	const KeyType&   size) :
		GridType(min, max, size)
{
}

template<typename GridType, typename ValueType>
template<typename BoxType>
SpatialHashTable<GridType, ValueType>::SpatialHashTable(const BoxType& bbox, const KeyType& size) :
		GridType(bbox, size)
{
}

template<typename GridType, typename ValueType>
bool SpatialHashTable<GridType, ValueType>::empty() const
{
	return map.empty();
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
std::size_t SpatialHashTable<GridType, ValueType>::cellCount(const KeyType& k) const
{
	return map.count(k);
}

template<typename GridType, typename ValueType>
std::set<typename SpatialHashTable<GridType, ValueType>::KeyType>
SpatialHashTable<GridType, ValueType>::nonEmptyCells() const
{
	std::set<KeyType> keys;
	for (const auto& p : map)
		keys.insert(p.first);
	return keys;
}

template<typename GridType, typename ValueType>
std::pair<
	typename SpatialHashTable<GridType, ValueType>::const_iterator,
	typename SpatialHashTable<GridType, ValueType>::const_iterator>
SpatialHashTable<GridType, ValueType>::cellValues(const KeyType& k) const
{
	return map.equal_range(k);
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
void SpatialHashTable<GridType, ValueType>::insert(const ValueType& v)
{
	typename GridType::BBoxType bb = vcl::boundingBox(v);

	typename GridType::CellCoord bmin = GridType::cell(bb.min);
	typename GridType::CellCoord bmax = GridType::cell(bb.max);

	for (const auto& cell : GridType::cells(bmin, bmax)) {
		insert(cell, v);
	}
}

template<typename GridType, typename ValueType>
void SpatialHashTable<GridType, ValueType>::insert(const ValueType& v)
	requires(PointConcept<ValueType>)
{
	typename GridType::CellCoord cell = GridType::cell(v);
	insert(cell, v);
}

template<typename GridType, typename ValueType>
bool SpatialHashTable<GridType, ValueType>::erase(const KeyType& k, const ValueType& v)
{
	std::pair<iterator, iterator> range = map.equal_range(k);
	for(iterator ci = range.first; ci != range.second; ++ci) {
		if (ci->second == v) {
			map.erase(ci);
			return true;
		}
	}
	return false;
}

template<typename GridType, typename ValueType>
bool SpatialHashTable<GridType, ValueType>::erase(const ValueType& v)
{
	typename GridType::BBoxType bb = vcl::boundingBox(v);

	typename GridType::CellCoord bmin = GridType::cell(bb.min);
	typename GridType::CellCoord bmax = GridType::cell(bb.max);

	bool found = false;

	for (const auto& cell : GridType::cells(bmin, bmax)) {
		found |= erase(cell, v);
	}
	return found;
}

template<typename GridType, typename ValueType>
bool SpatialHashTable<GridType, ValueType>::erase(const ValueType& v)
	requires(PointConcept<ValueType>)
{
	typename GridType::CellCoord cell = GridType::cell(v);
	return erase(cell, v);
}

template<typename GridType, typename ValueType>
bool SpatialHashTable<GridType, ValueType>::eraseCell(const KeyType& k)
{
	std::pair<iterator,iterator> range = map.equal_range(k);
	if (range != map.end()) {
		map.erase(range.first, range.second);
		return true;
	}
	return false;
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
