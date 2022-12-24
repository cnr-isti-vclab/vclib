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

#ifndef VCLIB_EXT_QT_GUI_DRAWABLE_OBJECT_FRAME_H
#define VCLIB_EXT_QT_GUI_DRAWABLE_OBJECT_FRAME_H

#include <QFrame>

#include <vclib/render/drawable_object.h>

namespace vcl {

namespace Ui {
class DrawableObjectFrame;
}

class DrawableObjectFrame : public QFrame
{
	Q_OBJECT

public:
	explicit DrawableObjectFrame(DrawableObject* obj, QWidget *parent = nullptr);
	~DrawableObjectFrame();

private slots:
	void on_visibilityCheckBox_stateChanged(int arg1);

private:
	Ui::DrawableObjectFrame *ui;
	DrawableObject* obj;
};

} // namespace vcl

#endif // VCLIB_EXT_QT_GUI_DRAWABLE_OBJECT_FRAME_H
