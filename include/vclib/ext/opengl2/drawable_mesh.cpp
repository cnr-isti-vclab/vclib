/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#include "drawable_mesh.h"
#include "draw_objects3.h"

#include <iostream>

namespace vcl {

template<MeshConcept MeshType>
DrawableMesh<MeshType>::DrawableMesh()
{
}

template<MeshConcept MeshType>
DrawableMesh<MeshType>::DrawableMesh(const MeshType &m) :
		mrb(m)
{
}

template<MeshConcept MeshType>
DrawableMesh<MeshType>::DrawableMesh(const MeshType& m, const MeshRenderSettings& mrs) :
		mrb(m), mrs(mrs)
{
}

template<MeshConcept MeshType>
const MeshRenderSettings& DrawableMesh<MeshType>::renderSettings() const
{
	return mrs;
}

template<MeshConcept MeshType>
MeshRenderSettings& DrawableMesh<MeshType>::renderSettings()
{
	return mrs;
}

template<MeshConcept MeshType>
void DrawableMesh<MeshType>::setRenderSettings(const MeshRenderSettings& rs)
{
	mrs = rs;
}

template<MeshConcept MeshType>
void DrawableMesh<MeshType>::updateBuffers(const MeshType& m)
{
	mrb = MeshRenderBuffers<MeshType>(m);
}

template<MeshConcept MeshType>
void DrawableMesh<MeshType>::draw() const
{
	draw(
		mrb.vertexNumber(),
		mrb.triangleNumber(),
		mrb.vertexBufferData(),
		mrb.triangleBufferData(),
		mrb.vertexNormalBufferData(),
		mrb.vertexColorBufferData(),
		mrb.triangleNormalBufferData(),
		mrb.triangleColorBufferData(),
		mrb.bbMin(),
		mrb.bbMax());
}

template<MeshConcept MeshType>
Point3d DrawableMesh<MeshType>::sceneCenter() const
{
	return (mrb.bbMin() + mrb.bbMax()) / 2;
}

template<MeshConcept MeshType>
double DrawableMesh<MeshType>::sceneRadius() const
{
	return (mrb.bbMax() - mrb.bbMin()).norm() / 2;
}

template<MeshConcept MeshType>
DrawableMesh<MeshType>* DrawableMesh<MeshType>::clone() const
{
	return new DrawableMesh(*this);
}

template<MeshConcept MeshType>
void DrawableMesh<MeshType>::draw(
	uint           nv,
	uint           nt,
	const float*   pCoords,
	const int*     pTriangles,
	const float*   pVertexNormals,
	const float*   pVertexColors,
	const float*   pTriangleNormals,
	const float*   pTriangleColors,
	const Point3d& min,
	const Point3d& max) const
{
	if (mrs.isVisible()) {
		if (mrs.isWireframeVisible()) {
			if (mrs.isPointCloudVisible()) {
				glDisable(GL_LIGHTING);
				glShadeModel(GL_FLAT);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDepthRange(0.0, 1.0);
				renderPass(
					nv,
					nt,
					pCoords,
					pTriangles,
					pVertexNormals,
					pVertexColors,
					pTriangleNormals,
					pTriangleColors);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			if (mrs.isSurfaceShadingFlat()) {
				glEnable(GL_LIGHTING);
				glShadeModel(GL_FLAT);
				glDepthRange(0.01, 1.0);
				renderPass(
					nv,
					nt,
					pCoords,
					pTriangles,
					pVertexNormals,
					pVertexColors,
					pTriangleNormals,
					pTriangleColors);

				glDisable(GL_LIGHTING);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDepthRange(0.0, 1.0);
				glDepthFunc(GL_LEQUAL);
				renderPass(
					nv,
					nt,
					pCoords,
					pTriangles,
					pVertexNormals,
					pVertexColors,
					pTriangleNormals,
					pTriangleColors);
				glDepthFunc(GL_LESS);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else if (mrs.isSurfaceShadingSmooth()) {
				glEnable(GL_LIGHTING);
				glShadeModel(GL_SMOOTH);
				glDepthRange(0.01, 1.0);
				renderPass(
					nv,
					nt,
					pCoords,
					pTriangles,
					pVertexNormals,
					pVertexColors,
					pTriangleNormals,
					pTriangleColors);

				glDisable(GL_LIGHTING);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDepthRange(0.0, 1.0);
				glDepthFunc(GL_LEQUAL);
				renderPass(
					nv,
					nt,
					pCoords,
					pTriangles,
					pVertexNormals,
					pVertexColors,
					pTriangleNormals,
					pTriangleColors);
				glDepthFunc(GL_LESS);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}
		else { // no wireframe
			if (mrs.isPointCloudVisible()) {
				glDisable(GL_LIGHTING);
				renderPass(
					nv,
					nt,
					pCoords,
					pTriangles,
					pVertexNormals,
					pVertexColors,
					pTriangleNormals,
					pTriangleColors);
			}
			if (mrs.isSurfaceShadingFlat()) {
				glEnable(GL_LIGHTING);
				glShadeModel(GL_FLAT);
				renderPass(
					nv,
					nt,
					pCoords,
					pTriangles,
					pVertexNormals,
					pVertexColors,
					pTriangleNormals,
					pTriangleColors);
			}
			else if (mrs.isSurfaceShadingSmooth()) {
				glEnable(GL_LIGHTING);
				glShadeModel(GL_SMOOTH);
				renderPass(
					nv,
					nt,
					pCoords,
					pTriangles,
					pVertexNormals,
					pVertexColors,
					pTriangleNormals,
					pTriangleColors);
			}
		}
		if (mrs.isBboxEnabled()) {
			vcl::drawBox3(min, max, vcl::Color(0, 0, 0));
		}
	}
}

template<MeshConcept MeshType>
void DrawableMesh<MeshType>::renderPass(
	uint         nv,
	uint         nt,
	const float* coords,
	const int*   triangles,
	const float* vertexNormals,
	const float* vertexColors,
	const float* triangleNormals,
	const float* triangleColors) const
{
	if (nv > 0 && coords) {
		if (mrs.isPointCloudVisible()) {
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, coords);

			if (vertexColors) {
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(3, GL_FLOAT, 0, vertexColors);
			}

			glPointSize(mrs.pointWidth());

			glDrawArrays(GL_POINTS, 0, nv);

			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
		}

		if (nt > 0 && triangles) {
			if (mrs.isSurfaceVisible()) {
				// Old fashioned, verbose and slow rendering.
				if (mrs.isSurfaceColorPerFace()) {
					int n_tris = nt;
					for (int tid = 0; tid < n_tris; ++tid) {
						int tid_ptr  = 3 * tid;
						int vid0     = triangles[tid_ptr + 0];
						int vid1     = triangles[tid_ptr + 1];
						int vid2     = triangles[tid_ptr + 2];
						int vid0_ptr = 3 * vid0;
						int vid1_ptr = 3 * vid1;
						int vid2_ptr = 3 * vid2;

						if (mrs.isSurfaceShadingSmooth()) {
							glBegin(GL_TRIANGLES);
							if (triangleColors)
								glColor3fv(&(triangleColors[tid_ptr]));
							if (vertexNormals)
								glNormal3fv(&(vertexNormals[vid0_ptr]));
							glVertex3fv(&(coords[vid0_ptr]));
							if (vertexNormals)
								glNormal3fv(&(vertexNormals[vid1_ptr]));
							glVertex3fv(&(coords[vid1_ptr]));
							if (vertexNormals)
								glNormal3fv(&(vertexNormals[vid2_ptr]));
							glVertex3fv(&(coords[vid2_ptr]));
							glEnd();
						}
						else {
							glBegin(GL_TRIANGLES);
							if (triangleColors)
								glColor3fv(&(triangleColors[tid_ptr]));
							if (triangleNormals)
								glNormal3fv(&(triangleNormals[tid_ptr]));
							glVertex3fv(&(coords[vid0_ptr]));
							if (triangleNormals)
								glNormal3fv(&(triangleNormals[tid_ptr]));
							glVertex3fv(&(coords[vid1_ptr]));
							if (triangleNormals)
								glNormal3fv(&(triangleNormals[tid_ptr]));
							glVertex3fv(&(coords[vid2_ptr]));
							glEnd();
						}
					}
				}
				else if (mrs.isSurfaceColorPerVertex()) {
					glEnableClientState(GL_VERTEX_ARRAY);
					glVertexPointer(3, GL_FLOAT, 0, coords);

					if (vertexNormals) {
						glEnableClientState(GL_NORMAL_ARRAY);
						glNormalPointer(GL_FLOAT, 0, vertexNormals);
					}

					if (vertexColors) {
						glEnableClientState(GL_COLOR_ARRAY);
						glColorPointer(3, GL_FLOAT, 0, vertexColors);
					}

					glDrawElements(GL_TRIANGLES, nt * 3, GL_UNSIGNED_INT, triangles);

					glDisableClientState(GL_COLOR_ARRAY);
					glDisableClientState(GL_NORMAL_ARRAY);
					glDisableClientState(GL_VERTEX_ARRAY);
				}
			}

			if (mrs.isWireframeVisible()) {
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(3, GL_FLOAT, 0, coords);

				glLineWidth(mrs.wireframeWidth());
				glColor4fv(mrs.wireframeColorData());

				glDrawElements(GL_TRIANGLES, nt * 3, GL_UNSIGNED_INT, triangles);

				glDisableClientState(GL_VERTEX_ARRAY);
			}
		}
	}
}

inline void _check_gl_error(const char* file, int line)
{
	GLenum err(glGetError());

	while (err != GL_NO_ERROR) {
		std::string error;

		switch (err) {
		case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
		case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
		case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY"; break;
#ifdef unix
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
#endif
		}

		std::cerr << "GL_" << error.c_str() << " - " << file << ":" << line << std::endl;
		err = glGetError();
	}
}

} // namespace vcl
