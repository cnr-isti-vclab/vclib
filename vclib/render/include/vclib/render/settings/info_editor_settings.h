// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_SETTINGS_INFO_EDITOR_SETTINGS_H
#define VCL_RENDER_SETTINGS_INFO_EDITOR_SETTINGS_H

#include <vclib/render/settings/editor_settings.h>

#include <vclib/space/core.h>

namespace vcl {

struct InfoEditorSettings : public EditorSettings
{
    vcl::Color color     = vcl::Color::Red;
    float      thickness = 5.0f;
    vcl::Color textColor = vcl::Color::Black;
    int        textSize  = 20;
};

} // namespace vcl

#endif // VCL_RENDER_SETTINGS_INFO_EDITOR_SETTINGS_H
