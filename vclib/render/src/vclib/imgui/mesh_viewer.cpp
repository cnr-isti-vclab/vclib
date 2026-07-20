// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/imgui/mesh_viewer.h>

namespace vcl::imgui {

template<typename V>
void setViewerSetts(V& v, const ViewerSettings& settings)
{
    if constexpr (ViewerConcept<V>) {
        v.setViewerSettings(settings);
    }
}

void MeshViewer::setViewerSettings(const ViewerSettings& settings)
{
    setViewerSetts(mApp, settings);
}

template<typename V>
ViewerSettings viewerSetts(const V& v)
{
    if constexpr (ViewerConcept<V>) {
        return v.viewerSettings();
    }
    return ViewerSettings {};
}

ViewerSettings MeshViewer::viewerSettings() const
{
    return viewerSetts(mApp);
}

template<typename V>
void setPanor(V& v, const std::string& panorama)
{
    if constexpr (ViewerConcept<V>) {
        v.setPanorama(panorama);
    }
}

void MeshViewer::setPanorama(const std::string& panorama)
{
    setPanor(mApp, panorama);
}

} // namespace vcl::imgui
