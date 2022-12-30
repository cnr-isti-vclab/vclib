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

#ifndef VCL_SPACE_GRID_GRID_T_H
#define VCL_SPACE_GRID_GRID_T_H

#include <vclib/space/box.h>

namespace vcl {

template<typename Scalar, int N>
class Grid
{
public:
	using ScalarType      = Scalar;
	using PointType       = Point<Scalar, N>;
	using CellCoord       = Point<uint, N>;
	using BBoxType        = Box<PointType>;
	static const int DIM  = N;

	Grid() = default;
	Grid(const Point<Scalar, N>& min, const Point<Scalar, N>& max, const Point<uint, N>& size);
	Grid(const Box<Point<Scalar, N>>& bbox, const Point<uint, N>& size);

	Point<Scalar, N> min() const;
	Point<Scalar, N> max() const;

	Scalar length(uint d) const;
	Point<Scalar, N> lengths() const;

	uint cellNumber(uint d) const;
	Point<uint, N> cellNumbers() const;

	Scalar cellLength(uint d) const;
	Point<Scalar, N> cellLengths() const;

	uint cell(uint d, const Scalar& s) const;
	CellCoord cell(const Point<Scalar, N>& p) const;

	Point<Scalar, N> cellLowerCorner(const CellCoord& c) const;

	Box<Point<Scalar, N>> cellBox(const CellCoord& c) const;

private:
	BBoxType       bbox;
	Point<uint, N> siz;
};

} // namespace vcl

#include "grid_t.cpp"

#endif // VCL_SPACE_GRID_GRID_T_H
