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

#ifndef VCL_SPACE_SPATIAL_DATA_STRUCTURES_GRID_STATIC_GRID_H
#define VCL_SPACE_SPATIAL_DATA_STRUCTURES_GRID_STATIC_GRID_H

#include <set>
#include <vector>

#include <vclib/algorithm/stat/bounding_box.h>
#include <vclib/misc/mark.h>
#include <vclib/space/spatial_data_structures/grid.h>

#include "abstract_ds_grid.h"
#include "iterators/static_grid_iterator.h"

namespace vcl {

template<typename GridType, typename ValueType>
class StaticGrid : public AbstractDSGrid<GridType, ValueType, StaticGrid<GridType, ValueType>>
{
	using AbstractGrid = AbstractDSGrid<GridType, ValueType, StaticGrid<GridType, ValueType>>;

	using PairType =  std::pair<uint, ValueType>;
	using PairComparator = FirstElementPairComparator<PairType>;

	friend AbstractGrid;

public:
	using KeyType = typename AbstractGrid::KeyType;
	using IsInCellFunction = typename AbstractGrid::IsInCellFunction;

	using Iterator = StaticGridIterator<KeyType, ValueType, GridType>;
	using ConstIterator = ConstStaticGridIterator<KeyType, ValueType, GridType>;

	StaticGrid();
	StaticGrid(const GridType& g);

	template<typename ObjIterator>
	StaticGrid(ObjIterator begin, ObjIterator end, const IsInCellFunction& intersects = nullptr);

	void build();

	bool empty() const;
	bool cellEmpty(const KeyType& k) const;
	std::set<KeyType> nonEmptyCells() const;

	std::size_t countInCell(const KeyType& k) const;

	std::pair<Iterator, Iterator> valuesInCell(const KeyType& k);
	std::pair<ConstIterator, ConstIterator> valuesInCell(const KeyType& k) const;

	Iterator begin();
	ConstIterator begin() const;
	Iterator end();
	ConstIterator end() const;

private:
	PairComparator comparator = PairComparator();

	// each value is stored as a pair: [cell index of the grid - value]
	// when the grid is built, this vector is sorted by the cell indices
	std::vector<PairType> values;

	// for each cell of the grid, we store the index (in the values vector ) of the first ValueType
	// object contained in the cell, or values.size() if the cell is empty
	std::vector<uint> grid;

	// not available member functions
	using AbstractGrid::erase;
	using AbstractGrid::eraseAllInCell;
	using AbstractGrid::eraseInSphere;

	bool insertInCell(const KeyType& cell, const ValueType& v);
	bool eraseInCell(const KeyType&, const ValueType&) { return false; }; // not allowing to erase
};

template<typename ValueType, bool AD = true, typename ScalarType = double>
using StaticGrid2 = StaticGrid<RegularGrid2<ScalarType>, ValueType>;

template<typename ValueType, bool AD = true, typename ScalarType = double>
using StaticGrid3 = StaticGrid<RegularGrid3<ScalarType>, ValueType>;

} // namespace vcl

#include "static_grid.cpp"

#endif // VCL_SPACE_SPATIAL_DATA_STRUCTURES_GRID_STATIC_GRID_H
