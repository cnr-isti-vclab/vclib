// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IMGUI_IMGUI_HELPERS_H
#define VCL_IMGUI_IMGUI_HELPERS_H

#include <functional>
#include <imgui/imgui.h>

#include <vclib/space/core/color.h>

namespace ImGui {

inline bool Checkbox(
    const char*               label,
    std::function<bool()>     get,
    std::function<void(bool)> set)
{
    bool       value = get();
    const bool ret   = ImGui::Checkbox(label, &value);
    set(value);
    return ret;
}

inline bool RadioButton(
    const char*               label,
    std::function<bool()>     get,
    std::function<void(bool)> set)
{
    bool       value = get();
    const bool ret   = ImGui::RadioButton(label, value);
    if (ret) {
        set(true);
    }
    return ret;
}

inline bool SliderFloat(
    const char*                label,
    std::function<float()>     get,
    std::function<void(float)> set,
    float                      vMin,
    float                      vMax)
{
    float      value = get();
    const bool ret   = ImGui::SliderFloat(label, &value, vMin, vMax);
    if (ret) {
        set(value);
    }
    return ret;
}

inline bool ColorEdit4(
    const char*                     label,
    std::function<vcl::Color()>     get,
    std::function<void(vcl::Color)> set,
    ImGuiColorEditFlags             flags)
{
    // TODO: use float based color
    vcl::Color c        = get();
    float      color[4] = {c.redF(), c.greenF(), c.blueF(), c.alphaF()};
    const bool ret      = ImGui::ColorEdit4(label, color, flags);
    if (ret) {
        c.setRgbF(color[0], color[1], color[2], color[3]);
        set(c);
    }
    return ret;
}

} // namespace ImGui

#endif // VCL_IMGUI_IMGUI_HELPERS_H
