// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_CONCEPTS_PBR_VIEWER_H
#define VCL_RENDER_CONCEPTS_PBR_VIEWER_H

#include <vclib/render/settings/pbr_viewer_settings.h>

#include <vclib/base.h>

namespace vcl {

template<typename T>
concept PBRViewerConcept = requires (T&& obj) {
    { obj.pbrSettings() } -> std::same_as<const PBRViewerSettings&>;

    // non const requirements
    requires IsConst<T> ||
                 requires { obj.setPbrSettings(PBRViewerSettings()); };
};
} // namespace vcl

#endif // VCL_RENDER_CONCEPTS_PBR_VIEWER_H
