/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#include <vclib/glfw/input.h>

#include <GLFW/glfw3.h>

namespace vcl::glfw {

vcl::MouseButton::Enum fromGLFW(MouseButton button)
{
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT: return vcl::MouseButton::LEFT;
    case GLFW_MOUSE_BUTTON_RIGHT: return vcl::MouseButton::RIGHT;
    case GLFW_MOUSE_BUTTON_MIDDLE: return vcl::MouseButton::MIDDLE;
    default: return vcl::MouseButton::NO_BUTTON;
    }
}

vcl::Key::Enum fromGLFW(Key key)
{
    switch (key) {
    case GLFW_KEY_SPACE: return vcl::Key::SPACE;

    case GLFW_KEY_0: return vcl::Key::_0;
    case GLFW_KEY_1: return vcl::Key::_1;
    case GLFW_KEY_2: return vcl::Key::_2;
    case GLFW_KEY_3: return vcl::Key::_3;
    case GLFW_KEY_4: return vcl::Key::_4;
    case GLFW_KEY_5: return vcl::Key::_5;
    case GLFW_KEY_6: return vcl::Key::_6;
    case GLFW_KEY_7: return vcl::Key::_7;
    case GLFW_KEY_8: return vcl::Key::_8;
    case GLFW_KEY_9: return vcl::Key::_9;

    case GLFW_KEY_A: return vcl::Key::A;
    case GLFW_KEY_B: return vcl::Key::B;
    case GLFW_KEY_C: return vcl::Key::C;
    case GLFW_KEY_D: return vcl::Key::D;
    case GLFW_KEY_E: return vcl::Key::E;
    case GLFW_KEY_F: return vcl::Key::F;
    case GLFW_KEY_G: return vcl::Key::G;
    case GLFW_KEY_H: return vcl::Key::H;
    case GLFW_KEY_I: return vcl::Key::I;
    case GLFW_KEY_J: return vcl::Key::J;
    case GLFW_KEY_K: return vcl::Key::K;
    case GLFW_KEY_L: return vcl::Key::L;
    case GLFW_KEY_M: return vcl::Key::M;
    case GLFW_KEY_N: return vcl::Key::N;
    case GLFW_KEY_O: return vcl::Key::O;
    case GLFW_KEY_P: return vcl::Key::P;
    case GLFW_KEY_Q: return vcl::Key::Q;
    case GLFW_KEY_R: return vcl::Key::R;
    case GLFW_KEY_S: return vcl::Key::S;
    case GLFW_KEY_T: return vcl::Key::T;
    case GLFW_KEY_U: return vcl::Key::U;
    case GLFW_KEY_V: return vcl::Key::V;
    case GLFW_KEY_W: return vcl::Key::W;
    case GLFW_KEY_X: return vcl::Key::X;
    case GLFW_KEY_Y: return vcl::Key::Y;
    case GLFW_KEY_Z: return vcl::Key::Z;

    case GLFW_KEY_LEFT: return vcl::Key::LEFT;
    case GLFW_KEY_RIGHT: return vcl::Key::RIGHT;
    case GLFW_KEY_UP: return vcl::Key::UP;
    case GLFW_KEY_DOWN: return vcl::Key::DOWN;

    case GLFW_KEY_LEFT_SHIFT: return vcl::Key::SHIFT;
    case GLFW_KEY_RIGHT_SHIFT: return vcl::Key::SHIFT;
#if defined(__APPLE__)
    case GLFW_KEY_LEFT_CONTROL: return vcl::Key::SUPER;
    case GLFW_KEY_RIGHT_CONTROL: return vcl::Key::SUPER;
    case GLFW_KEY_LEFT_SUPER: return vcl::Key::CONTROL;
    case GLFW_KEY_RIGHT_SUPER: return vcl::Key::CONTROL;
#else
    case GLFW_KEY_LEFT_CONTROL: return vcl::Key::CONTROL;
    case GLFW_KEY_RIGHT_CONTROL: return vcl::Key::CONTROL;
    case GLFW_KEY_LEFT_SUPER: return vcl::Key::SUPER;
    case GLFW_KEY_RIGHT_SUPER: return vcl::Key::SUPER;
#endif
    case GLFW_KEY_LEFT_ALT: return vcl::Key::ALT;
    case GLFW_KEY_RIGHT_ALT: return vcl::Key::ALT;
    case GLFW_KEY_CAPS_LOCK: return vcl::Key::CAPS_LOCK;
    case GLFW_KEY_NUM_LOCK: return vcl::Key::NUM_LOCK;
    case GLFW_KEY_SCROLL_LOCK: return vcl::Key::SCROLL_LOCK;

    case GLFW_KEY_F1: return vcl::Key::F1;
    case GLFW_KEY_F2: return vcl::Key::F2;
    case GLFW_KEY_F3: return vcl::Key::F3;
    case GLFW_KEY_F4: return vcl::Key::F4;
    case GLFW_KEY_F5: return vcl::Key::F5;
    case GLFW_KEY_F6: return vcl::Key::F6;
    case GLFW_KEY_F7: return vcl::Key::F7;
    case GLFW_KEY_F8: return vcl::Key::F8;
    case GLFW_KEY_F9: return vcl::Key::F9;
    case GLFW_KEY_F10: return vcl::Key::F10;
    case GLFW_KEY_F11: return vcl::Key::F11;
    case GLFW_KEY_F12: return vcl::Key::F12;

    case GLFW_KEY_KP_0: return vcl::Key::NP_0;
    case GLFW_KEY_KP_1: return vcl::Key::NP_1;
    case GLFW_KEY_KP_2: return vcl::Key::NP_2;
    case GLFW_KEY_KP_3: return vcl::Key::NP_3;
    case GLFW_KEY_KP_4: return vcl::Key::NP_4;
    case GLFW_KEY_KP_5: return vcl::Key::NP_5;
    case GLFW_KEY_KP_6: return vcl::Key::NP_6;
    case GLFW_KEY_KP_7: return vcl::Key::NP_7;
    case GLFW_KEY_KP_8: return vcl::Key::NP_8;
    case GLFW_KEY_KP_9: return vcl::Key::NP_9;

    default: return vcl::Key::UNKNOWN;
    }
}

KeyModifier::Enum fromGLFW(KeyboardModifier key)
{
    switch (key) {
    case GLFW_KEY_LEFT_SHIFT: return KeyModifier::SHIFT;
    case GLFW_KEY_RIGHT_SHIFT: return KeyModifier::SHIFT;
#if defined(__APPLE__)
    case GLFW_KEY_LEFT_CONTROL: return KeyModifier::SUPER;
    case GLFW_KEY_RIGHT_CONTROL: return KeyModifier::SUPER;
    case GLFW_KEY_LEFT_SUPER: return KeyModifier::CONTROL;
    case GLFW_KEY_RIGHT_SUPER: return KeyModifier::CONTROL;
#else
    case GLFW_KEY_LEFT_CONTROL: return KeyModifier::CONTROL;
    case GLFW_KEY_RIGHT_CONTROL: return KeyModifier::CONTROL;
    case GLFW_KEY_LEFT_SUPER: return KeyModifier::SUPER;
    case GLFW_KEY_RIGHT_SUPER: return KeyModifier::SUPER;
#endif
    case GLFW_KEY_LEFT_ALT: return KeyModifier::ALT;
    case GLFW_KEY_RIGHT_ALT: return KeyModifier::ALT;
    default: return KeyModifier::NO_MODIFIER;
    }
}

KeyModifiers fromGLFW(KeyboardModifiers mods)
{
    KeyModifiers res;

    if (mods & GLFW_MOD_SHIFT)
        res[KeyModifier::SHIFT] = true;
#if defined(__APPLE__)
    if (mods & GLFW_MOD_CONTROL)
        res[KeyModifier::SUPER] = true;
    if (mods & GLFW_MOD_SUPER)
        res[KeyModifier::CONTROL] = true;
#else
    if (mods & GLFW_MOD_CONTROL)
        res[KeyModifier::CONTROL] = true;
    if (mods & GLFW_MOD_SUPER)
        res[KeyModifier::SUPER] = true;
#endif
    if (mods & GLFW_MOD_ALT)
        res[KeyModifier::ALT] = true;

    // no modifier must be enabled only when no other modifier is enabled
    if (res.none())
        res[KeyModifier::NO_MODIFIER] = true;

    return res;
}

} // namespace vcl::glfw
