// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/input_bindings_widget.h>
#include <vclib/qt/gui/settings_dialog/settings_dialog_data.h>
#include <vclib/qt/gui/settings_dialog/shortcuts_settings_tab.h>
#include <vclib/render/input/abstract_input_action_map.h>

#include <QLabel>
#include <QScrollArea>
#include <QToolButton>
#include <QVBoxLayout>

namespace vcl::qt {

QString ShortcutsSettingsTab::category() const
{
    return "Shortcuts";
}

QString ShortcutsSettingsTab::name() const
{
    return "Key Bindings";
}

QWidget* ShortcutsSettingsTab::createWidget(QWidget* parent)
{
    mWidgets.clear();

    QScrollArea* scrollArea = new QScrollArea(parent);
    scrollArea->setWidgetResizable(true);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget*     scrollContent = new QWidget(scrollArea);
    QVBoxLayout* layout        = new QVBoxLayout(scrollContent);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(5);

    QLabel* infoLabel = new QLabel(
        "Note: Editor shortcuts are only active when the editor is enabled. "
        "If a conflict occurs, active editors override the base Viewer "
        "shortcuts.",
        scrollContent);
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("color: gray; font-style: italic;");
    layout->addWidget(infoLabel);
    layout->addSpacing(10);

    auto groups = mProvider();
    for (const auto& group : groups) {
        QLabel* titleLabel =
            new QLabel(QString::fromStdString(group.name), scrollContent);
        QFont font = titleLabel->font();
        font.setBold(true);
        font.setPointSize(font.pointSize() + 2);
        titleLabel->setFont(font);
        layout->addWidget(titleLabel);

        for (const auto& mapRef : group.maps) {
            QToolButton* toggleButton = new QToolButton(scrollContent);
            toggleButton->setStyleSheet(
                "QToolButton { border: none; font-weight: bold; text-align: "
                "left; }");
            toggleButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            toggleButton->setArrowType(Qt::DownArrow);
            toggleButton->setText(
                QString::fromStdString(mapRef.get().mapName()));
            toggleButton->setCheckable(true);
            toggleButton->setChecked(false);

            InputBindingsWidget* bindingsWidget =
                new InputBindingsWidget(mapRef, scrollContent);

            QObject::connect(
                toggleButton,
                &QToolButton::toggled,
                bindingsWidget,
                [toggleButton, bindingsWidget](bool checked) {
                    toggleButton->setArrowType(
                        checked ? Qt::RightArrow : Qt::DownArrow);
                    bindingsWidget->setVisible(!checked);
                });

            layout->addWidget(toggleButton);
            layout->addWidget(bindingsWidget);
            mWidgets.push_back(bindingsWidget);
        }

        layout->addSpacing(15);
    }

    if (groups.empty()) {
        QLabel* placeholderLabel =
            new QLabel("No shortcut available.", scrollContent);
        placeholderLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(placeholderLabel);
    }

    layout->addStretch();
    scrollArea->setWidget(scrollContent);

    return scrollArea;
}

void ShortcutsSettingsTab::applySettings()
{
    for (auto* widget : mWidgets) {
        widget->applySettings();
    }
}

void ShortcutsSettingsTab::saveSettings(nlohmann::json& /*j*/) const
{
}

} // namespace vcl::qt
