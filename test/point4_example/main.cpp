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

#include <iostream>

#include <vclib/space/point.h>

int main()
{
	vcl::Point4d p1(1, 0, 0, 0);

	vcl::Point4d p2(0, 1, 0, 0);

	std::cout << "Point p1 is [" << p1 << "].\n";
	std::cout << "Point p2 is [" << p2 << "].\n";

	std::cout << "p1 norm is " << p1.norm() << ".\n";
	std::cout << "p1 squared norm is " << p1.squaredNorm() << "\n";

	std::cout << "p1 dot p2: " << p1.dot(p2) << "\n";

	vcl::Point4f pf = p1;

	std::cout << "p1 converted to float: [" << pf << "]\n";

	return 0;
}
