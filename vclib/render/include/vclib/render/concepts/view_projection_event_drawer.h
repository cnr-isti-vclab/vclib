// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_CONCEPTS_VIEW_PROJECTION_EVENT_DRAWER_H
#define VCL_RENDER_CONCEPTS_VIEW_PROJECTION_EVENT_DRAWER_H

#include "event_drawer.h"
#include "view_projection.h"

namespace vcl {

template<typename T>
concept ViewProjectionEventDrawerConcept =
    ViewProjectionConcept<T> && EventDrawerConcept<T>;

} // namespace vcl

#endif // VCL_RENDER_CONCEPTS_VIEW_PROJECTION_EVENT_DRAWER_H
