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

#ifndef VCL_EXT_GLFW_GUI_INPUT_H
#define VCL_EXT_GLFW_GUI_INPUT_H

#include <GLFW/glfw3.h>

#include <vclib/gui/input.h>

namespace vcl::glfw {

enum MouseButton : uint {};

enum Key : uint {};

enum KeyboardModifier : uint {};

enum KeyboardModifiers : uint {};

vcl::MouseButton fromGLFW(glfw::MouseButton button);

vcl::Key fromGLFW(glfw::Key key);

KeyModifier fromGLFW(glfw::KeyboardModifier key);

KeyModifiers fromGLFW(glfw::KeyboardModifiers mods);

} // namespace vcl::glfw

#endif // VCL_EXT_GLFW_GUI_INPUT_H
