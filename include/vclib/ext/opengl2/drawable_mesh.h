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

#ifndef VCLIB_EXT_OPENGL2_DRAWABLE_MESH_H
#define VCLIB_EXT_OPENGL2_DRAWABLE_MESH_H

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <vclib/render/generic_drawable_mesh.h>
#include <vclib/render/mesh_render_buffers.h>

namespace vcl {

// From: https://blog.nobel-joergensen.com/2013/01/29/debugging-opengl-using-glgeterror/
void _check_gl_error(const char *file, int line);

///
/// Usage
/// [... some opengl calls]
/// glCheckError();
///
#define check_gl_error() _check_gl_error(__FILE__,__LINE__)


template<MeshConcept MeshType>
class DrawableMesh : public GenericDrawableMesh
{
public:
	DrawableMesh();
	DrawableMesh(const MeshType& m);

	void updateBuffers(const MeshType& m);

	// DrawableObject implementation
	void draw() const;
	vcl::Point3d sceneCenter() const;
	double sceneRadius() const;
	DrawableMesh* clone() const;
private:
	void renderPass() const;

	MeshRenderBuffers<MeshType> mrb;
};

} //namespace vcl

#include "drawable_mesh.cpp"

#endif // VCLIB_EXT_OPENGL2_DRAWABLE_MESH_H
