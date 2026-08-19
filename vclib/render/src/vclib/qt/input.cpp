// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/input.h>

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
    switch(key) {
    case Qt::Key_Space : return Key::SPACE;

    case Qt::Key_0 : return Key::_0;
    case Qt::Key_1 : return Key::_1;
    case Qt::Key_2 : return Key::_2;
    case Qt::Key_3 : return Key::_3;
    case Qt::Key_4 : return Key::_4;
    case Qt::Key_5 : return Key::_5;
    case Qt::Key_6 : return Key::_6;
    case Qt::Key_7 : return Key::_7;
    case Qt::Key_8 : return Key::_8;
    case Qt::Key_9 : return Key::_9;

    case Qt::Key_A : return Key::A;
    case Qt::Key_B : return Key::B;
    case Qt::Key_C : return Key::C;
    case Qt::Key_D : return Key::D;
    case Qt::Key_E : return Key::E;
    case Qt::Key_F : return Key::F;
    case Qt::Key_G : return Key::G;
    case Qt::Key_H : return Key::H;
    case Qt::Key_I : return Key::I;
    case Qt::Key_J : return Key::J;
    case Qt::Key_K : return Key::K;
    case Qt::Key_L : return Key::L;
    case Qt::Key_M : return Key::M;
    case Qt::Key_N : return Key::N;
    case Qt::Key_O : return Key::O;
    case Qt::Key_P : return Key::P;
    case Qt::Key_Q : return Key::Q;
    case Qt::Key_R : return Key::R;
    case Qt::Key_S : return Key::S;
    case Qt::Key_T : return Key::T;
    case Qt::Key_U : return Key::U;
    case Qt::Key_V : return Key::V;
    case Qt::Key_W : return Key::W;
    case Qt::Key_X : return Key::X;
    case Qt::Key_Y : return Key::Y;
    case Qt::Key_Z : return Key::Z;

    case Qt::Key_Escape : return Key::ESCAPE;
    case Qt::Key_Left : return Key::LEFT;
    case Qt::Key_Right : return Key::RIGHT;
    case Qt::Key_Up : return Key::UP;
    case Qt::Key_Down : return Key::DOWN;

    case Qt::Key_Shift : return Key::SHIFT_KEY;
    case Qt::Key_Control : return Key::CONTROL_KEY;
    case Qt::Key_Meta : return Key::SUPER_KEY;
    case Qt::Key_Alt : return Key::ALT_KEY;

    case Qt::Key_CapsLock : return Key::CAPS_LOCK;
    case Qt::Key_NumLock : return Key::NUM_LOCK;
    case Qt::Key_ScrollLock : return Key::SCROLL_LOCK;

    case Qt::Key_F1 : return Key::F1;
    case Qt::Key_F2 : return Key::F2;
    case Qt::Key_F3 : return Key::F3;
    case Qt::Key_F4 : return Key::F4;
    case Qt::Key_F5 : return Key::F5;
    case Qt::Key_F6 : return Key::F6;
    case Qt::Key_F7 : return Key::F7;
    case Qt::Key_F8 : return Key::F8;
    case Qt::Key_F9 : return Key::F9;
    case Qt::Key_F10 : return Key::F10;
    case Qt::Key_F11 : return Key::F11;
    case Qt::Key_F12 : return Key::F12;

    case Qt::Key_Launch0 : return Key::NP_0;
    case Qt::Key_Launch1 : return Key::NP_1;
    case Qt::Key_Launch2 : return Key::NP_2;
    case Qt::Key_Launch3 : return Key::NP_3;
    case Qt::Key_Launch4 : return Key::NP_4;
    case Qt::Key_Launch5 : return Key::NP_5;
    case Qt::Key_Launch6 : return Key::NP_6;
    case Qt::Key_Launch7 : return Key::NP_7;
    case Qt::Key_Launch8 : return Key::NP_8;
    case Qt::Key_Launch9 : return Key::NP_9;

    default:
        return Key::UNKNOWN;
    }
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
