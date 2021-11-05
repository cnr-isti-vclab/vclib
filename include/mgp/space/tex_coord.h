/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_TEX_COORD_H
#define MGP_TEX_COORD_H

#include "point/point2.h"

namespace mgp {

template <typename ScalarType>
class TexCoord
{
public:
	TexCoord();

	ScalarType u() const;
	ScalarType v() const;
	void u(ScalarType s);
	void v(ScalarType s);

private:
	Point2<ScalarType> coord;
	short n;
};

}

#include "tex_coord.cpp"

#endif // MGP_TEX_COORD_H
