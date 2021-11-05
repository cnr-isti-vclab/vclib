/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "tex_coord.h"

namespace mgp {

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
void TexCoord<ScalarType>::u(ScalarType s)
{
	assert(s >= 0 && s <= 1);
	coord.x() = s;
}

template<typename ScalarType>
void TexCoord<ScalarType>::v(ScalarType s)
{
	assert(s >= 0 && s <= 1);
	coord.y() = s;
}

}
