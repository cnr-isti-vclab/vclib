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

#include <iostream>

#include <vclib/algorithms/polygon.h>

int main()
{
	std::vector<vcl::Point2d> polygon = {
		vcl::Point2d(1, 0),
		vcl::Point2d(1, 1),
		vcl::Point2d(2, 1),
		vcl::Point2d(2, 0),
		vcl::Point2d(3, 0),
		vcl::Point2d(3, 3),
		vcl::Point2d(0, 3),
		vcl::Point2d(0, 0)};

	std::vector<uint> tri = vcl::earCut2D(polygon);

	std::cerr << "Triangle indices: \n";
	for (uint i = 0; i < tri.size(); i+= 3){
		std::cerr << tri[i] << "; " << tri[i+1] << "; " << tri[i+2] << "\n";
	}

	return 0;
}
