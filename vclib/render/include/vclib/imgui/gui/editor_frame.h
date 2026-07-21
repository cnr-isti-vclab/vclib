// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IMGUI_GUI_EDITOR_FRAME_H
#define VCL_IMGUI_GUI_EDITOR_FRAME_H

namespace vcl::imgui {

class EditorFrameImgui {
public:
    virtual void draw() = 0;
    virtual ~EditorFrameImgui() = default;
};

template <template<typename> typename EditorT, typename ViewerType>
struct EditorFrameTraits {
    using FrameType = void;
};

} // namespace vcl::imgui

#endif // VCL_IMGUI_GUI_EDITOR_FRAME_H
