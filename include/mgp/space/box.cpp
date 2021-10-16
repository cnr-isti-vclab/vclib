/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "box.h"

namespace mgp {

template<class Scalar, int N>
Box<Scalar, N>::Box()
{
	setNull();
}

template<class Scalar, int N>
void Box<Scalar, N>::setNull()
{
	min.setConstant(1);
	max.setConstant(-1);
}

} // namespace mgp
