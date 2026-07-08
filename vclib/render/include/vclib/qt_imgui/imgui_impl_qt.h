// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_IMGUI_IMGUI_IMPL_QT_H
#define VCL_QT_IMGUI_IMGUI_IMPL_QT_H

#include <QWidget>
#include <imgui/imgui.h>
#ifndef IMGUI_DISABLE

IMGUI_IMPL_API bool ImGui_ImplQt_Init(QWidget* widget);
IMGUI_IMPL_API void ImGui_ImplQt_Shutdown();
IMGUI_IMPL_API void ImGui_ImplQt_NewFrame();

#endif // IMGUI_DISABLE
#endif // VCL_QT_IMGUI_IMGUI_IMPL_QT_H
