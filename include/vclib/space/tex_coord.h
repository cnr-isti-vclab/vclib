/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

template <typename Scalar>
class TexCoord
{
public:
	using ScalarType = Scalar;

	TexCoord();

	Scalar u() const;
	Scalar v() const;
	Scalar& u();
	Scalar& v();
	void setU(Scalar s);
	void setV(Scalar s);
	void set(Scalar u, Scalar v);

	short nTexture() const;
	short& nTexture();

private:
	Point2<Scalar> coord;
	short n;
};

}

#include "tex_coord.cpp"

#endif // VCL_TEX_COORD_H
