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

#ifndef VCL_RENDER_INPUT_H
#define VCL_RENDER_INPUT_H

#include <vclib/space/core/bit_set.h>

namespace vcl {

struct MouseButton
{
    enum Enum {
        LEFT      = 0,
        RIGHT     = 1,
        MIDDLE    = 2,
        NO_BUTTON = 3,
    };
};

struct KeyModifier
{
    enum Enum {
        SHIFT       = 0,
        CONTROL     = 1,
        ALT         = 2,
        NO_MODIFIER = 3,
    };
};

using KeyModifiers = BitSet<char>;

struct Key
{
    enum Enum {
        SPACE = 0x20,

        _0 = 0x30,
        _1 = 0x31,
        _2 = 0x32,
        _3 = 0x33,
        _4 = 0x34,
        _5 = 0x35,
        _6 = 0x36,
        _7 = 0x37,
        _8 = 0x38,
        _9 = 0x39,

        A = 0x41,
        B = 0x42,
        C = 0x43,
        D = 0x44,
        E = 0x45,
        F = 0x46,
        G = 0x47,
        H = 0x48,
        I = 0x49,
        J = 0x4a,
        K = 0x4b,
        L = 0x4c,
        M = 0x4d,
        N = 0x4e,
        O = 0x4f,
        P = 0x50,
        Q = 0x51,
        R = 0x52,
        S = 0x53,
        T = 0x54,
        U = 0x55,
        V = 0x56,
        W = 0x57,
        X = 0x58,
        Y = 0x59,
        Z = 0x5a,

        LEFT  = 0x01000012,
        UP    = 0x01000013,
        RIGHT = 0x01000014,
        DOWN  = 0x01000015,

        SHIFT   = 0x01000020, // modifiers
        CONTROL = 0x01000021,
        SUPER   = 0x01000022,
        ALT     = 0x01000023,

        CAPS_LOCK   = 0x01000024,
        NUM_LOCK    = 0x01000025,
        SCROLL_LOCK = 0x01000026,

        F1  = 0x01000030,
        F2  = 0x01000031,
        F3  = 0x01000032,
        F4  = 0x01000033,
        F5  = 0x01000034,
        F6  = 0x01000035,
        F7  = 0x01000036,
        F8  = 0x01000037,
        F9  = 0x01000038,
        F10 = 0x01000039,
        F11 = 0x0100003a,
        F12 = 0x0100003b,

        NP_0 = 0x010000a2,
        NP_1 = 0x010000a3,
        NP_2 = 0x010000a4,
        NP_3 = 0x010000a5,
        NP_4 = 0x010000a6,
        NP_5 = 0x010000a7,
        NP_6 = 0x010000a8,
        NP_7 = 0x010000a9,
        NP_8 = 0x010000aa,
        NP_9 = 0x010000ab,

        UNKNOWN = 0x01ffffff
    };
};

inline bool isModifierKey(Key::Enum key)
{
    return key == Key::SHIFT || key == Key::CONTROL || key == Key::SUPER ||
           key == Key::ALT;
}

inline Key::Enum modifierToKey(KeyModifier::Enum modifier)
{
    switch (modifier) {
    case KeyModifier::SHIFT: return Key::SHIFT;
    case KeyModifier::CONTROL: return Key::CONTROL;
    case KeyModifier::ALT: return Key::ALT;
    default: return Key::UNKNOWN;
    }
}

inline KeyModifier::Enum keyToModifier(Key::Enum key)
{
    switch (key) {
    case Key::SHIFT: return KeyModifier::SHIFT;
    case Key::CONTROL: return KeyModifier::CONTROL;
    case Key::ALT: return KeyModifier::ALT;
    default: return KeyModifier::NO_MODIFIER;
    }
}

} // namespace vcl

#endif // VCL_RENDER_INPUT_H
