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
