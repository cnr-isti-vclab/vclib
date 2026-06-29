// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_OPENGL2_DRAWABLE_DRAW_OBJECTS2_H
#define VCL_OPENGL2_DRAWABLE_DRAW_OBJECTS2_H

#include <vclib/space/core/color.h>

namespace vcl {

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

} // namespace vcl

#endif // VCL_OPENGL2_DRAWABLE_DRAW_OBJECTS2_H
