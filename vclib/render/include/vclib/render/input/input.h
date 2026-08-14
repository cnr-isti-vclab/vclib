// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_INPUT_INPUT_H
#define VCL_RENDER_INPUT_INPUT_H

#include <vclib/space/core.h>

#include <string>
#include <vector>

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
        SUPER       = 3,
        NO_MODIFIER = 4,
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

        ESCAPE = 0x01000000,

        LEFT  = 0x01000012,
        UP    = 0x01000013,
        RIGHT = 0x01000014,
        DOWN  = 0x01000015,

        SHIFT_KEY   = 0x01000020, // modifiers
        CONTROL_KEY = 0x01000021,
        SUPER_KEY   = 0x01000022,
        ALT_KEY     = 0x01000023,

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
    return key == Key::SHIFT_KEY || key == Key::CONTROL_KEY ||
           key == Key::SUPER_KEY || key == Key::ALT_KEY;
}

inline Key::Enum modifierToKey(KeyModifier::Enum modifier)
{
    switch (modifier) {
    case KeyModifier::SHIFT: return Key::SHIFT_KEY;
    case KeyModifier::CONTROL: return Key::CONTROL_KEY;
    case KeyModifier::SUPER: return Key::SUPER_KEY;
    case KeyModifier::ALT: return Key::ALT_KEY;
    default: return Key::UNKNOWN;
    }
}

inline KeyModifier::Enum keyToModifier(Key::Enum key)
{
    switch (key) {
    case Key::SHIFT_KEY: return KeyModifier::SHIFT;
    case Key::CONTROL_KEY: return KeyModifier::CONTROL;
    case Key::SUPER_KEY: return KeyModifier::SUPER;
    case Key::ALT_KEY: return KeyModifier::ALT;
    default: return KeyModifier::NO_MODIFIER;
    }
}

inline std::string toString(MouseButton::Enum b)
{
    switch (b) {
    case MouseButton::LEFT: return "Left";
    case MouseButton::RIGHT: return "Right";
    case MouseButton::MIDDLE: return "Middle";
    case MouseButton::NO_BUTTON: return "None";
    default: return "Unknown";
    }
}

inline MouseButton::Enum mouseButtonFromString(const std::string& str)
{
    if (str == "Left")
        return MouseButton::LEFT;
    if (str == "Right")
        return MouseButton::RIGHT;
    if (str == "Middle")
        return MouseButton::MIDDLE;
    return MouseButton::NO_BUTTON;
}

inline std::string toString(KeyModifier::Enum m)
{
    switch (m) {
    case KeyModifier::SHIFT: return "Shift";
    case KeyModifier::CONTROL: return "Ctrl";
    case KeyModifier::ALT: return "Alt";
    case KeyModifier::SUPER: return "Super";
    case KeyModifier::NO_MODIFIER: return "None";
    default: return "Unknown";
    }
}

inline KeyModifier::Enum keyModifierFromString(const std::string& str)
{
    if (str == "Shift")
        return KeyModifier::SHIFT;
    if (str == "Ctrl")
        return KeyModifier::CONTROL;
    if (str == "Alt")
        return KeyModifier::ALT;
    if (str == "Super")
        return KeyModifier::SUPER;
    return KeyModifier::NO_MODIFIER;
}

inline std::string toString(KeyModifiers mods)
{
    if (mods.none())
        return "";
    std::string res;
    if (mods[KeyModifier::CONTROL])
        res += "Ctrl+";
    if (mods[KeyModifier::ALT])
        res += "Alt+";
    if (mods[KeyModifier::SHIFT])
        res += "Shift+";
    if (mods[KeyModifier::SUPER])
        res += "Super+";
    if (!res.empty())
        res.pop_back(); // remove trailing +
    return res;
}

inline KeyModifiers keyModifiersFromString(const std::string& str)
{
    KeyModifiers mods;
    if (str.find("Ctrl") != std::string::npos)
        mods[KeyModifier::CONTROL] = true;
    if (str.find("Alt") != std::string::npos)
        mods[KeyModifier::ALT] = true;
    if (str.find("Shift") != std::string::npos)
        mods[KeyModifier::SHIFT] = true;
    if (str.find("Super") != std::string::npos)
        mods[KeyModifier::SUPER] = true;
    return mods;
}

inline std::string toString(Key::Enum k)
{
    if (k >= Key::A && k <= Key::Z) {
        return std::string(1, static_cast<char>('A' + (k - Key::A)));
    }
    if (k >= Key::_0 && k <= Key::_9) {
        return std::string(1, static_cast<char>('0' + (k - Key::_0)));
    }
    if (k >= Key::F1 && k <= Key::F12) {
        return "F" + std::to_string((k - Key::F1) + 1);
    }
    if (k >= Key::NP_0 && k <= Key::NP_9) {
        return "NumPad " + std::to_string(k - Key::NP_0);
    }

    switch (k) {
    case Key::SPACE: return "Space";
    case Key::ESCAPE: return "Escape";
    case Key::LEFT: return "Left";
    case Key::UP: return "Up";
    case Key::RIGHT: return "Right";
    case Key::DOWN: return "Down";
    case Key::SHIFT_KEY: return "Shift";
    case Key::CONTROL_KEY: return "Ctrl";
    case Key::SUPER_KEY: return "Super";
    case Key::ALT_KEY: return "Alt";
    case Key::CAPS_LOCK: return "Caps Lock";
    case Key::NUM_LOCK: return "Num Lock";
    case Key::SCROLL_LOCK: return "Scroll Lock";
    default: return "Unknown";
    }
}

inline Key::Enum keyFromString(const std::string& str)
{
    if (str.length() == 1) {
        char c = str[0];
        if (c >= 'A' && c <= 'Z')
            return static_cast<Key::Enum>(Key::A + (c - 'A'));
        if (c >= 'a' && c <= 'z')
            return static_cast<Key::Enum>(Key::A + (c - 'a'));
        if (c >= '0' && c <= '9')
            return static_cast<Key::Enum>(Key::_0 + (c - '0'));
    }
    if (str.size() > 1 && str[0] == 'F' && str[1] >= '1' && str[1] <= '9') {
        int f = std::stoi(str.substr(1));
        if (f >= 1 && f <= 12)
            return static_cast<Key::Enum>(Key::F1 + (f - 1));
    }
    if (str.find("NumPad ") == 0) {
        int n = std::stoi(str.substr(7));
        if (n >= 0 && n <= 9)
            return static_cast<Key::Enum>(Key::NP_0 + n);
    }

    if (str == "Space")
        return Key::SPACE;
    if (str == "Escape")
        return Key::ESCAPE;
    if (str == "Left")
        return Key::LEFT;
    if (str == "Up")
        return Key::UP;
    if (str == "Right")
        return Key::RIGHT;
    if (str == "Down")
        return Key::DOWN;
    if (str == "Shift")
        return Key::SHIFT_KEY;
    if (str == "Ctrl")
        return Key::CONTROL_KEY;
    if (str == "Super")
        return Key::SUPER_KEY;
    if (str == "Alt")
        return Key::ALT_KEY;
    if (str == "Caps Lock")
        return Key::CAPS_LOCK;
    if (str == "Num Lock")
        return Key::NUM_LOCK;
    if (str == "Scroll Lock")
        return Key::SCROLL_LOCK;

    return Key::UNKNOWN;
}

} // namespace vcl

#endif // VCL_RENDER_INPUT_INPUT_H
