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

#ifndef VCL_SPACE_SPATIAL_HASH_TABLE_H
#define VCL_SPACE_SPATIAL_HASH_TABLE_H

#include "grid.h"

#include <set>
#include <unordered_map>

#include <vclib/mesh/requirements.h>
#include <vclib/space/sphere.h>

namespace vcl {

template<typename GridType, typename ValueType>
class SpatialHashTable : public GridType
{
public:
	using KeyType = typename GridType::CellCoord;

	using const_iterator = typename std::unordered_multimap<KeyType, ValueType>::const_iterator;

	SpatialHashTable();

	SpatialHashTable(const GridType& grid);

	template<PointConcept PointType>
	SpatialHashTable(const PointType& min, const PointType& max, const KeyType& size);

	template<typename BoxType>
	SpatialHashTable(const BoxType& bbox, const KeyType& size);

	bool empty() const;
	std::size_t size() const;
	bool cellEmpty(const KeyType& k) const;
	std::set<KeyType> nonEmptyCells() const;

	std::size_t countInCell(const KeyType& k) const;
	uint countInSphere(const Sphere<typename GridType::ScalarType>& s) const;

	std::pair<const_iterator, const_iterator> valuesInCell(const KeyType& k) const;
	std::vector<const_iterator> valuesInSphere(const Sphere<typename GridType::ScalarType>& s) const;

	void clear();

	void insert(const KeyType& k, const ValueType& v);
	void insert(const ValueType& v);

	bool erase(const KeyType& k, const ValueType& v);
	bool erase(const ValueType& v);
	bool eraseCell(const KeyType& k);
	void eraseInSphere(const Sphere<typename GridType::ScalarType>& s);

	const_iterator begin() const;
	const_iterator end() const;

private:
	using iterator = typename std::unordered_multimap<KeyType, ValueType>::iterator;
	using MapValueType = typename std::unordered_multimap<KeyType, ValueType>::value_type;

	std::unordered_multimap<KeyType, ValueType> map;
};

template<typename ValueType, typename ScalarType = double>
using SpatialHashTable2 = SpatialHashTable<Grid2<ScalarType>, ValueType>;

template<typename ValueType, typename ScalarType = double>
using SpatialHashTable3 = SpatialHashTable<Grid3<ScalarType>, ValueType>;

} // namespace vcl

#include "spatial_hash_table.cpp"

#endif // VCL_SPACE_SPATIAL_HASH_TABLE_H
