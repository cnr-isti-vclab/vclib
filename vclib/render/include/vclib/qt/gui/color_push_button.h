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

#ifndef VCL_QT_GUI_COLOR_PUSH_BUTTON_H
#define VCL_QT_GUI_COLOR_PUSH_BUTTON_H

#include <QColor>
#include <QPushButton>

namespace vcl::qt {

class ColorPushButton : public QPushButton
{
    Q_OBJECT

public:
    explicit ColorPushButton(QWidget* parent = Q_NULLPTR);
    explicit ColorPushButton(const QColor& c, QWidget* parent = Q_NULLPTR);

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
