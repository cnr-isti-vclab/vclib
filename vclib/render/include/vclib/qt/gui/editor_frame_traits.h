// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_EDITOR_FRAME_TRAITS_H
#define VCL_QT_GUI_EDITOR_FRAME_TRAITS_H

#include <vclib/render/editors.h>
#include <type_traits>

namespace vcl::qt {

/**
 * @brief Traits struct to associate an Editor type with its Qt Frame type.
 * 
 * Specializations of this struct should define `FrameType` as the QWidget 
 * subclass that implements the UI frame for the specific editor. If no frame 
 * is associated, `FrameType` defaults to `void`.
 */
template <template<typename> typename EditorT, typename ViewerType>
struct EditorFrameTraits {
    using FrameType = void;
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_EDITOR_FRAME_TRAITS_H
