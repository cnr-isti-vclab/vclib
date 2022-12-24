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

#ifndef VCLIB_EXT_QGLVIEWER_VIEWER_MAIN_WINDOW_H
#define VCLIB_EXT_QGLVIEWER_VIEWER_MAIN_WINDOW_H

#include <QMainWindow>

#include <vclib/render/drawable_object_vector.h>

namespace vcl {

namespace Ui {
class ViewerMainWindow;
}

class ViewerMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit ViewerMainWindow(QWidget *parent = nullptr);
	~ViewerMainWindow();

	void setDrawableObjectVector(std::shared_ptr<vcl::DrawableObjectVector> v);

public slots:
	void visibilityDrawableObjectChanged();
	void selectedDrawableObjectChanged(uint i);
	void renderSettingsUpdated();

private:
	Ui::ViewerMainWindow *ui;
	std::shared_ptr<vcl::DrawableObjectVector> drawVector;
};

} // namespace vcl

#endif // VCLIB_EXT_QGLVIEWER_VIEWER_MAIN_WINDOW_H
