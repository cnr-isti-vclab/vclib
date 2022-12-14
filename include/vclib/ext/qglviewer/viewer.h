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

#ifndef VCLIB_EXT_QGLVIEWER_VIEWER_H
#define VCLIB_EXT_QGLVIEWER_VIEWER_H

#include <QGLViewer/qglviewer.h>

#include <vclib/render/drawable_object.h>
#include <vclib/space/box.h>

namespace vcl {

class Viewer : public QGLViewer
{
public:
	virtual ~Viewer();

	uint pushDrawableObject(const DrawableObject& obj);
	uint pushDrawableObject(const DrawableObject* obj);

	void fitScene();

protected:
	virtual void    draw();
	virtual void    init();
	virtual QString helpString() const;

private:
	std::vector<DrawableObject*> drawList;

	vcl::Box3d fullBB() const;
};

} // namespace vcl

#include "viewer.cpp"

#endif // VCLIB_EXT_QGLVIEWER_VIEWER_H
