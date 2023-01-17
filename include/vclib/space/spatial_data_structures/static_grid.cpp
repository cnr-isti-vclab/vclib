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

	uint totCellNumber = 1;
	for (uint i = 0; i < GridType::DIM; ++i) {
		totCellNumber *= GridType::cellNumber(i);
	}

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
				insertNode(cell, *it);
			}
			else if constexpr (VertexConcept<VT>) {
				typename GridType::CellCoord cell = GridType::cell(vv->coord());
				insertNode(cell, *it);
			}
			else {
				typename GridType::BBoxType bb = vcl::boundingBox(*vv);

				typename GridType::CellCoord bmin = GridType::cell(bb.min);
				typename GridType::CellCoord bmax = GridType::cell(bb.max);

				for (auto cell : GridType::cells(bmin, bmax)) {
					insertNode(cell, *it);
				}
			}
		}
	}


	values.push_back({totCellNumber, vcl::Markable<ValueType>(*begin)}); // sentinel

	grid.resize(totCellNumber);


	std::sort(
		values.begin(),
		values.end(),
		FirstElementPairComparator<std::pair<uint,  vcl::Markable<ValueType>>>());

	uint vi = 0; // values[vi].first points to the next non empty cell in the grid

	// for each cell
	for (uint ci = 0; ci < grid.size(); ++ci) {
		if (values[vi].first == ci) // values[vi] is in the cell ci
			grid[ci] = vi;
		else { // there are no values in this grid cell
			grid[ci] = values.size() - 1; // set the sentinel value
		}

		// move vi to the next non-empty cell
		// skip all the values that are in the current cell ci
		while (ci == values[vi].first) {
			vi++;
		}
	}

	values.pop_back(); // remove sentinel
}

template<typename GridType, typename ValueType>
void StaticGrid<GridType, ValueType>::insertNode(typename GridType::CellCoord& cell, const ValueType& v)
{
	uint cellIndex = GridType::indexOfCell(cell);
	values.push_back(std::make_pair(cellIndex, vcl::Markable<ValueType>(v)));
}

} // namespace vcl
