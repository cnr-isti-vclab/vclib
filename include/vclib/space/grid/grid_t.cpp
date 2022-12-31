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

#include "grid_t.h"

namespace vcl {

template<typename Scalar, int N>
Grid<Scalar, N>::Grid(
	const Point<Scalar, N>& min,
	const Point<Scalar, N>& max,
	const Point<uint, N>&   size) :
		bbox(min, max), siz(size)
{
}

template<typename Scalar, int N>
Grid<Scalar, N>::Grid(const Box<Point<Scalar, N> >& bbox, const Point<uint, N>& size) :
		bbox(bbox), siz(size)
{
}

template<typename Scalar, int N>
Point<Scalar, N> Grid<Scalar, N>::min() const
{
	return bbox.min;
}

template<typename Scalar, int N>
Point<Scalar, N> Grid<Scalar, N>::max() const
{
	return bbox.max;
}

/**
 * @brief Returns the edge legth of the bounding box of the grid in the d-th
 * dimension
 * @return
 */
template<typename Scalar, int N>
Scalar Grid<Scalar, N>::length(uint d) const
{
	return bbox.dim(d);
}

/**
 * @brief Returns the edge legths of the bounding box of the grid
 * @return
 */
template<typename Scalar, int N>
Point<Scalar, N> Grid<Scalar, N>::lengths() const
{
	Point<Scalar, N> p;
	for (size_t i = 0; i < DIM; ++i)
		p(i) = length(i);
	return p;
}

/**
 * @brief Returns the number of cells of the Grid in the d-th dimension
 * @param i
 * @return
 */
template<typename Scalar, int N>
uint Grid<Scalar, N>::cellNumber(uint d) const
{
	return siz(d);
}

/**
 * @brief Returns the number of cells for each dimension of the grid
 * @return
 */
template<typename Scalar, int N>
Point<uint, N> Grid<Scalar, N>::cellNumbers() const
{
	return siz;
}

/**
 * @brief Returns the length of a cell of the grid in the d-th dimension
 * @param i
 * @return
 */
template<typename Scalar, int N>
Scalar Grid<Scalar, N>::cellLength(uint d) const
{
	return length(d)/cellNumber(d);
}

/**
 * @brief Returns the lengths of a cell of the grid for each dimension
 * @return
 */
template<typename Scalar, int N>
Point<Scalar, N> Grid<Scalar, N>::cellLengths() const
{
	Point<Scalar, N> p;
	for (size_t i = 0; i < DIM; ++i)
		p(i) = cellLength(i);
	return p;
}

template<typename Scalar, int N>
uint Grid<Scalar, N>::cell(uint d, const Scalar& s) const
{
	Scalar t = s - bbox.min(d);
	return uint(t / cellLength(d));
}

template<typename Scalar, int N>
Point<uint, N> Grid<Scalar, N>::cell(const Point<Scalar, N>& p) const
{
	CellCoord c;
	for (size_t i = 0; i < DIM; ++i)
		c(i) = cell(i, p(i));
	return c;
}

template<typename Scalar, int N>
Point<Scalar, N> Grid<Scalar, N>::cellLowerCorner(const CellCoord& c) const
{
	Point<Scalar, N> l;
	for (size_t i = 0; i < DIM; ++i)
		l(i) = min(i) + c(i) * cellLength(i);//cellLowerCorner(i, c(i));
	return l;
}

template<typename Scalar, int N>
Box<Point<Scalar, N>> Grid<Scalar, N>::cellBox(const CellCoord& c) const
{
	Box<Point<Scalar, N>> b;

	Point<Scalar, N> p;
	for (size_t i = 0; i < DIM; ++i)
		p(i) = (Scalar)c(i)*cellLength(i);
	p += bbox.min;
	b.min = p;
	b.max = (p + cellLengths());

	return b;
}

template<typename Scalar, int N>
typename Grid<Scalar, N>::CellIterator Grid<Scalar, N>::cellBegin() const
{
	return CellIterator(CellCoord(), siz);
}

template<typename Scalar, int N>
typename Grid<Scalar, N>::CellIterator
Grid<Scalar, N>::cellBegin(const CellCoord& first, const CellCoord& last) const
{
	return CellIterator(first, last + 1);
}

template<typename Scalar, int N>
typename Grid<Scalar, N>::CellIterator Grid<Scalar, N>::cellEnd() const
{
	return CellIterator();
}

template<typename Scalar, int N>
typename Grid<Scalar, N>::CellRangeIterator Grid<Scalar, N>::cells() const
{
	return CellRangeIterator(
		*this,
		&Grid<Scalar, N>::cellBegin,
		&Grid<Scalar, N>::cellEnd);
}

template<typename Scalar, int N>
typename Grid<Scalar, N>::CellRangeIterator
Grid<Scalar, N>::cells(const CellCoord& first, const CellCoord& last) const
{
	return CellRangeIterator(
		*this,
		&Grid<Scalar, N>::cellBegin,
		&Grid<Scalar, N>::cellEnd,
		first,
		last);
}

} // namespace vcl
