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

#ifndef VCL_QT_GUI_EDITORS_GENERIC_EDITOR_FRAME_H
#define VCL_QT_GUI_EDITORS_GENERIC_EDITOR_FRAME_H

#include <QFrame>
#include <QPushButton>

namespace vcl::qt {

namespace Ui {
class GenericEditorFrame;
} // namespace Ui

class GenericEditorFrame : public QFrame
{
    Q_OBJECT

    Ui::GenericEditorFrame* mUI;

public:
    explicit GenericEditorFrame(QWidget* parent = nullptr);
    ~GenericEditorFrame();

protected:
    void setIcon(const QIcon& icon);

    QPushButton* editorButton() const;
    QPushButton* settingsButton() const;
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_EDITORS_GENERIC_EDITOR_FRAME_H
