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

#ifndef VCLIB_EXT_QGLVIEWER_GL_AREA_H
#define VCLIB_EXT_QGLVIEWER_GL_AREA_H

#include <memory>

#include <QGLViewer/qglviewer.h>

#include <vclib/render/drawable_object_vector.h>
#include <vclib/space/box.h>

namespace vcl {

class GLArea : public QGLViewer
{
public:
	GLArea(QWidget* parent = nullptr);
	GLArea(std::shared_ptr<DrawableObjectVector> v, QWidget* parent = nullptr);

	void init();

	void setDrawableObjectVector(std::shared_ptr<DrawableObjectVector> v);
	std::shared_ptr<DrawableObjectVector> drawableObjectVector();
	std::shared_ptr<const DrawableObjectVector> drawableObjectVector() const;

	void fitScene();

protected:
	virtual void    draw();

private:
	// the viewer owns the DrawableObjects contained in this list
	std::shared_ptr<DrawableObjectVector> drawList;

	uint firstVisibleObject() const;
	vcl::Box3d fullBB() const;
};

} // namespace vcl

#include "gl_area.cpp"

#endif // VCLIB_EXT_QGLVIEWER_GL_AREA_H
