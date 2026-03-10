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

#include <vclib/render/settings/editor_settings.h>

#include <QFrame>
#include <QMenu>
#include <QPushButton>
#include <QWidgetAction>

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
    QPushButton* addButton(const QIcon& icon, bool checkable = true);

    QPushButton* settingsButton() const;

    void hideSettingsButton() {
        settingsButton()->setVisible(false);
    }

    void showSettingsButton() {
        settingsButton()->setVisible(true);
    }

    template<typename SettingsFrame>
    [[nodiscard]] SettingsFrame* setSettingsFrame(EditorSettings& sts)
    {
        QWidgetAction* wa = new QWidgetAction(this);
        SettingsFrame*sf = new SettingsFrame(sts);
        wa->setDefaultWidget(sf);
        QMenu* popupMenu = new QMenu(this);
        popupMenu->addAction(wa);
        settingsButton()->setMenu(popupMenu);
        settingsButton()->setStyleSheet(
            "QPushButton::menu-indicator {"
            "    subcontrol-origin: padding;"
            "    subcontrol-position: center center;"
            "}");
        return sf;
    }

protected slots:
    virtual void refreshSettings() {};
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_EDITORS_GENERIC_EDITOR_FRAME_H
