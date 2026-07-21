// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_OPENGL2_DRAWABLE_DRAW_OBJECTS3_H
#define VCL_OPENGL2_DRAWABLE_DRAW_OBJECTS3_H

#include <vclib/space/core/color.h>

namespace vcl {

void drawPoint3(const Point3d& p, const Color& c, int size = 8);

void drawSphere(
    const Point3d& center,
    float          radius,
    const Color&   color,
    int            precision = 4);

void drawCylinder(
    const Point3d& a,
    const Point3d& b,
    float          top_radius,
    float          bottom_radius,
    const Color&   color,
    unsigned int   slices = 50,
    unsigned int   stacks = 10);

void drawArrow3(
    const Point3d& a,
    const Point3d& b,
    float          bottom_radius,
    const Color&   color,
    unsigned int   slices = 50,
    unsigned int   stacks = 10);

void drawLine3(
    const Point3d& a,
    const Point3d& b,
    const Color&   c,
    int            width = 3);

void drawSegment3(
    const Point3d& a,
    const Point3d& b,
    const Color&   c,
    int            width = 3);

void drawDashedLine3(
    const Point3d& a,
    const Point3d& b,
    const Color&   c,
    int            width = 3);

void drawTriangle(
    const Point3d& p1,
    const Point3d& p2,
    const Point3d& p3,
    const Color&   c,
    int            width = 3,
    bool           fill  = false);

void drawQuad3(
    const Point3d& a,
    const Point3d& b,
    const Point3d& c,
    const Point3d& d,
    int            width = 3);

void drawBox3(
    const Point3d& min,
    const Point3d& max,
    const Color&   c,
    int            width = 3);

void drawBox3(
    const Point3d& p0,
    const Point3d& p1,
    const Point3d& p2,
    const Point3d& p3,
    const Point3d& p4,
    const Point3d& p5,
    const Point3d& p6,
    const Point3d& p7,
    const Color&   c,
    int            width = 3);

void drawBox3(const std::vector<Point3d>& p, const Color& c, int width = 3);

} // namespace vcl

#endif // VCL_OPENGL2_DRAWABLE_DRAW_OBJECTS3_H
