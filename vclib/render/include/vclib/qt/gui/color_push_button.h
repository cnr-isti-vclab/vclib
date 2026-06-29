// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_COLOR_PUSH_BUTTON_H
#define VCL_QT_GUI_COLOR_PUSH_BUTTON_H

#include <QColor>
#include <QPushButton>

namespace vcl::qt {

class ColorPushButton : public QPushButton
{
    Q_OBJECT

public:
    explicit ColorPushButton(const QColor& c, QWidget* parent = Q_NULLPTR);
    explicit ColorPushButton(QWidget* parent = Q_NULLPTR);

    ~ColorPushButton() = default;

    void   setBackgroundColor(const QColor& c);
    QColor getBackgroundColor() const;

signals:
    void colorChanged(QColor);

private slots:
    void onClicked(bool checked);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_COLOR_PUSH_BUTTON_H
