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

#include "static_grid.h"

namespace vcl {

template<typename GridType, typename ValueType>
StaticGrid<GridType, ValueType>::StaticGrid()
{
}

template<typename GridType, typename ValueType>
StaticGrid<GridType, ValueType>::StaticGrid(const GridType& g) :
		AbstractGrid(g)
{
}

template<typename GridType, typename ValueType>
template<typename ObjIterator>
StaticGrid<GridType, ValueType>::StaticGrid(
	ObjIterator             begin,
	ObjIterator             end,
	const IsInCellFunction& intersects) :
		AbstractGrid(begin, end, intersects)
{
	AbstractGrid::insert(begin, end);
	build();
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
std::pair<
	typename StaticGrid<GridType, ValueType>::Iterator,
	typename StaticGrid<GridType, ValueType>::Iterator>
StaticGrid<GridType, ValueType>::valuesInCell(const KeyType& k)
{
	uint ind = GridType::indexOfCell(k);
	uint i = grid[ind];
	if (i < values.size()) {
		std::pair<Iterator, Iterator> p;
		p.first = Iterator(values.begin() + i, (const GridType&)*this);
		while(i < values.size() && values[i].first == ind) {
			i++;
		}
		auto it = i < values.size() ? values.begin() + i : values.end();
		p.second = Iterator(it, (const GridType&)*this);
		return p;
	}
	else {
		return std::make_pair(end(), end());
	}
}

template<typename GridType, typename ValueType>
std::pair<
	typename StaticGrid<GridType, ValueType>::ConstIterator,
	typename StaticGrid<GridType, ValueType>::ConstIterator>
StaticGrid<GridType, ValueType>::valuesInCell(const KeyType& k) const
{
	uint ind = GridType::indexOfCell(k);
	uint i = grid[ind];
	if (i < values.size()) {
		std::pair<ConstIterator, ConstIterator> p;
		p.first = ConstIterator(values.begin() + i, (const GridType&)*this);
		while(i < values.size() && values[i].first == ind) {
			i++;
		}
		auto it = i < values.size() ? values.begin() + i : values.end();
		p.second = ConstIterator(it, (const GridType&)*this);
		return p;
	}
	else {
		return std::make_pair(end(), end());
	}
}

template<typename GridType, typename ValueType>
typename StaticGrid<GridType, ValueType>::Iterator StaticGrid<GridType, ValueType>::begin()
{
	return Iterator(values.begin(), (const GridType&)*this);
}

template<typename GridType, typename ValueType>
typename StaticGrid<GridType, ValueType>::ConstIterator
StaticGrid<GridType, ValueType>::begin() const
{
	return ConstIterator(values.begin(), (const GridType&)*this);
}

template<typename GridType, typename ValueType>
typename StaticGrid<GridType, ValueType>::Iterator StaticGrid<GridType, ValueType>::end()
{
	return Iterator(values.end(), (const GridType&)*this);
}

template<typename GridType, typename ValueType>
typename StaticGrid<GridType, ValueType>::ConstIterator StaticGrid<GridType, ValueType>::end() const
{
	return ConstIterator(values.end(), (const GridType&)*this);
}

template<typename GridType, typename ValueType>
bool StaticGrid<GridType, ValueType>::insertInCell(const KeyType& cell, ValueType* v)
{
	uint cellIndex = GridType::indexOfCell(cell);
	values.push_back(std::make_pair(cellIndex, v));
	return true;
}

} // namespace vcl
