/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#include <vclib/algorithms.h>

#include <iostream>

int main()
{
    /*
     *  3 6------------------5
     *    |                  |
     *  2 |                  |
     *    |                  |
     *  1 |  1--+--2         |
     *    |  |     |         |
     *  0 7--0     3--+--+---4
     *    0  1     2  3
     */

    vcl::Polygon2d polygon = {
        vcl::Point2d(1, 0),
        vcl::Point2d(1, 1),
        vcl::Point2d(2, 1),
        vcl::Point2d(2, 0),
        vcl::Point2d(3, 0),
        vcl::Point2d(3, 3),
        vcl::Point2d(0, 3),
        vcl::Point2d(0, 0)};

    std::vector<unsigned int> tri = vcl::earCut(polygon.begin(), polygon.end());

    std::cerr << "Triangle indices: \n";
    for (unsigned int i = 0; i < tri.size(); i += 3) {
        std::cerr << tri[i] << "; " << tri[i + 1] << "; " << tri[i + 2] << "\n";
    }

    return 0;
}
