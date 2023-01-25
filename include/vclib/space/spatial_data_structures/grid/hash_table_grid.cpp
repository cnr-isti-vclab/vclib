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

#include "hash_table_grid.h"

#include <vclib/algorithms/stat/bounding_box.h>

namespace vcl {

template<typename GridType, typename ValueType, bool AllowDuplicates>
HashTableGrid<GridType, ValueType, AllowDuplicates>::HashTableGrid()
{
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
HashTableGrid<GridType, ValueType, AllowDuplicates>::HashTableGrid(const GridType& g) :
		AbstractGrid(g)
{
}

/**
 * @brief Creates an HashTableGrid that contains all the elements that can be iterated from `begin`
 * to `end`.
 *
 * The bounding box and the sizes of the Grid are automatically computed.
 * Bounding box is computed starting from the bounding box of all the iterated elements, and then
 * inflated. The number of cells per dimension is computed using the `vcl::bestGridSize` function.
 *
 * @param begin
 * @param end
 */
template<typename GridType, typename ValueType, bool AllowDuplicates>
template<typename ObjIterator>
HashTableGrid<GridType, ValueType, AllowDuplicates>::HashTableGrid(
	ObjIterator             begin,
	ObjIterator             end,
	const IsInCellFunction& intersects) :
		AbstractGrid(begin, end, intersects)
{
	AbstractGrid::insert(begin, end);
}

/**
 * @brief Returns true if the HashTableGrid is empty (no elements in it).
 * @return
 */
template<typename GridType, typename ValueType, bool AllowDuplicates>
bool HashTableGrid<GridType, ValueType, AllowDuplicates>::empty() const
{
	return map.empty();
}

/**
 * @brief Returns true if the given cell coordinate does not contain elements in it.
 * @param k
 * @return
 */
template<typename GridType, typename ValueType, bool AllowDuplicates>
bool HashTableGrid<GridType, ValueType, AllowDuplicates>::cellEmpty(const KeyType& k) const
{
	return map.find(k) == map.end();
}

/**
 * @brief Returns an std::set containing the cell coordinates of all the cells that contain at least
 * one element.
 * @return
 */
template<typename GridType, typename ValueType, bool AllowDuplicates>
std::set<typename HashTableGrid<GridType, ValueType, AllowDuplicates>::KeyType>
HashTableGrid<GridType, ValueType, AllowDuplicates>::nonEmptyCells() const
{
	std::set<KeyType> keys;
	for (const auto& p : map)
		keys.insert(p.first);
	return keys;
}

/**
 * @brief Returns the number of elements contained in the given cell.
 * @param k
 * @return
 */
template<typename GridType, typename ValueType, bool AllowDuplicates>
std::size_t HashTableGrid<GridType, ValueType, AllowDuplicates>::countInCell(const KeyType& k) const
{
	return map.count(k);
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
std::pair<
	typename HashTableGrid<GridType, ValueType, AllowDuplicates>::Iterator,
	typename HashTableGrid<GridType, ValueType, AllowDuplicates>::Iterator>
HashTableGrid<GridType, ValueType, AllowDuplicates>::valuesInCell(const KeyType& k)
{
	auto p = map.equal_range(k);
	return std::make_pair(Iterator(p.first), Iterator(p.second));
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
std::pair<
	typename HashTableGrid<GridType, ValueType, AllowDuplicates>::ConstIterator,
	typename HashTableGrid<GridType, ValueType, AllowDuplicates>::ConstIterator>
HashTableGrid<GridType, ValueType, AllowDuplicates>::valuesInCell(const KeyType& k) const
{
	auto p = map.equal_range(k);
	return std::make_pair(ConstIterator(p.first), ConstIterator(p.second));
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
void HashTableGrid<GridType, ValueType, AllowDuplicates>::clear()
{
	map.clear();
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
bool HashTableGrid<GridType, ValueType, AllowDuplicates>::eraseAllInCell(const KeyType& k)
{
	std::pair<MapIterator,MapIterator> range = map.equal_range(k);
	if (range != map.end()) {
		map.erase(range.first, range.second);
		return true;
	}
	return false;
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
void HashTableGrid<GridType, ValueType, AllowDuplicates>::eraseInSphere(
	const Sphere<typename GridType::ScalarType>& s)
{
	std::vector<Iterator> toDel = AbstractGrid::valuesInSphere(s);
	for (auto& it : toDel)
		map.erase(it.mapIt);
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
typename HashTableGrid<GridType, ValueType, AllowDuplicates>::Iterator
HashTableGrid<GridType, ValueType, AllowDuplicates>::begin()
{
	return Iterator(map.begin());
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
typename HashTableGrid<GridType, ValueType, AllowDuplicates>::ConstIterator
HashTableGrid<GridType, ValueType, AllowDuplicates>::begin() const
{
	return ConstIterator(map.begin());
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
typename HashTableGrid<GridType, ValueType, AllowDuplicates>::Iterator
HashTableGrid<GridType, ValueType, AllowDuplicates>::end()
{
	return Iterator(map.end());
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
typename HashTableGrid<GridType, ValueType, AllowDuplicates>::ConstIterator
HashTableGrid<GridType, ValueType, AllowDuplicates>::end() const
{
	return ConstIterator(map.end());
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
bool HashTableGrid<GridType, ValueType, AllowDuplicates>::insertInCell(
	const KeyType&       k,
	MarkableValuePointer v)
{
	if constexpr (AllowDuplicates) {
		map.insert(MapValueType(k, v));
		return true;
	}
	else {
		auto range = map.equal_range(k);
		bool found = false;
		for(MapIterator ci = range.first; ci != range.second && !found; ++ci) {
			if (ci->second->get() == v->get()) {
				found = true;
			}
		}
		if (!found)
			map.insert(MapValueType(k, v));
		return !found;
	}
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
bool HashTableGrid<GridType, ValueType, AllowDuplicates>::eraseInCell(
	const KeyType& k,
	const ValueType& v)
{
	bool found = false;

	std::pair<MapIterator, MapIterator> range = map.equal_range(k);
	for(MapIterator ci = range.first; ci != range.second; ++ci) {
		if (ci->second->get() == v) {
			found = true;
			map.erase(ci);
			if constexpr (!AllowDuplicates) {
				return true;
			}
		}
	}
	return found;
}

} // namespace vcl
