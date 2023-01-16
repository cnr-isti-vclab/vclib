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
#include <vclib/misc/markable_vector.h>
#include <vclib/space/sphere.h>

namespace vcl {

template<typename GridType, typename ValueType>
class HashTableGrid : public GridType
{
public:
	using KeyType = typename GridType::CellCoord;

	using Iterator = HashTableGridIterator<KeyType, ValueType>;

	HashTableGrid();

	HashTableGrid(const GridType& grid);

	template<PointConcept PointType>
	HashTableGrid(const PointType& min, const PointType& max, const KeyType& sizes);

	template<typename BoxType>
	HashTableGrid(const BoxType& bbox, const KeyType& sizes);

	bool empty() const;
	std::size_t size() const;
	bool cellEmpty(const KeyType& k) const;
	std::set<KeyType> nonEmptyCells() const;

	std::size_t countInCell(const KeyType& k) const;
	uint countInSphere(const Sphere<typename GridType::ScalarType>& s) const;

	std::pair<Iterator, Iterator> valuesInCell(const KeyType& k) const;
	std::vector<Iterator> valuesInSphere(const Sphere<typename GridType::ScalarType>& s) const;

	void clear();

	bool insert(const ValueType& v);

	bool erase(const ValueType& v);
	bool eraseCell(const KeyType& k);
	void eraseInSphere(const Sphere<typename GridType::ScalarType>& s);

	Iterator begin() const;
	Iterator end() const;

private:
	using iterator = typename std::unordered_multimap<KeyType, uint>::iterator;
	using MapValueType = typename std::unordered_multimap<KeyType, uint>::value_type;

	vcl::MarkableVector<ValueType> values;
	std::size_t valuesNumber = 0; // actual number of values accessible by the map, could differ
								  // from values.size()

	std::unordered_multimap<KeyType, uint> map;

	bool insert(const KeyType& k, const ValueType& v, uint vid);
	std::pair<bool, uint> erase(const KeyType& k, const ValueType& v);
};

template<typename ValueType, typename ScalarType = double>
using SpatialHashTable2 = HashTableGrid<Grid2<ScalarType>, ValueType>;

template<typename ValueType, typename ScalarType = double>
using SpatialHashTable3 = HashTableGrid<Grid3<ScalarType>, ValueType>;

} // namespace vcl

#include "hash_table_grid.cpp"

#endif // VCL_SPACE_SPATIAL_DATA_STRUCTURES_HASH_TABLE_GRID_H
