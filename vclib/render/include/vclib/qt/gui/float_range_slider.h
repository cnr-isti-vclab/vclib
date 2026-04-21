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

#ifndef VCL_QT_GUI_FLOAT_RANGE_SLIDER_H
#define VCL_QT_GUI_FLOAT_RANGE_SLIDER_H

#include "range_slider.h"

namespace vcl::qt {

class FloatRangeSlider : public RangeSlider
{
    Q_OBJECT

    const float STEP = 0.01f;

public:
    FloatRangeSlider(float step, QWidget* parent = nullptr) :
            RangeSlider(parent), STEP(step)
    {
        connect(
            dynamic_cast<RangeSlider*>(this),
            SIGNAL(lowerValueChanged(int)),
            this,
            SLOT(notifyLowerValueChanged(int)));
        connect(
            dynamic_cast<RangeSlider*>(this),
            SIGNAL(upperValueChanged(int)),
            this,
            SLOT(notifyUpperValueChanged(int)));
    }

    FloatRangeSlider(QWidget* parent = nullptr) :
            FloatRangeSlider(0.01f, parent)
    {
    }

    float step() const { return STEP; }

    float minimum() const { return RangeSlider::minimum() * STEP; }

    float maximum() const { return RangeSlider::maximum() * STEP; }

    float lowerValue() const { return RangeSlider::lowerValue() * STEP; }

    float upperValue() const { return RangeSlider::upperValue() * STEP; }

    void setMinimum(float min)
    {
        RangeSlider::setMinimum(static_cast<int>(min / STEP));
    }

    void setMaximum(float max)
    {
        RangeSlider::setMaximum(static_cast<int>(max / STEP));
    }

public slots:

    void setLowerValue(float lowerValue)
    {
        RangeSlider::setLowerValue(static_cast<int>(lowerValue / STEP));
    }

    void setUpperValue(float upperValue)
    {
        RangeSlider::setUpperValue(static_cast<int>(upperValue / STEP));
    }

    void setRange(float min, float max)
    {
        RangeSlider::setRange(
            static_cast<int>(min / STEP), static_cast<int>(max / STEP));
    }

signals:
    void lowerValueChanged(float lowerValue);
    void upperValueChanged(float upperValue);

private slots:

    void notifyLowerValueChanged(int value)
    {
        float floatValue = value * STEP;
        emit lowerValueChanged(floatValue);
    }

    void notifyUpperValueChanged(int value)
    {
        float floatValue = value * STEP;
        emit upperValueChanged(floatValue);
    }
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_FLOAT_RANGE_SLIDER_H
