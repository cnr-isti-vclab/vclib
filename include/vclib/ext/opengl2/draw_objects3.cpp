/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
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

#include "draw_objects3.h"

namespace vcl {

/**
 * @brief Draws a point on the plane (coord z = 0 if 3D).
 *
 * @param p: coordinates of the point
 * @param c: color of the point
 * @param size: size of the point (default: 8)
 */
inline void drawPoint3(const Point3d& p, const Color& c, int size)
{
	glEnable(GL_POINT_SMOOTH);
	glPointSize(size);
	glBegin(GL_POINTS);

	glColor3f(c.redF(), c.greenF(), c.blueF());

	glVertex3d(p.x(), p.y(), p.z());

	glEnd();
}

/**
 * @brief Draws a sphere with opengl
 *
 * @param center: coordinates of the center of the sphere
 * @param radius: radius of the sphere
 * @param color: color of the sphere
 * @param precision: precision of the rendered sphere (default: 4)
 */
inline void drawSphere(const Point3d& center, float radius, const Color& color, int precision)
{
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glColor3f(color.redF(), color.greenF(), color.blueF());
	glPushMatrix();
	glTranslated(center.x(), center.y(), center.z());
	GLUquadric* sphere = gluNewQuadric();
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricOrientation(sphere, GLU_OUTSIDE);
	gluSphere(sphere, radius, precision, precision);
	glPopMatrix();
}

/**
 * @brief drawCylinder
 *
 * Draws a cylinder with opengl.
 * The cylinder links the two points passed as parameters
 *
 * @param a: first point of the cylinder
 * @param b: second point of the cylinder
 * @param top_radius: radius at the "a" point
 * @param bottom_radius: radius at the "b" point
 * @param color: color of the cylinder
 */
inline void drawCylinder(
	const Point3d& a,
	const Point3d& b,
	float          top_radius,
	float          bottom_radius,
	const Color&   color,
	unsigned int   slices,
	unsigned int   stacks)
{
	Point3d dir = b - a;
	dir.normalize();
	Point3d z      = Point3d(0, 0, 1);
	Point3d normal = dir.cross(z);
	double  angle  = acos(dir.dot(z)) * 180 / M_PI;

	bool cull = glIsEnabled(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// cylinder
	glColor3f(color.redF(), color.greenF(), color.blueF());
	glPushMatrix();
	glTranslated(a.x(), a.y(), a.z());
	glRotatef(-angle, normal.x(), normal.y(), normal.z());

	GLUquadric* cylinder = gluNewQuadric();
	gluQuadricNormals(cylinder, GLU_SMOOTH);
	gluQuadricOrientation(cylinder, GLU_OUTSIDE);
	gluCylinder(cylinder, top_radius, bottom_radius, (a - b).norm(), slices, stacks);

	GLUquadric* disk1 = gluNewQuadric();
	gluQuadricNormals(disk1, GLU_SMOOTH);
	gluQuadricOrientation(disk1, GLU_INSIDE);
	gluDisk(disk1, 0, top_radius, slices, stacks);

	/*glTranslated((b-a).x(), (b-a).y(), (b-a).z());
	GLUquadric *disk2 = gluNewQuadric();
	gluQuadricNormals(disk2, GLU_SMOOTH);
	gluQuadricOrientation(disk2, GLU_OUTSIDE);
	gluDisk(disk2, 0, bottom_radius, slices, stacks);*/

	glColor3f(0, 0, 0);
	glPopMatrix();
	if (cull)
		glEnable(GL_CULL_FACE);
}

/**
 * @brief drawArrow
 * @param a
 * @param b
 * @param bottom_radius
 * @param color
 * @param slices
 * @param stacks
 */
inline void drawArrow3(
	const Point3d& a,
	const Point3d& b,
	float          bottom_radius,
	const Color&   color,
	unsigned int   slices,
	unsigned int   stacks)
{
	Point3d midPoint = (a * 1 + b * 9) / 10;
	drawCylinder(a, midPoint, bottom_radius, bottom_radius, color, slices, stacks);
	drawCylinder(midPoint, b, bottom_radius * 2, 0, color, slices, stacks);
}

/**
 * @brief drawLine
 *
 * Draws a line with opengl that links the two points passed as parameters.
 *
 * @param a: first point of the line
 * @param b: second point of the line
 * @param c: color of the line
 * @param width: width of the line (default: 3)
 */
inline void drawLine3(const Point3d& a, const Point3d& b, const Color& c, int width)
{
	glLineWidth(width);
	glColor3f(c.redF(), c.greenF(), c.blueF());
	glBegin(GL_LINES);
	glVertex3f(a.x(), a.y(), a.z());
	glVertex3f(b.x(), b.y(), b.z());
	glEnd();
	glColor3f(0, 0, 0);
}

/**
 * @brief
 * @see drawLine3
 */
inline void drawSegment3(const Point3d& a, const Point3d& b, const Color& c, int width)
{
	drawLine3(a, b, c, width);
}

/**
 * @brief drawDashedLine
 *
 * Draws a dashed line with opengl that links the two points passed as parameters.
 *
 * @param a: first point of the dashed line
 * @param b: second point of the dashed line
 * @param c: color of the dashed line
 * @param width: width of the dashed line (default: 3)
 */
inline void drawDashedLine3(const Point3d& a, const Point3d& b, const Color& c, int width)
{
	glPushAttrib(GL_ENABLE_BIT);

	glColor3f(c.redF(), c.greenF(), c.blueF());
	glLineWidth(width);
	glLineStipple(1, 0x00FF);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
	glVertex3f(a.x(), a.y(), a.z());
	glVertex3f(b.x(), b.y(), b.z());
	glEnd();

	glPopAttrib();
}

/**
 * @brief drawTriangle
 * @param p1
 * @param p2
 * @param p3
 * @param c
 * @param width
 * @param fill
 */
inline void drawTriangle(
	const Point3d& p1,
	const Point3d& p2,
	const Point3d& p3,
	const Color&   c,
	int            width,
	bool           fill)
{
	if (width != 0) {
		vcl::drawLine3(p1, p2, c, width);
		vcl::drawLine3(p2, p3, c, width);
		vcl::drawLine3(p3, p1, c, width);
	}
	if (fill) {
		glBegin(GL_TRIANGLES); // Begin triangle coordinates
		glVertex3f(p1.x(), p1.y(), p1.z());
		glVertex3f(p2.x(), p2.y(), p2.z());
		glVertex3f(p3.x(), p3.y(), p3.z());
		glEnd(); // End triangle coordinates
	}
}

/**
 * @brief drawQuad3
 * @param a
 * @param b
 * @param c
 * @param d
 * @param width
 */
inline void
drawQuad3(const Point3d& a, const Point3d& b, const Point3d& c, const Point3d& d, int width)
{
	glBegin(GL_QUADS);
	glLineWidth(width);
	glVertex3f(a.x(), a.y(), a.z());
	glVertex3f(b.x(), b.y(), b.z());
	glVertex3f(c.x(), c.y(), c.z());
	glVertex3f(d.x(), d.y(), d.z());
	glEnd();
}

/**
 * @brief drawBox3
 *
 * Draws an axis aligned 3D box using opengl lines.
 *
 * @param min
 * @param max
 * @param c
 * @param width
 */
inline void drawBox3(const Point3d& min, const Point3d& max, const Color& c, int width)
{
	drawLine3(min, Point3d(max.x(), min.y(), min.z()), c, width);
	drawLine3(Point3d(max.x(), min.y(), min.z()), Point3d(max.x(), min.y(), max.z()), c, width);
	drawLine3(Point3d(max.x(), min.y(), max.z()), Point3d(min.x(), min.y(), max.z()), c, width);
	drawLine3(min, Point3d(min.x(), min.y(), max.z()), c, width);

	drawLine3(Point3d(min.x(), max.y(), min.z()), Point3d(max.x(), max.y(), min.z()), c, width);
	drawLine3(Point3d(max.x(), max.y(), min.z()), max, c, width);
	drawLine3(max, Point3d(min.x(), max.y(), max.z()), c, width);
	drawLine3(Point3d(min.x(), max.y(), min.z()), Point3d(min.x(), max.y(), max.z()), c, width);

	drawLine3(min, Point3d(min.x(), max.y(), min.z()), c, width);
	drawLine3(Point3d(max.x(), min.y(), min.z()), Point3d(max.x(), max.y(), min.z()), c, width);
	drawLine3(Point3d(max.x(), min.y(), max.z()), max, c, width);
	drawLine3(Point3d(min.x(), min.y(), max.z()), Point3d(min.x(), max.y(), max.z()), c, width);
}

/**
 * @brief drawBox3
 *
 * draws the box composed by the first 8 point contained on the vector passed as parameter.
 *
 * @param p
 * @param c
 * @param width
 */
inline void drawBox3(const std::vector<Point3d>& p, const Color& c, int width)
{
	drawLine3(p[0], p[1], c, width);
	drawLine3(p[1], p[2], c, width);
	drawLine3(p[2], p[3], c, width);
	drawLine3(p[0], p[3], c, width);

	drawLine3(p[4], p[5], c, width);
	drawLine3(p[5], p[6], c, width);
	drawLine3(p[6], p[7], c, width);
	drawLine3(p[4], p[7], c, width);

	drawLine3(p[0], p[4], c, width);
	drawLine3(p[1], p[5], c, width);
	drawLine3(p[2], p[6], c, width);
	drawLine3(p[3], p[7], c, width);
}

/**
 * @brief drawBox3
 * @param p0
 * @param p1
 * @param p2
 * @param p3
 * @param p4
 * @param p5
 * @param p6
 * @param p7
 * @param c
 * @param width
 */
inline void drawBox3(
	const Point3d& p0,
	const Point3d& p1,
	const Point3d& p2,
	const Point3d& p3,
	const Point3d& p4,
	const Point3d& p5,
	const Point3d& p6,
	const Point3d& p7,
	const Color&   c,
	int            width)
{
	drawLine3(p0, p1, c, width);
	drawLine3(p1, p2, c, width);
	drawLine3(p2, p3, c, width);
	drawLine3(p0, p3, c, width);

	drawLine3(p4, p5, c, width);
	drawLine3(p5, p6, c, width);
	drawLine3(p6, p7, c, width);
	drawLine3(p4, p7, c, width);

	drawLine3(p0, p4, c, width);
	drawLine3(p1, p5, c, width);
	drawLine3(p2, p6, c, width);
	drawLine3(p3, p7, c, width);
}

} // namespace vcl
