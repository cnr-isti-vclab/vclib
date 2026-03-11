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

#include <vclib/qt/gui/editors/generic_editor_frame.h>

#include "ui_generic_editor_frame.h"

void initIcons()
{
    Q_INIT_RESOURCE(icons);
}

namespace vcl::qt {

std::once_flag initIconsFlag;

GenericEditorFrame::GenericEditorFrame(QWidget* parent) :
        QFrame(parent), mUI(new Ui::GenericEditorFrame)
{
    mUI->setupUi(this);
    std::call_once(initIconsFlag, initIcons);
}

GenericEditorFrame::~GenericEditorFrame()
{
    delete mUI;
}

QPushButton* GenericEditorFrame::addButton(
    const QIcon& icon,
    bool         checkable)
{
    QPushButton* button = new QPushButton(this);
    button->setIcon(icon);
    button->setIconSize(QSize(50,50));
    button->setMinimumSize(50,50);
    button->setMaximumSize(50,50);

    button->setCheckable(checkable);

    mUI->buttonFrame->layout()->addWidget(button);
    return button;
}

QPushButton* GenericEditorFrame::settingsButton() const
{
    return mUI->settingsPushButton;
}

} // namespace vcl::qt
