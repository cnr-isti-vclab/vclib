/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "tex_coord.h"
#include <assert.h>

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
