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

#ifndef VCL_SPACE_SPATIAL_DATA_STRUCTURES_HASH_TABLE_GRID_H
#define VCL_SPACE_SPATIAL_DATA_STRUCTURES_HASH_TABLE_GRID_H

#include "grid.h"

#include <set>
#include <unordered_map>

#include <vclib/iterators/grid/hash_table_grid_iterator.h>
#include <vclib/mesh/requirements.h>
#include <vclib/misc/mark.h>
#include <vclib/space/sphere.h>

namespace vcl {

/**
 * @brief The HashTableGrid class stores N-Dimensional spatial elements (that could be anything on
 * which it can be computed a N-dimensional bounding box) in a regular grid, using a Hash Table
 * having the Cell Grid coordinate as key type.
 *
 * This Grid allows to perform insertion, deletions and queries in a time that depends only on the
 * number of elements contained in the involved cell(s) during the operation.
 * The user can permit the insertion of duplicate values by setting to true the AllowDuplicates
 * template parameter.
 */
template<typename GridType, typename ValueType, bool AllowDuplicates = true>
class HashTableGrid : public GridType
{
public:
	static_assert(
		AllowDuplicates || std::equality_comparable<ValueType>,
		"Not allowing duplicates in a Spatial Data Structures means that ValueType must implement "
		"operator==.");

	using KeyType = typename GridType::CellCoord;

	using Iterator = HashTableGridIterator<KeyType, ValueType>;

	HashTableGrid();

	HashTableGrid(const GridType& grid);

	template<PointConcept PointType>
	HashTableGrid(const PointType& min, const PointType& max, const KeyType& sizes);

	template<typename BoxType>
	HashTableGrid(const BoxType& bbox, const KeyType& sizes);

	template<typename ObjIterator>
	HashTableGrid(ObjIterator begin, ObjIterator end);

	bool empty() const;
	bool cellEmpty(const KeyType& k) const;
	std::set<KeyType> nonEmptyCells() const;

	std::size_t countInCell(const KeyType& k) const;
	uint countInSphere(const Sphere<typename GridType::ScalarType>& s) const;

	std::pair<Iterator, Iterator> valuesInCell(const KeyType& k) const;
	std::vector<Iterator> valuesInSphere(const Sphere<typename GridType::ScalarType>& s) const;

	void clear();

	bool insert(const ValueType& v);

	template<typename ObjIterator>
	uint insert(ObjIterator begin, ObjIterator end);

	bool erase(const ValueType& v);
	bool eraseCell(const KeyType& k);
	void eraseInSphere(const Sphere<typename GridType::ScalarType>& s);

	Iterator begin() const;
	Iterator end() const;

private:
	using MapType      = typename std::unordered_multimap<KeyType, vcl::Markable<ValueType>>;
	using MapIterator  = typename MapType::iterator;
	using MapValueType = typename MapType::value_type;

	mutable uint m = 1;
	MapType map;

	bool insert(const KeyType& k, const ValueType& v);
	bool erase(const KeyType& k, const ValueType& v);

	bool isMarked(MapIterator it) const;
	void mark(MapIterator it) const;
	void unMarkAll() const;
};

template<typename ValueType, bool AD = true, typename ScalarType = double>
using HashTableGrid2 = HashTableGrid<Grid2<ScalarType>, ValueType, AD>;

template<typename ValueType, bool AD = true, typename ScalarType = double>
using HashTableGrid3 = HashTableGrid<Grid3<ScalarType>, ValueType, AD>;

} // namespace vcl

#include "hash_table_grid.cpp"

#endif // VCL_SPACE_SPATIAL_DATA_STRUCTURES_HASH_TABLE_GRID_H
