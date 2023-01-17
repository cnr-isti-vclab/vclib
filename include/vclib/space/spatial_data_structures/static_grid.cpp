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

#include "static_grid.h"

namespace vcl {

template<typename GridType, typename ValueType>
StaticGrid<GridType, ValueType>::StaticGrid()
{
}

template<typename GridType, typename ValueType>
void StaticGrid<GridType, ValueType>::build()
{
	uint totCellNumber = 1;
	for (uint i = 0; i < GridType::DIM; ++i) {
		totCellNumber *= GridType::cellNumber(i);
	}

	grid.resize(totCellNumber);

	std::sort(values.begin(), values.end(), comparator);

	uint vi = 0; // values[vi].first points to the next non empty cell in the grid

	for (uint ci = 0; ci < grid.size(); ++ci) { // for each cell
		if (vi < values.size() && values[vi].first == ci) // values[vi] is in the cell ci
			grid[ci] = vi;
		else { // there are no values in this grid cell
			grid[ci] = values.size(); // set the sentinel value
		}

		// move vi to the next non-empty cell
		// skip all the values that are in the current cell ci
		// won't increment vi if the values[vi].first is not equal to ci
		while (vi < values.size() && values[vi].first == ci) {
			vi++;
		}
	}
}

template<typename GridType, typename ValueType>
bool StaticGrid<GridType, ValueType>::empty() const
{
	return values.empty();
}

template<typename GridType, typename ValueType>
bool StaticGrid<GridType, ValueType>::cellEmpty(const KeyType& k) const
{
	uint ind = GridType::indexOfCell(k);
	return grid[ind] == values.size();
}

template<typename GridType, typename ValueType>
std::set<typename StaticGrid<GridType, ValueType>::KeyType>
StaticGrid<GridType, ValueType>::nonEmptyCells() const
{
	std::set<KeyType> keys;
	uint actualInd = values.size();
	for (uint i = 0; i < values.size(); ++i) {
		if (values[i].first != actualInd) {
			actualInd = values[i].first;
			keys.insert(GridType::cellOfIndex(actualInd));
		}
	}
	return keys;
}

template<typename GridType, typename ValueType>
std::size_t StaticGrid<GridType, ValueType>::countInCell(const KeyType& k) const
{
	uint ind = GridType::indexOfCell(k);
	uint i = grid[ind];
	uint cnt = 0;

	while(i < values.size() && values[i].first == ind) {
		i++;
		cnt++;
	}
	return cnt;
}

template<typename GridType, typename ValueType>
template<typename ObjIterator>
StaticGrid<GridType, ValueType>::StaticGrid(ObjIterator begin, ObjIterator end)
{
	using ScalarType = typename GridType::ScalarType;
	using BBoxType = typename GridType::BBoxType;
	using CellCoord = typename GridType::CellCoord;

	BBoxType bbox = boundingBox(begin, end);
	uint nElements = std::distance(begin, end);

	if (nElements > 0) {
		// inflated bb
		ScalarType infl = bbox.diagonal()/nElements;
		bbox.min -= infl;
		bbox.max += infl;

		CellCoord sizes = bestGridSize(bbox.size(), nElements);

		GridType::set(bbox, sizes);

		insertElements(begin, end);
	}
}

template<typename GridType, typename ValueType>
template<typename ObjIterator>
void StaticGrid<GridType, ValueType>::insertElements(ObjIterator begin, ObjIterator end)
{
	using TMPVT = typename std::remove_pointer<ValueType>::type;
	using VT = typename std::remove_reference<TMPVT>::type;

	for (ObjIterator it = begin; it != end; ++it) {
		const VT* vv = nullptr;
		if constexpr(std::is_pointer<ValueType>::value) {
			if (*it != nullptr)
				vv = (*it);
		}
		else {
			vv = &(*it);
		}
		if (vv) {
			if constexpr (PointConcept<VT>) {
				typename GridType::CellCoord cell = GridType::cell(*vv);
				insert(cell, *it);
			}
			else if constexpr (VertexConcept<VT>) {
				typename GridType::CellCoord cell = GridType::cell(vv->coord());
				insert(cell, *it);
			}
			else {
				typename GridType::BBoxType bb = vcl::boundingBox(*vv);

				typename GridType::CellCoord bmin = GridType::cell(bb.min);
				typename GridType::CellCoord bmax = GridType::cell(bb.max);

				for (auto cell : GridType::cells(bmin, bmax)) {
					insert(cell, *it);
				}
			}
		}
	}

	build();
}

template<typename GridType, typename ValueType>
void StaticGrid<GridType, ValueType>::insert(
	typename GridType::CellCoord& cell,
	const ValueType&              v)
{
	uint cellIndex = GridType::indexOfCell(cell);
	values.push_back(std::make_pair(cellIndex, vcl::Markable<ValueType>(v)));
}

} // namespace vcl
