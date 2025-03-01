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

#ifndef VCL_QT_IMGUI_IMGUI_IMPL_QT_H
#define VCL_QT_IMGUI_IMGUI_IMPL_QT_H

#include <QWidget>
#include <imgui.h>
#ifndef IMGUI_DISABLE

IMGUI_IMPL_API bool ImGui_ImplQt_Init(QWidget* widget);
IMGUI_IMPL_API void ImGui_ImplQt_Shutdown();
IMGUI_IMPL_API void ImGui_ImplQt_NewFrame();

#endif // IMGUI_DISABLE
#endif // VCL_QT_IMGUI_IMGUI_IMPL_QT_H
