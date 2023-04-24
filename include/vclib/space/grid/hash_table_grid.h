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

	using KeyType = typename AbsGrid::KeyType;
	using IsInCellFunction = typename AbsGrid::IsInCellFunction;

	using Iterator = typename std::unordered_multimap<KeyType, ValueType>::iterator;
	using ConstIterator = typename std::unordered_multimap<KeyType, ValueType>::const_iterator;

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
	using MapType      = typename std::unordered_multimap<KeyType, ValueType>;
	using MapValueType = typename MapType::value_type;

	mutable uint m = 1;
	MapType map;

	bool insertInCell(const KeyType& k, const ValueType& v);
	bool eraseInCell(const KeyType& k, const ValueType& v);
};

template<typename ValueType, typename ScalarType = double, bool AllowDuplicates = true>
using HashTableGrid2 = HashTableGrid<RegularGrid2<ScalarType>, ValueType, AllowDuplicates>;

template<typename ValueType, typename ScalarType = double, bool AllowDuplicates = true>
using HashTableGrid3 = HashTableGrid<RegularGrid3<ScalarType>, ValueType, AllowDuplicates>;

} // namespace vcl

#include "hash_table_grid.cpp"

#endif // VCL_SPACE_GRID_HASH_TABLE_GRID_H
