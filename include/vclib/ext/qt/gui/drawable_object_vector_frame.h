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

#ifndef VCLIB_EXT_QT_GUI_DRAWABLE_OBJECT_VECTOR_FRAME_H
#define VCLIB_EXT_QT_GUI_DRAWABLE_OBJECT_VECTOR_FRAME_H

#include <QFrame>

namespace vcl {

namespace Ui {
class DrawableObjectVectorFrame;
}

class DrawableObjectVectorFrame : public QFrame
{
	Q_OBJECT

public:
	explicit DrawableObjectVectorFrame(QWidget *parent = nullptr);
	~DrawableObjectVectorFrame();

private:
	Ui::DrawableObjectVectorFrame *ui;
};

} // namespace vcl

#endif // VCLIB_EXT_QT_GUI_DRAWABLE_OBJECT_VECTOR_FRAME_H
