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

vcl::MouseButton::Enum fromQt(Qt::MouseButton button)
{
    switch (button) {
    case Qt::LeftButton: return MouseButton::LEFT;
    case Qt::RightButton: return MouseButton::RIGHT;
    case Qt::MiddleButton: return MouseButton::MIDDLE;
    default: return MouseButton::NO_BUTTON;
    }
}

KeyModifier::Enum fromQt(Qt::KeyboardModifier modifier)
{
    switch (modifier) {
    case Qt::ShiftModifier: return KeyModifier::SHIFT;
    case Qt::ControlModifier: return KeyModifier::CONTROL;
    case Qt::AltModifier: return KeyModifier::ALT;
    default: return KeyModifier::NO_MODIFIER;
    }
}

Key::Enum fromQt(Qt::Key key, Qt::KeyboardModifiers modifiers)
{
    if (modifiers & Qt::KeypadModifier) {
        if (key >= Qt::Key_0 && key <= Qt::Key_9) {
            uint diff = key - Qt::Key_0;
            return static_cast<Key::Enum>(Key::NP_0 + diff);
        }
        // if shift is pressed, Qt changes the numpad keys to the corresponding
        // arrow keys
        if (modifiers & Qt::ShiftModifier) {
            if (key == Qt::Key_Left)
                return Key::NP_4;
            if (key == Qt::Key_Right)
                return Key::NP_6;
            if (key == Qt::Key_Up)
                return Key::NP_8;
            if (key == Qt::Key_Down)
                return Key::NP_2;
            if (key == Qt::Key_PageUp)
                return Key::NP_9;
            if (key == Qt::Key_PageDown)
                return Key::NP_3;
            if (key == Qt::Key_Home)
                return Key::NP_7;
            if (key == Qt::Key_End)
                return Key::NP_1;
        }
    }
    return static_cast<Key::Enum>(key);
}

KeyModifiers fromQt(Qt::KeyboardModifiers modifiers)
{
    KeyModifiers res;

    if (modifiers & Qt::ShiftModifier)
        res[KeyModifier::SHIFT] = true;
    if (modifiers & Qt::ControlModifier)
        res[KeyModifier::CONTROL] = true;
    if (modifiers & Qt::AltModifier)
        res[KeyModifier::ALT] = true;

    // no modifier must be enabled only when no other modifier is enabled
    if (res.none())
        res[KeyModifier::NO_MODIFIER] = true;

    return res;
}

} // namespace vcl::qt
