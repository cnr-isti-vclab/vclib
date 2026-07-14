// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_TOOLBAR_FRAMES_GENERIC_EDITOR_FRAME_H
#define VCL_QT_GUI_TOOLBAR_FRAMES_GENERIC_EDITOR_FRAME_H

#include <vclib/render/settings/editor_settings.h>

#include <QFrame>
#include <QMenu>
#include <QPushButton>
#include <QToolButton>
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

    QToolButton* settingsButton() const;

    void hideSettingsButton() { settingsButton()->setVisible(false); }

    void showSettingsButton() { settingsButton()->setVisible(true); }

    template<typename SettingsFrame>
    [[nodiscard]] SettingsFrame* setSettingsFrame(EditorSettings& sts)
    {
        QMenu*         popupMenu = new QMenu(this);
        QWidgetAction* wa        = new QWidgetAction(popupMenu);
        SettingsFrame* sf        = new SettingsFrame(sts);
        wa->setDefaultWidget(sf);
        popupMenu->addAction(wa);
        settingsButton()->setMenu(popupMenu);
        // Make the arrow centered trough a style sheet
        settingsButton()->setStyleSheet(
            "QToolButton {"
            "  background-color: transparent;"
            "  border: 1px solid transparent;"
            "  border-radius: 4px;"
            "}"
            "QToolButton:hover {"
            "  background-color: palette(midlight);"
            "  border-color: palette(dark);"
            "}"
            "QToolButton::menu-indicator { image: none; width: 0px; }");
        return sf;
    }

protected slots:
    virtual void refreshSettings() {};
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_TOOLBAR_FRAMES_GENERIC_EDITOR_FRAME_H
