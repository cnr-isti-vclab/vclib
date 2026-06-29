// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_GLFW_INPUT_H
#define VCL_GLFW_INPUT_H

#include <vclib/render/input.h>

#include <GLFW/glfw3.h>

namespace vcl::glfw {

enum MouseButton : uint {};

enum Key : uint {};

enum KeyboardModifier : uint {};

enum KeyboardModifiers : uint {};

vcl::MouseButton::Enum fromGLFW(glfw::MouseButton button);

vcl::Key::Enum fromGLFW(glfw::Key key);

KeyModifier::Enum fromGLFW(glfw::KeyboardModifier key);

KeyModifiers fromGLFW(glfw::KeyboardModifiers mods);

} // namespace vcl::glfw

#endif // VCL_GLFW_INPUT_H
