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

#ifndef VCL_SPACE_GRID_STATIC_GRID_H
#define VCL_SPACE_GRID_STATIC_GRID_H

#include <set>
#include <vector>

#include <vclib/concepts/ranges/mesh/vertex_range.h>
#include <vclib/iterators/space/grid/static_grid_iterator.h>

#include "abstract_grid.h"
#include "regular_grid.h"


namespace vcl {

template<typename GridType, typename ValueType>
class StaticGrid : public AbstractGrid<GridType, ValueType, StaticGrid<GridType, ValueType>>
{
	using AbsGrid = AbstractGrid<GridType, ValueType, StaticGrid<GridType, ValueType>>;

	using PairType =  std::pair<uint, ValueType>;
	using PairComparator = FirstElementPairComparator<PairType>;

	friend AbsGrid;

public:
	using KeyType = AbsGrid::KeyType;
	using IsInCellFunction = AbsGrid::IsInCellFunction;

	using Iterator = StaticGridIterator<KeyType, ValueType, GridType>;
	using ConstIterator = ConstStaticGridIterator<KeyType, ValueType, GridType>;

	StaticGrid();
	StaticGrid(const GridType& g);

	template<typename ObjIterator>
	StaticGrid(ObjIterator begin, ObjIterator end, const IsInCellFunction& intersects = nullptr);

	template<vcl::Range Rng>
	StaticGrid(Rng&& r, const IsInCellFunction& intersects = nullptr);

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
	using AbsGrid::erase;
	using AbsGrid::eraseAllInCell;
	using AbsGrid::eraseInSphere;

	bool insertInCell(const KeyType& cell, const ValueType& v);
	bool eraseInCell(const KeyType&, const ValueType&) { return false; }; // not allowing to erase
};

// deduction guides

template<PointIteratorConcept It>
StaticGrid(It, It) -> StaticGrid<
	RegularGrid<typename It::value_type, It::value_type::DIM>,
	typename It::value_type::ScalarType>;

template<PointIteratorConcept It, typename F>
StaticGrid(It, It, F) -> StaticGrid<
	RegularGrid<typename It::value_type, It::value_type::DIM>,
	typename It::value_type::ScalarType>;

template<VertexPointerRangeConcept Rng>
StaticGrid(Rng) -> StaticGrid<
	RegularGrid<
		// scalar type used for the grid, the same of the CoordType of the Vertex
		typename std::remove_pointer_t<
			typename std::ranges::iterator_t<Rng>::value_type>::CoordType::ScalarType,
		3>, // the dimension of the Grid
	// the ValueType of the StaticGrid, which is the iterated type in the given range
	// (pointer to vertex)
	typename std::ranges::iterator_t<Rng>::value_type>;

template<typename ValueType, typename ScalarType = double>
using StaticGrid2 = StaticGrid<RegularGrid2<ScalarType>, ValueType>;

template<typename ValueType, typename ScalarType = double>
using StaticGrid3 = StaticGrid<RegularGrid3<ScalarType>, ValueType>;

} // namespace vcl

#include "static_grid.cpp"

#endif // VCL_SPACE_GRID_STATIC_GRID_H
