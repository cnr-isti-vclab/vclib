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

#ifndef VCL_EXT_QT_GUI_DRAWABLE_OBJECT_VECTOR_FRAME_H
#define VCL_EXT_QT_GUI_DRAWABLE_OBJECT_VECTOR_FRAME_H

#include <QFrame>

#include <vclib/render/drawable_object_vector.h>

#include "drawable_object_frame.h"

namespace vcl {

namespace Ui {
class DrawableObjectVectorFrame;
}

class DrawableObjectVectorFrame : public QFrame
{
	Q_OBJECT

public:
	explicit DrawableObjectVectorFrame(QWidget *parent = nullptr);
	explicit DrawableObjectVectorFrame(std::shared_ptr<DrawableObjectVector> v, QWidget *parent = nullptr);
	~DrawableObjectVectorFrame();

	void setDrawableObjectVector(std::shared_ptr<vcl::DrawableObjectVector> v);

	uint selectedDrawableObject() const;

signals:
	void drawableObjectVisibilityChanged();
	void drawableObjectSelectionChanged(uint i);

private slots:
	void on_listWidget_itemSelectionChanged();

private:
	// this frame does not normally own this drawList
	std::shared_ptr<DrawableObjectVector> drawList;
	Ui::DrawableObjectVectorFrame *ui;

	void updateDrawableVectorWidget();
};

} // namespace vcl

#endif // VCL_EXT_QT_GUI_DRAWABLE_OBJECT_VECTOR_FRAME_H
