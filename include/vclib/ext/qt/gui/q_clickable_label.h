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

#ifndef VCL_EXT_QT_GUI_Q_CLICKABLE_LABEL_H
#define VCL_EXT_QT_GUI_Q_CLICKABLE_LABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>

namespace vcl {

/**
 * @brief The QClickableLabel class
 *
 * A QLabel class which can be clicked (clicked event).
 *
 * Usage: just Promote a QLabel to a vcl::QClickableLabel
 *
 * @link https://wiki.qt.io/Clickable_QLabel
 */
class QClickableLabel : public QLabel
{
	Q_OBJECT
public:
	explicit QClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
	~QClickableLabel();

signals:
	void clicked();

protected:
	void mousePressEvent(QMouseEvent* event);
};

} // namespace vcl

#endif // VCL_EXT_QT_GUI_Q_CLICKABLE_LABEL_H
