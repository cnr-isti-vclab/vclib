/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_QT_GUI_EDITORS_BOUNDING_BOX_EDITOR_FRAME_H
#define VCL_QT_GUI_EDITORS_BOUNDING_BOX_EDITOR_FRAME_H

#include "settings/bounding_box_editor_settings_frame.h"
#include "generic_editor_frame.h"

#include <vclib/render/editors/bounding_box_editor.h>

namespace vcl::qt {

template<typename ViewerType>
class BoundingBoxEditorFrame : public GenericEditorFrame
{
    using Base = GenericEditorFrame;

    std::shared_ptr<vcl::BoundingBoxEditor<ViewerType>>
        mBoundingBoxEditor;

public:
    explicit BoundingBoxEditorFrame(QWidget* parent = nullptr) :
            GenericEditorFrame(parent)
    {
        QIcon ic(":/icons/bbox.png");

        GenericEditorFrame::setIcon(ic);
    }

    explicit BoundingBoxEditorFrame(
        std::shared_ptr<vcl::BoundingBoxEditor<ViewerType>> ptr,
        QWidget*                                            parent = nullptr) :
            BoundingBoxEditorFrame(parent)
    {
        mBoundingBoxEditor = ptr;

        connect(
            Base::editorButton(),
            &QPushButton::clicked,
            this,
            [this]() {
                if (mBoundingBoxEditor) {
                    mBoundingBoxEditor->setActive(
                        !mBoundingBoxEditor->isActive());
                }
            });

        Base::setSettingsFrame<BoundingBoxEditorSettingsFrame>();
    }
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_EDITORS_BOUNDING_BOX_EDITOR_FRAME_H
