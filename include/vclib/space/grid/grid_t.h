/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#ifndef VCL_GRID_T_H
#define VCL_GRID_T_H

#include <vclib/space/box.h>

namespace vcl {

template<typename Scalar, int N>
class Grid
{
public:
	using ScalarType      = Scalar;
	static const int DIM  = N;

	Grid() = default;
	Grid(const Point<Scalar, N>& min, const Point<Scalar, N>& max, const Point<uint, N>& size);

	Point<Scalar, N> min() const;
	Point<Scalar, N> max() const;

	Scalar length(uint d) const;
	Point<Scalar, N> lengths() const;

	uint size(uint d) const;
	Point<uint, N> size() const;

	Scalar cellLength(uint d) const;
	Point<Scalar, N> cellLengths() const;

	uint cell(uint d, const Scalar& s);
	Point<uint, N> cell(const Point<Scalar, N>& p);

	Scalar cellLowerCorner(uint d, uint c);
	Point<Scalar, N> cellLowerCorner(const Point<uint, N>& c);

private:
	Box<Point<Scalar, N>> bbox;
	Point<uint, N>        siz;
};

} // namespace vcl

#include "grid_t.h"

#endif // VCL_GRID_T_H
