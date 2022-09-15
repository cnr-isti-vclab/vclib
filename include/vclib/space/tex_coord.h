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

#ifndef VCL_TEX_COORD_H
#define VCL_TEX_COORD_H

#include "point/point2.h"

namespace vcl {

template<typename Scalar>
class TexCoord
{
	template<typename S>
	friend class TexCoord;

public:
	using ScalarType = Scalar;

	TexCoord();
	TexCoord(const Scalar& s1, const Scalar& s2, short n = 0);
	TexCoord(const Point2<Scalar>& p, short n = 0);

	template<typename S>
	TexCoord<S> cast() const;

	Scalar  u() const;
	Scalar  v() const;
	Scalar& u();
	Scalar& v();
	void    setU(Scalar s);
	void    setV(Scalar s);
	void    set(Scalar u, Scalar v);

	short  nTexture() const;
	short& nTexture();

	// operators
	Scalar&       operator()(uint i);
	const Scalar& operator()(uint i) const;
	Scalar&       operator[](uint i);
	const Scalar& operator[](uint i) const;

	bool operator==(const TexCoord& t1) const = default;

private:
	Point2<Scalar> coord;
	short          n;
};

using TexCoordi = TexCoord<int>;
using TexCoordf = TexCoord<float>;
using TexCoordd = TexCoord<double>;

} // namespace vcl

#include "tex_coord.cpp"

#endif // VCL_TEX_COORD_H
