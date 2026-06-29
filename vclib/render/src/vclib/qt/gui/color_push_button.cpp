// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/color_push_button.h>

#include <QColorDialog>

namespace vcl::qt {

ColorPushButton::ColorPushButton(const QColor& c, QWidget* parent) :
        QPushButton(parent)
{
    setBackgroundColor(c);
    connect(this, SIGNAL(clicked(bool)), this, SLOT(onClicked(bool)));
    setAutoFillBackground(true);
}

ColorPushButton::ColorPushButton(QWidget* parent) :
        ColorPushButton(QColor(), parent)
{
}

void ColorPushButton::setBackgroundColor(const QColor& c)
{
    QPalette px;
    px.setColor(QPalette::Button, c);
    setPalette(px);
    update();
}

QColor ColorPushButton::getBackgroundColor() const
{
    QPalette px = palette();
    return px.color(QPalette::Button);
}

void ColorPushButton::onClicked(bool checked)
{
    QColor color = QColorDialog::getColor(getBackgroundColor(), this);
    if (color.isValid()) {
        setBackgroundColor(color);
        emit colorChanged(color);
    }
}

} // namespace vcl::qt
