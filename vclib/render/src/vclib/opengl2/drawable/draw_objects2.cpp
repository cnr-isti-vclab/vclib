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

#include <vclib/opengl2/drawable/draw_objects2.h>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

namespace vcl {

/**
 * @brief drawPoint2
 *
 * Draws a point on the plane (coord z = 0 if 3D).
 *
 * @param p: coordinates of the point
 * @param c: color of the point
 * @param size: size of the point (default: 8)
 */
void drawPoint2(const Point2d& p, const Color& c, int size)
{
    glEnable(GL_POINT_SMOOTH);
    glPointSize(size);
    glBegin(GL_POINTS);

    glColor3f(c.redF(), c.greenF(), c.blueF());

    glVertex2d(p.x(), p.y());

    glEnd();
}

/**
 * @brief drawLine2
 *
 * Draws a line with opengl on the plane (z=0 if 3D).
 * It links the two points passed as parameters.
 *
 * @param a: first point of the line
 * @param b: second point of the line
 * @param c: color of the line
 * @param width: width of the line (default: 3)
 */
void drawLine2(const Point2d& a, const Point2d& b, const Color& c, int width)
{
    glLineWidth(width);

    glBegin(GL_LINES);

    glColor3f(c.redF(), c.greenF(), c.blueF());

    glVertex2d(a.x(), a.y());
    glVertex2d(b.x(), b.y());

    glEnd();
}

void drawTriangle2(
    const Point2d& p1,
    const Point2d& p2,
    const Point2d& p3,
    const Color&   c,
    int            width,
    bool           fill)
{
    if (width != 0) {
        drawLine2(p1, p2, c, width);
        drawLine2(p2, p3, c, width);
        drawLine2(p3, p1, c, width);
    }
    if (fill) {
        glBegin(GL_TRIANGLES); // Begin triangle coordinates
        glVertex2f(p1.x(), p1.y());
        glVertex2f(p2.x(), p2.y());
        glVertex2f(p3.x(), p3.y());
        glEnd(); // End triangle coordinates
    }
}

void drawTriangle2(
    const std::array<Point2d, 3>& arr,
    const Color&                  c,
    int                           width,
    bool                          fill)
{
    drawTriangle2(arr[0], arr[1], arr[2], c, width, fill);
}

void drawQuad2(
    const std::array<Point2d, 4>& points,
    const Color&                  c,
    int                           width,
    bool                          fill)
{
    for (unsigned int i = 0; i < 4; i++) {
        drawLine2(points[i], points[(i + 1) % 4], c, width);
    }
    if (fill) {
        // find angle >=90°
        int pivot = -1;
        for (unsigned int i = 0; i < 4; i++) {
            double angle = (points[(i - 1) % 4] - points[i])
                               .dot(points[(i + 1) % 4] - points[i]);
            if (angle <= 0)
                pivot = i;
        }
        assert(pivot >= 0);
        drawTriangle2(
            points[pivot],
            points[(pivot + 1) % 4],
            points[(pivot + 2) % 4],
            c,
            0,
            true);
        drawTriangle2(
            points[(pivot + 2) % 4],
            points[(pivot + 3) % 4],
            points[pivot],
            c,
            0,
            true);
    }
}

void drawQuad2(
    const Point2d& p1,
    const Point2d& p2,
    const Point2d& p3,
    const Point2d& p4,
    const Color&   c,
    int            width,
    bool           fill)
{
    std::array<Point2d, 4> arr = {p1, p2, p3, p4};
    drawQuad2(arr, c, width, fill);
}

} // namespace vcl
