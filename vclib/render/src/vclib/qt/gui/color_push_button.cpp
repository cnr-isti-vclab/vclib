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

#include <vclib/qt/gui/color_push_button.h>

#include <QColorDialog>

namespace vcl::qt {

ColorPushButton::ColorPushButton(QWidget* parent) :
        QPushButton(parent)
{
    setBackgroundColor(QColor());
    connect(this, SIGNAL(clicked(bool)), this, SLOT(onClicked(bool)));
}

ColorPushButton::ColorPushButton(const QColor& c, QWidget* parent) :
        QPushButton(parent)
{
    setBackgroundColor(c);
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
