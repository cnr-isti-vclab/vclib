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

#include "tex_coord.h"
#include <assert.h>

namespace vcl {

template <typename ScalarType>
TexCoord<ScalarType>::TexCoord() : coord(0,0), n(0)
{
}

template<typename ScalarType>
ScalarType TexCoord<ScalarType>::u() const
{
	return coord.x();
}

template<typename ScalarType>
ScalarType TexCoord<ScalarType>::v() const
{
	return coord.y();
}

template<typename ScalarType>
ScalarType& TexCoord<ScalarType>::u()
{
	return coord.x();
}

template<typename ScalarType>
ScalarType& TexCoord<ScalarType>::v()
{
	return coord.y();
}

template<typename ScalarType>
void TexCoord<ScalarType>::setU(ScalarType s)
{
	assert(s >= 0 && s <= 1);
	coord.x() = s;
}

template<typename ScalarType>
void TexCoord<ScalarType>::setV(ScalarType s)
{
	assert(s >= 0 && s <= 1);
	coord.y() = s;
}

template<typename ScalarType>
void TexCoord<ScalarType>::set(ScalarType u, ScalarType v)
{
	assert(u >= 0 && u <= 1);
	assert(v >= 0 && v <= 1);
	coord.x() = u;
	coord.y() = v;
}

template<typename ScalarType>
short TexCoord<ScalarType>::nTexture() const
{
	return n;
}

template<typename ScalarType>
short& TexCoord<ScalarType>::nTexture()
{
	return n;
}

}
