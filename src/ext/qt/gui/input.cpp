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

#include <vclib/ext/qt/gui/input.h>

namespace vcl::qt {

vcl::MouseButton fromQt(Qt::MouseButton button)
{
    switch (button) {
    case Qt::LeftButton: return LEFT;
    case Qt::RightButton: return RIGHT;
    case Qt::MiddleButton: return MIDDLE;
    default: return NO_BUTTON;
    }
}

KeyModifier fromQt(Qt::KeyboardModifier modifier)
{
    switch (modifier) {
    case Qt::ShiftModifier: return SHIFT;
    case Qt::ControlModifier: return CONTROL;
    case Qt::AltModifier: return ALT;
    default: return NO_MODIFIER;
    }
}

Key fromQt(Qt::Key key)
{
    return static_cast<Key>(key);
}

KeyModifiers fromQt(Qt::KeyboardModifiers modifiers)
{
    KeyModifiers res;

    if (modifiers & Qt::ShiftModifier)
        res[SHIFT] = true;
    if (modifiers & Qt::ControlModifier)
        res[CONTROL] = true;
    if (modifiers & Qt::AltModifier)
        res[ALT] = true;

           // no modifier must be enabled only when no other modifier is enabled
    if (res.none())
        res[NO_MODIFIER] = true;

    return res;
}

} // namespace vcl::qt
