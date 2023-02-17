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

#ifndef VCL_SPACE_SPATIAL_DATA_STRUCTURES_GRID_ITERATORS_CELL_RANGE_ITERATOR_H
#define VCL_SPACE_SPATIAL_DATA_STRUCTURES_GRID_ITERATORS_CELL_RANGE_ITERATOR_H

#include <vclib/iterators/range_iterator.h>

#include <vclib/space/point.h>

namespace vcl {

template<typename Container, typename ConstIterator>
class CellRangeIterator : public ConstRangeIterator<Container, ConstIterator>
{
private:
	using Base = ConstRangeIterator<Container, ConstIterator>;
	using CellCoord = typename Container::CellCoord;
public:

	CellRangeIterator(
		const Container& c,
		ConstIterator (Container::*beginFunction)() const,
		ConstIterator (Container::*endFunction)() const) :
			Base(c, beginFunction, endFunction)
		{
			last = c.cellNumbers() - 1;
		};

	CellRangeIterator(
		const Container& c,
		ConstIterator (Container::*beginFunction)() const,
		ConstIterator (Container::*endFunction)() const,
		const CellCoord& first,
		const CellCoord& last) :
			Base(c, beginFunction, endFunction), first(first), last(last)
		{};

	ConstIterator begin() const
	{
		return Base::c.cellBegin(first, last);
	}
private:
	CellCoord first, last;
};

} // namespace vcl

#endif // VCL_SPACE_SPATIAL_DATA_STRUCTURES_GRID_ITERATORS_CELL_RANGE_ITERATOR_H
