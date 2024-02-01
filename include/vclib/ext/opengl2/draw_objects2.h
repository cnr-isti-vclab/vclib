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

#ifndef VCL_EXT_OPENGL2_DRAW_OBJECTS2_H
#define VCL_EXT_OPENGL2_DRAW_OBJECTS2_H

#include <vclib/space/color.h>

namespace vcl::gl2 {

void drawPoint2(const Point2d& p, const Color& c, int size = 8);

void drawLine2(
    const Point2d& a,
    const Point2d& b,
    const Color&   c,
    int            width = 3);

void drawTriangle2(
    const Point2d& p1,
    const Point2d& p2,
    const Point2d& p3,
    const Color&   c,
    int            width = 3,
    bool           fill  = false);

void drawTriangle2(
    const std::array<Point2d, 3>& arr,
    const Color&                  c,
    int                           width = 3,
    bool                          fill  = false);

void drawQuad2(
    const std::array<Point2d, 4>& points,
    const Color&                  c,
    int                           width = 3,
    bool                          fill  = false);

void drawQuad2(
    const Point2d& p1,
    const Point2d& p2,
    const Point2d& p3,
    const Point2d& p4,
    const Color&   c,
    int            width = 3,
    bool           fill  = false);

} // namespace vcl::gl2

#endif // VCL_EXT_OPENGL2_DRAW_OBJECTS2_H
