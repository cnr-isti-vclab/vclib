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

#ifndef VCL_GUI_INPUT_H
#define VCL_GUI_INPUT_H

#include <vclib/space/bit_set.h>

namespace vcl {

enum MouseButton {
    LEFT      = 0,
    RIGHT     = 1,
    MIDDLE    = 2,
    NO_BUTTON = 3,
};

enum KeyModifier {
    SHIFT       = 0,
    CONTROL     = 1,
    ALT         = 2,
    NO_MODIFIER = 3,
};

using KeyModifiers = BitSet<char>;

enum Key {
    KEY_SPACE = 0x20,

    KEY_0 = 0x30,
    KEY_1 = 0x31,
    KEY_2 = 0x32,
    KEY_3 = 0x33,
    KEY_4 = 0x34,
    KEY_5 = 0x35,
    KEY_6 = 0x36,
    KEY_7 = 0x37,
    KEY_8 = 0x38,
    KEY_9 = 0x39,

    KEY_A = 0x41,
    KEY_B = 0x42,
    KEY_C = 0x43,
    KEY_D = 0x44,
    KEY_E = 0x45,
    KEY_F = 0x46,
    KEY_G = 0x47,
    KEY_H = 0x48,
    KEY_I = 0x49,
    KEY_J = 0x4a,
    KEY_K = 0x4b,
    KEY_L = 0x4c,
    KEY_M = 0x4d,
    KEY_N = 0x4e,
    KEY_O = 0x4f,
    KEY_P = 0x50,
    KEY_Q = 0x51,
    KEY_R = 0x52,
    KEY_S = 0x53,
    KEY_T = 0x54,
    KEY_U = 0x55,
    KEY_V = 0x56,
    KEY_W = 0x57,
    KEY_X = 0x58,
    KEY_Y = 0x59,
    KEY_Z = 0x5a,

    KEY_LEFT  = 0x01000012,
    KEY_UP    = 0x01000013,
    KEY_RIGHT = 0x01000014,
    KEY_DOWN  = 0x01000015,

    KEY_F1  = 0x01000030,
    KEY_F2  = 0x01000031,
    KEY_F3  = 0x01000032,
    KEY_F4  = 0x01000033,
    KEY_F5  = 0x01000034,
    KEY_F6  = 0x01000035,
    KEY_F7  = 0x01000036,
    KEY_F8  = 0x01000037,
    KEY_F9  = 0x01000038,
    KEY_F10 = 0x01000039,
    KEY_F11 = 0x0100003a,
    KEY_F12 = 0x0100003b,
};

} // namespace vcl

#endif // VCL_GUI_INPUT_H
