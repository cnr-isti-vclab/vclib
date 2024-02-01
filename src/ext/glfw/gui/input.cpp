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

#include <vclib/ext/glfw/gui/input.h>

#include <GLFW/glfw3.h>

namespace vcl::glfw {

vcl::MouseButton fromGLFW(MouseButton button)
{
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT: return LEFT;
    case GLFW_MOUSE_BUTTON_RIGHT: return RIGHT;
    case GLFW_MOUSE_BUTTON_MIDDLE: return MIDDLE;
    default: return NO_BUTTON;
    }
}

vcl::Key fromGLFW(Key key)
{
    switch (key) {
    case GLFW_KEY_SPACE: return KEY_SPACE;

    case GLFW_KEY_0: return KEY_0;
    case GLFW_KEY_1: return KEY_1;
    case GLFW_KEY_2: return KEY_2;
    case GLFW_KEY_3: return KEY_3;
    case GLFW_KEY_4: return KEY_4;
    case GLFW_KEY_5: return KEY_5;
    case GLFW_KEY_6: return KEY_6;
    case GLFW_KEY_7: return KEY_7;
    case GLFW_KEY_8: return KEY_8;
    case GLFW_KEY_9: return KEY_9;

    case GLFW_KEY_A: return KEY_A;
    case GLFW_KEY_B: return KEY_B;
    case GLFW_KEY_C: return KEY_C;
    case GLFW_KEY_D: return KEY_D;
    case GLFW_KEY_E: return KEY_E;
    case GLFW_KEY_F: return KEY_F;
    case GLFW_KEY_G: return KEY_G;
    case GLFW_KEY_H: return KEY_H;
    case GLFW_KEY_I: return KEY_I;
    case GLFW_KEY_J: return KEY_J;
    case GLFW_KEY_K: return KEY_K;
    case GLFW_KEY_L: return KEY_L;
    case GLFW_KEY_M: return KEY_M;
    case GLFW_KEY_N: return KEY_N;
    case GLFW_KEY_O: return KEY_O;
    case GLFW_KEY_P: return KEY_P;
    case GLFW_KEY_Q: return KEY_Q;
    case GLFW_KEY_R: return KEY_R;
    case GLFW_KEY_S: return KEY_S;
    case GLFW_KEY_T: return KEY_T;
    case GLFW_KEY_U: return KEY_U;
    case GLFW_KEY_V: return KEY_V;
    case GLFW_KEY_W: return KEY_W;
    case GLFW_KEY_X: return KEY_X;
    case GLFW_KEY_Y: return KEY_Y;
    case GLFW_KEY_Z: return KEY_Z;

    case GLFW_KEY_LEFT: return KEY_LEFT;
    case GLFW_KEY_RIGHT: return KEY_RIGHT;
    case GLFW_KEY_UP: return KEY_UP;
    case GLFW_KEY_DOWN: return KEY_DOWN;

    case GLFW_KEY_F1: return KEY_F1;
    case GLFW_KEY_F2: return KEY_F2;
    case GLFW_KEY_F3: return KEY_F3;
    case GLFW_KEY_F4: return KEY_F4;
    case GLFW_KEY_F5: return KEY_F5;
    case GLFW_KEY_F6: return KEY_F6;
    case GLFW_KEY_F7: return KEY_F7;
    case GLFW_KEY_F8: return KEY_F8;
    case GLFW_KEY_F9: return KEY_F9;
    case GLFW_KEY_F10: return KEY_F10;
    case GLFW_KEY_F11: return KEY_F11;
    case GLFW_KEY_F12: return KEY_F12;

    default: return KEY_UNKNOWN;
    }
}

KeyModifier fromGLFW(KeyboardModifier key)
{
    switch (key) {
    case GLFW_KEY_LEFT_SHIFT: return SHIFT;
    case GLFW_KEY_RIGHT_SHIFT: return SHIFT;
    case GLFW_KEY_LEFT_CONTROL: return CONTROL;
    case GLFW_KEY_RIGHT_CONTROL: return CONTROL;
    case GLFW_KEY_LEFT_ALT: return ALT;
    case GLFW_KEY_RIGHT_ALT: return ALT;
    default: return NO_MODIFIER;
    }
}

KeyModifiers fromGLFW(KeyboardModifiers mods)
{
    KeyModifiers res;

    if (mods & GLFW_MOD_SHIFT)
        res[SHIFT] = true;
    if (mods & GLFW_MOD_CONTROL)
        res[CONTROL] = true;
    if (mods & GLFW_MOD_ALT)
        res[ALT] = true;

           // no modifier must be enabled only when no other modifier is enabled
    if (res.none())
        res[NO_MODIFIER] = true;

    return res;
}

} // namespace vcl::glfw
