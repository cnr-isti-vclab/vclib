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

#ifndef VCL_IMGUI_HELPERS_H
#define VCL_IMGUI_HELPERS_H

#include <imgui.h>
#include <functional>

namespace ImGui {

inline bool Checkbox(
    const char* label,
    std::function<bool()> get,
    std::function<void(bool)> set)
{
    bool value = get();
    const bool ret = ImGui::Checkbox(label, &value);
    set(value);
    return ret;
}

inline bool RadioButton(
    const char* label,
    std::function<bool()> get,
    std::function<void(bool)> set)
{
    bool value = get();
    const bool ret = ImGui::RadioButton(label, value);
    if (ret) {
        set(true);
    }
    return ret;
}

inline bool SliderFloat(
    const char* label,
    std::function<float()> get,
    std::function<void(float)> set,
    float vMin,
    float vMax)
{
    
    float value = get();
    const bool ret = ImGui::SliderFloat(label, &value, vMin, vMax);
    if (ret) {
        set(value);
    }
    return ret;
}

} // namespace ImGui

#endif // VCL_IMGUI_HELPERS_H
