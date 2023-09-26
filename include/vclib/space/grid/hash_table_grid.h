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

#ifndef VCL_SPACE_GRID_HASH_TABLE_GRID_H
#define VCL_SPACE_GRID_HASH_TABLE_GRID_H

#include <set>
#include <unordered_map>

#include <vclib/mesh/requirements.h>

#include "abstract_grid.h"
#include "regular_grid.h"

namespace vcl {

/******************************************************************************
 *                                Declarations                                *
 ******************************************************************************/

/**
 * @brief The HashTableGrid class stores N-Dimensional spatial elements (that could be anything on
 * which it can be computed a N-dimensional bounding box) in a regular grid, using a Hash Table
 * having the Cell Grid coordinate as key type.
 *
 * This Grid allows to perform insertion, deletions and queries in a time that depends only on the
 * number of elements contained in the involved cell(s) during the operation. The total overhead of
 * managing this data structure is the same of managing an std::unordered_multimap.
 * The user can allow or disallow the insertion of duplicate values by setting the boolean
 * AllowDuplicates template parameter, that is defaulted to `true`.
 *
 * @ingroup space
 */
template<typename GridType, typename ValueType, bool AllowDuplicates = true>
class HashTableGrid : public AbstractGrid<GridType, ValueType, HashTableGrid<GridType, ValueType, AllowDuplicates>>
{
	using AbsGrid =
		AbstractGrid<GridType, ValueType, HashTableGrid<GridType, ValueType, AllowDuplicates>>;

	friend AbsGrid;

public:
	static_assert(
		AllowDuplicates || std::equality_comparable<ValueType>,
		"Not allowing duplicates in a Spatial Data Structures means that ValueType must implement "
		"operator==.");

	using KeyType = AbsGrid::KeyType;
	using IsInCellFunction = AbsGrid::IsInCellFunction;

	using Iterator = std::unordered_multimap<KeyType, ValueType>::iterator;
	using ConstIterator = std::unordered_multimap<KeyType, ValueType>::const_iterator;

	HashTableGrid();
	HashTableGrid(const GridType& g);

	template<typename ObjIterator>
	HashTableGrid(ObjIterator begin, ObjIterator end, const IsInCellFunction& intersects = nullptr);
	
	template<vcl::Range Rng>
	HashTableGrid(Rng&& r, const IsInCellFunction& intersects = nullptr);

	bool empty() const;
	bool cellEmpty(const KeyType& k) const;
	std::set<KeyType> nonEmptyCells() const;

	std::size_t countInCell(const KeyType& k) const;

	std::pair<Iterator, Iterator> valuesInCell(const KeyType& k);
	std::pair<ConstIterator, ConstIterator> valuesInCell(const KeyType& k) const;

	void clear();

	bool eraseAllInCell(const KeyType& k);
	void eraseInSphere(const Sphere<typename GridType::ScalarType>& s);

	Iterator begin();
	ConstIterator begin() const;
	Iterator end();
	ConstIterator end() const;

private:
	using MapType      = std::unordered_multimap<KeyType, ValueType>;
	using MapValueType = MapType::value_type;

	mutable uint m = 1;
	MapType map;

	bool insertInCell(const KeyType& k, const ValueType& v);
	bool eraseInCell(const KeyType& k, const ValueType& v);
};

template<typename ValueType, typename ScalarType = double, bool AllowDuplicates = true>
using HashTableGrid2 = HashTableGrid<RegularGrid2<ScalarType>, ValueType, AllowDuplicates>;

template<typename ValueType, typename ScalarType = double, bool AllowDuplicates = true>
using HashTableGrid3 = HashTableGrid<RegularGrid3<ScalarType>, ValueType, AllowDuplicates>;

/******************************************************************************
 *                                Definitions                                 *
 ******************************************************************************/

template<typename GridType, typename ValueType, bool AllowDuplicates>
HashTableGrid<GridType, ValueType, AllowDuplicates>::HashTableGrid()
{
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
HashTableGrid<GridType, ValueType, AllowDuplicates>::HashTableGrid(const GridType& g) :
		AbsGrid(g)
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
		AbsGrid(begin, end, intersects)
{
	AbsGrid::insert(begin, end);
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
template<vcl::Range Rng>
HashTableGrid<GridType, ValueType, AllowDuplicates>::HashTableGrid(
	Rng&& r,
	const IsInCellFunction& intersects) :
		HashTableGrid(std::ranges::begin(r), std::ranges::end(r), intersects)
{
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
	std::pair<Iterator,Iterator> range = map.equal_range(k);
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
	std::vector<ConstIterator> toDel = AbsGrid::valuesInSphere(s);
	for (auto& it : toDel)
		map.erase(it);
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
typename HashTableGrid<GridType, ValueType, AllowDuplicates>::Iterator
HashTableGrid<GridType, ValueType, AllowDuplicates>::begin()
{
	return map.begin();
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
typename HashTableGrid<GridType, ValueType, AllowDuplicates>::ConstIterator
HashTableGrid<GridType, ValueType, AllowDuplicates>::begin() const
{
	return map.begin();
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
typename HashTableGrid<GridType, ValueType, AllowDuplicates>::Iterator
HashTableGrid<GridType, ValueType, AllowDuplicates>::end()
{
	return map.end();
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
typename HashTableGrid<GridType, ValueType, AllowDuplicates>::ConstIterator
HashTableGrid<GridType, ValueType, AllowDuplicates>::end() const
{
	return map.end();
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
bool HashTableGrid<GridType, ValueType, AllowDuplicates>::insertInCell(
	const KeyType&   k,
	const ValueType& v)
{
	if constexpr (AllowDuplicates) {
		map.emplace(k, v);
		return true;
	}
	else {
		auto range = map.equal_range(k);
		bool found = false;
		for(Iterator ci = range.first; ci != range.second && !found; ++ci) {
			if (ci->second == v) {
				found = true;
			}
		}
		if (!found)
			map.emplace(k, v);
		return !found;
	}
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
bool HashTableGrid<GridType, ValueType, AllowDuplicates>::eraseInCell(
	const KeyType& k,
	const ValueType& v)
{
	bool found = false;

	std::pair<Iterator, Iterator> range = map.equal_range(k);
	for(Iterator ci = range.first; ci != range.second; ++ci) {
		if (ci->second == v) {
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

#endif // VCL_SPACE_GRID_HASH_TABLE_GRID_H
