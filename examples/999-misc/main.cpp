/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
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
#include <vector>

#include <vclib/views.h>

struct Point
{
    float x, y, z;
};

class Vertex
{
    Point p;
public:
    Vertex(float x, float y, float z) : p{x, y, z} {}

    Point& coord() { return p; }
    const Point& coord() const { return p; }
};

int main()
{
    std::vector<Vertex> v;

    v.push_back(Vertex(-0.5, -0.5, 0.5));
    v.push_back(Vertex(0.5, -0.5, 0.5));
    v.push_back(Vertex(-0.5, 0.5, 0.5));
    v.push_back(Vertex(0.5, 0.5, 0.5));

    for (auto& p : v | vcl::views::coords) {
        p.x += 1;
    }

    for (const auto& p : v | vcl::views::coords) {
        std::cout << p.x << " " << p.y << " " << p.z << std::endl;
    }
}
