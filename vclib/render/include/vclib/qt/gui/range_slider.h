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

#ifndef VCL_QT_GUI_RANGE_SLIDER_H
#define VCL_QT_GUI_RANGE_SLIDER_H

#include <QMouseEvent>
#include <QPainter>
#include <QWidget>

// Implementation adapted from https://github.com/ThisIsClark/Qt-RangeSlider

namespace vcl::qt {

class RangeSlider : public QWidget
{
    Q_OBJECT
    Q_ENUMS(RangeSliderTypes)

public:
    enum Option {
        NoHandle      = 0x0,
        LeftHandle    = 0x1,
        RightHandle   = 0x2,
        DoubleHandles = LeftHandle | RightHandle
    };
    Q_DECLARE_FLAGS(Options, Option)

private:
    int             mMinimum             = 0;
    int             mMaximum             = 100;
    int             mLowerValue          = 0;
    int             mUpperValue          = 100;
    bool            mFirstHandlePressed  = false;
    bool            mSecondHandlePressed = false;
    int             mInterval            = 100;
    int             mDelta               = 0;
    Qt::Orientation mOrientation         = Qt::Horizontal;
    Options         mType                = DoubleHandles;

public:
    RangeSlider(
        Qt::Orientation ori,
        Options         t      = DoubleHandles,
        QWidget*        parent = Q_NULLPTR);

    RangeSlider(QWidget* parent = Q_NULLPTR);

    QSize minimumSizeHint() const override;

    int minimum() const;
    int maximum() const;

    int lowerValue() const;
    int upperValue() const;

    void setMaximum(int max);
    void setMinimum(int min);

public slots:
    void setLowerValue(int lowerValue);
    void setUpperValue(int upperValue);
    void setRange(int min, int max);

signals:
    void lowerValueChanged(int lowerValue);
    void upperValueChanged(int upperValue);
    void rangeChanged(int min, int max);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void changeEvent(QEvent* event) override;

    QRectF firstHandleRect() const;
    QRectF secondHandleRect() const;
    QRectF handleRect(int value) const;

private:
    Q_DISABLE_COPY(RangeSlider)
    int validLength() const;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(RangeSlider::Options)

} // namespace vcl::qt

#endif // VCL_QT_GUI_RANGE_SLIDER_H
