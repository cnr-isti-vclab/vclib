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
        SPACE,

        _0,
        _1,
        _2,
        _3,
        _4,
        _5,
        _6,
        _7,
        _8,
        _9,

        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        ESCAPE,

        LEFT,
        UP,
        RIGHT,
        DOWN,

        SHIFT_KEY, // modifiers
        CONTROL_KEY,
        SUPER_KEY,
        ALT_KEY,

        CAPS_LOCK,
        NUM_LOCK,
        SCROLL_LOCK,

        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,

        NP_0,
        NP_1,
        NP_2,
        NP_3,
        NP_4,
        NP_5,
        NP_6,
        NP_7,
        NP_8,
        NP_9,

        UNKNOWN
    };
};

struct ScrollAxis
{
    enum Enum {
        HORIZONTAL = 0,
        VERTICAL   = 1,
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

inline void fromString(const std::string& str, MouseButton::Enum& out)
{
    if (str == "Left")
        out = MouseButton::LEFT;
    else if (str == "Right")
        out = MouseButton::RIGHT;
    else if (str == "Middle")
        out = MouseButton::MIDDLE;
    else
        out = MouseButton::NO_BUTTON;
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

inline void fromString(const std::string& str, KeyModifier::Enum& out)
{
    if (str == "Shift")
        out = KeyModifier::SHIFT;
    else if (str == "Ctrl")
        out = KeyModifier::CONTROL;
    else if (str == "Alt")
        out = KeyModifier::ALT;
    else if (str == "Super")
        out = KeyModifier::SUPER;
    else
        out = KeyModifier::NO_MODIFIER;
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

inline void fromString(const std::string& str, KeyModifiers& out)
{
    out.reset();
    if (str.find("Ctrl") != std::string::npos)
        out[KeyModifier::CONTROL] = true;
    if (str.find("Alt") != std::string::npos)
        out[KeyModifier::ALT] = true;
    if (str.find("Shift") != std::string::npos)
        out[KeyModifier::SHIFT] = true;
    if (str.find("Super") != std::string::npos)
        out[KeyModifier::SUPER] = true;
    if (out.none())
        out[KeyModifier::NO_MODIFIER] = true;
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

inline void fromString(const std::string& str, Key::Enum& out)
{
    if (str.length() == 1) {
        char c = str[0];
        if (c >= 'A' && c <= 'Z') {
            out = static_cast<Key::Enum>(Key::A + (c - 'A'));
            return;
        }
        if (c >= 'a' && c <= 'z') {
            out = static_cast<Key::Enum>(Key::A + (c - 'a'));
            return;
        }
        if (c >= '0' && c <= '9') {
            out = static_cast<Key::Enum>(Key::_0 + (c - '0'));
            return;
        }
    }
    if (str.size() > 1 && str[0] == 'F' && str[1] >= '1' && str[1] <= '9') {
        int f = std::stoi(str.substr(1));
        if (f >= 1 && f <= 12) {
            out = static_cast<Key::Enum>(Key::F1 + (f - 1));
            return;
        }
    }
    if (str.find("NumPad ") == 0) {
        int n = std::stoi(str.substr(7));
        if (n >= 0 && n <= 9) {
            out = static_cast<Key::Enum>(Key::NP_0 + n);
            return;
        }
    }

    if (str == "Space")
        out = Key::SPACE;
    else if (str == "Escape")
        out = Key::ESCAPE;
    else if (str == "Left")
        out = Key::LEFT;
    else if (str == "Up")
        out = Key::UP;
    else if (str == "Right")
        out = Key::RIGHT;
    else if (str == "Down")
        out = Key::DOWN;
    else if (str == "Shift")
        out = Key::SHIFT_KEY;
    else if (str == "Ctrl")
        out = Key::CONTROL_KEY;
    else if (str == "Super")
        out = Key::SUPER_KEY;
    else if (str == "Alt")
        out = Key::ALT_KEY;
    else if (str == "Caps Lock")
        out = Key::CAPS_LOCK;
    else if (str == "Num Lock")
        out = Key::NUM_LOCK;
    else if (str == "Scroll Lock")
        out = Key::SCROLL_LOCK;
    else
        out = Key::UNKNOWN;
}

inline std::string toString(ScrollAxis::Enum axis)
{
    switch (axis) {
    case ScrollAxis::HORIZONTAL: return "Horizontal";
    case ScrollAxis::VERTICAL: return "Vertical";
    default: return "Unknown";
    }
}

inline void fromString(const std::string& str, ScrollAxis::Enum& out)
{
    if (str == "Horizontal")
        out = ScrollAxis::HORIZONTAL;
    else if (str == "Vertical")
        out = ScrollAxis::VERTICAL;
    else {
        throw std::invalid_argument(
            "fromString ScrollAxis::Enum failed to parse: '" + str + "'");
    }
}

// --- std::pair conversions ---

inline std::string toString(
    const std::pair<MouseButton::Enum, KeyModifiers>& input)
{
    std::string modStr = toString(input.second);
    std::string btnStr = toString(input.first);
    if (modStr.empty())
        return btnStr;
    return modStr + "+" + btnStr;
}

inline void fromString(
    const std::string&                          str,
    std::pair<MouseButton::Enum, KeyModifiers>& out)
{
    size_t lastPlus = str.find_last_of('+');
    if (lastPlus == std::string::npos) {
        fromString("", out.second);
        fromString(str, out.first);
    }
    else {
        fromString(str.substr(0, lastPlus), out.second);
        fromString(str.substr(lastPlus + 1), out.first);
    }
}

inline std::string toString(const std::pair<Key::Enum, KeyModifiers>& input)
{
    std::string modStr = toString(input.second);
    std::string keyStr = toString(input.first);
    if (modStr.empty())
        return keyStr;
    return modStr + "+" + keyStr;
}

inline void fromString(
    const std::string&                  str,
    std::pair<Key::Enum, KeyModifiers>& out)
{
    size_t lastPlus = str.find_last_of('+');
    if (lastPlus == std::string::npos) {
        fromString("", out.second);
        fromString(str, out.first);
    }
    else {
        fromString(str.substr(0, lastPlus), out.second);
        fromString(str.substr(lastPlus + 1), out.first);
    }
}

inline std::string toString(
    const std::pair<ScrollAxis::Enum, KeyModifiers>& input)
{
    std::string modStr  = toString(input.second);
    std::string axisStr = toString(input.first);
    if (modStr.empty())
        return axisStr;
    return modStr + "+" + axisStr;
}

inline void fromString(
    const std::string&                         str,
    std::pair<ScrollAxis::Enum, KeyModifiers>& out)
{
    size_t lastPlus = str.find_last_of('+');
    if (lastPlus == std::string::npos) {
        fromString("", out.second);
        fromString(str, out.first);
    }
    else {
        fromString(str.substr(0, lastPlus), out.second);
        fromString(str.substr(lastPlus + 1), out.first);
    }
}

} // namespace vcl

#endif // VCL_RENDER_INPUT_INPUT_H
