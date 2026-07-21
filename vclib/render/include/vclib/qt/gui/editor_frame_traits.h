// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_EDITOR_FRAME_TRAITS_H
#define VCL_QT_GUI_EDITOR_FRAME_TRAITS_H

#include <vclib/qt/gui/toolbar_frames.h>
#include <vclib/render/editors.h>
#include <type_traits>

namespace vcl::qt {

// Default trait: no frame associated
template <template<typename> typename EditorT, typename ViewerType>
struct EditorFrameTraits {
    using FrameType = void;
};

// Specializations for specific editors
template <typename ViewerType>
struct EditorFrameTraits<vcl::BoundingBoxEditor, ViewerType> {
    using FrameType = BoundingBoxEditorFrame<ViewerType>;
};

template <typename ViewerType>
struct EditorFrameTraits<vcl::SelectionEditor, ViewerType> {
    using FrameType = SelectionEditorFrame<ViewerType>;
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_EDITOR_FRAME_TRAITS_H
