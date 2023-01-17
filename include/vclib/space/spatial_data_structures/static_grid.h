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

#ifndef VCL_SPACE_SPATIAL_DATA_STRUCTURES_STATIC_GRID_H
#define VCL_SPACE_SPATIAL_DATA_STRUCTURES_STATIC_GRID_H

#include <set>
#include <vector>

#include "grid.h"

#include <vclib/algorithms/stat/bounding_box.h>
#include <vclib/misc/comparators.h>
#include <vclib/misc/mark.h>

namespace vcl {

template<typename GridType, typename ValueType>
class StaticGrid : public GridType
{
public:
	using KeyType = typename GridType::CellCoord;

	StaticGrid();

	template<typename ObjIterator>
	StaticGrid(ObjIterator begin, ObjIterator end);

	void build();

	bool empty() const;
	bool cellEmpty(const KeyType& k) const;
	std::set<KeyType> nonEmptyCells() const;

	std::size_t countInCell(const KeyType& k) const;

private:
	static const FirstElementPairComparator<std::pair<uint,  vcl::Markable<ValueType>>> comparator;

	// each value is stored as a pair: [cell index of the grid - value]
	// when the grid is built, this vector is sorted by the cell indices
	std::vector<std::pair<uint, vcl::Markable<ValueType>>> values;

	// for each cell of the grid, we store the index (in the values vector ) of the first ValueType
	// object contained in the cell, or values.size() if the cell is empty
	std::vector<uint> grid;

	template<typename ObjIterator>
	void insertElements(ObjIterator begin, ObjIterator end);

	void insert(typename GridType::CellCoord& cell, const ValueType& v);
};

template<typename ValueType, bool AD = true, typename ScalarType = double>
using StaticGrid2 = StaticGrid<Grid2<ScalarType>, ValueType>;

template<typename ValueType, bool AD = true, typename ScalarType = double>
using StaticGrid3 = StaticGrid<Grid3<ScalarType>, ValueType>;

} // namespace vcl

#include "static_grid.cpp"

#endif // VCL_SPACE_SPATIAL_DATA_STRUCTURES_STATIC_GRID_H
