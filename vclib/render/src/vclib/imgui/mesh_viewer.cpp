// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/imgui/mesh_viewer.h>

namespace vcl::imgui {

template<typename V>
void setPbrSetts(V& v, const PBRViewerSettings& settings)
{
    if constexpr (PBRViewerConcept<V>) {
        v.setPbrSettings(settings);
    }
}

void MeshViewer::setPbrSettings(const PBRViewerSettings& settings)
{
    setPbrSetts(mApp, settings);
}

template<typename V>
PBRViewerSettings pbrSetts(const V& v)
{
    if constexpr (PBRViewerConcept<V>) {
        return v.pbrSettings();
    }
    return PBRViewerSettings {};
}

PBRViewerSettings MeshViewer::pbrSettings() const
{
    return pbrSetts(mApp);
}

template<typename V>
void setPanor(V& v, const std::string& panorama)
{
    if constexpr (PBRViewerConcept<V>) {
        v.setPanorama(panorama);
    }
}

void MeshViewer::setPanorama(const std::string& panorama)
{
    setPanor(mApp, panorama);
}

} // namespace vcl::imgui
