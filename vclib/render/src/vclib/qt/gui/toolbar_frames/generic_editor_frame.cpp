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

#include <vclib/qt/gui/toolbar_frames/generic_editor_frame.h>

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
    mUI->settingsPushButton->setFocusPolicy(Qt::NoFocus);
}

GenericEditorFrame::~GenericEditorFrame()
{
    delete mUI;
}

QPushButton* GenericEditorFrame::addButton(const QIcon& icon, bool checkable)
{
    QPushButton* button = new QPushButton(this);
    button->setIcon(icon);
    // Icon is 32x32 inside a 40x40 button: the 4px padding on each side
    // prevents the native macOS Aqua bezel from clipping the icon.
    button->setIconSize(QSize(32, 32));
    button->setFixedSize(QSize(40, 40));
    button->setCheckable(checkable);
    button->setFocusPolicy(Qt::NoFocus);
    // Stylesheet overrides the native macOS QPushButton rendering (which has
    // a large internal bezel that clips icons) and keeps a consistent look
    // across platforms.
    button->setStyleSheet(
        "QPushButton {"
        "  background-color: transparent;"
        "  border: 1px solid transparent;"
        "  border-radius: 4px;"
        "}"
        "QPushButton:checked {"
        "  background-color: palette(highlight);"
        "  border-color: palette(dark);"
        "}"
        "QPushButton:hover {"
        "  background-color: palette(midlight);"
        "  border-color: palette(dark);"
        "}");

    // add the button before the settings button in the mUI layout
    int settingsButtonIndex =
        mUI->horizontalLayout->indexOf(mUI->settingsPushButton);
    mUI->horizontalLayout->insertWidget(settingsButtonIndex, button);
    return button;
}

QToolButton* GenericEditorFrame::settingsButton() const
{
    return mUI->settingsPushButton;
}

} // namespace vcl::qt
