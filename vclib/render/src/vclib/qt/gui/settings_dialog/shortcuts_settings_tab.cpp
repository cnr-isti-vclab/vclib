// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/settings_dialog/shortcuts_settings_tab.h>

#include <QLabel>
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
    QWidget* widget = new QWidget(parent);
    QVBoxLayout* layout = new QVBoxLayout(widget);
    
    QLabel* placeholderLabel = new QLabel("Key Bindings coming...", widget);
    placeholderLabel->setAlignment(Qt::AlignCenter);
    
    layout->addWidget(placeholderLabel);
    layout->addStretch();

    return widget;
}

} // namespace vcl::qt
