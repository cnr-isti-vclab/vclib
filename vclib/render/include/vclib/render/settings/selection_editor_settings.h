// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_SETTINGS_SELECTION_EDITOR_SETTINGS_H
#define VCL_RENDER_SETTINGS_SELECTION_EDITOR_SETTINGS_H

#include <vclib/render/settings/editor_settings.h>

#include <vclib/space/core.h>

namespace vcl {

struct SelectionEditorSettings : public EditorSettings
{
    bool       selectVertices    = false;
    bool       selectFaces       = false;
    bool       onlyVisible       = false;
    vcl::Color selectionBoxColor = vcl::Color(27, 120, 249, 64);
};

} // namespace vcl

#endif // VCL_RENDER_SETTINGS_SELECTION_EDITOR_SETTINGS_H
