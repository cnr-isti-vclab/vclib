/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

template <typename ScalarType>
class TexCoord
{
public:
	TexCoord();

	ScalarType u() const;
	ScalarType v() const;
	ScalarType& u();
	ScalarType& v();
	void setU(ScalarType s);
	void setV(ScalarType s);
	void set(ScalarType u, ScalarType v);

	short nTexture() const;
	short& nTexture();

private:
	Point2<ScalarType> coord;
	short n;
};

}

#include "tex_coord.cpp"

#endif // VCL_TEX_COORD_H
