/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_EXT_QT_GUI_INPUT_H
#define VCL_EXT_QT_GUI_INPUT_H

#include <vclib/gui/input.h>

#include <QMouseEvent>

namespace vcl::qt {

MouseButton::Enum fromQt(Qt::MouseButton button);

KeyModifier::Enum fromQt(Qt::KeyboardModifier modifier);

Key::Enum fromQt(Qt::Key key, Qt::KeyboardModifiers modifiers = Qt::NoModifier);

KeyModifiers fromQt(Qt::KeyboardModifiers modifiers);

} // namespace vcl::qt

#endif // VCL_EXT_QT_GUI_INPUT_H
