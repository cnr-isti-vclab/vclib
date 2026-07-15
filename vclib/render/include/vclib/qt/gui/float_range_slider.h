// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_FLOAT_RANGE_SLIDER_H
#define VCL_QT_GUI_FLOAT_RANGE_SLIDER_H

#include "range_slider.h"

namespace vcl::qt {

class FloatRangeSlider : public RangeSlider
{
    Q_OBJECT

    float mStep = 0.01f;

public:
    FloatRangeSlider(float step, QWidget* parent = nullptr) :
            RangeSlider(parent), mStep(step)
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

    float step() const { return mStep; }
    
    void setStep(float step)
    {
        if (step <= 0.0f) return;
        
        float currMin = minimum();
        float currMax = maximum();
        float currLower = lowerValue();
        float currUpper = upperValue();
        
        mStep = step;
        
        this->blockSignals(true);
        setRange(currMin, currMax);
        setLowerValue(currLower);
        setUpperValue(currUpper);
        this->blockSignals(false);
    }

    float minimum() const { return RangeSlider::minimum() * mStep; }

    float maximum() const { return RangeSlider::maximum() * mStep; }

    float lowerValue() const { return RangeSlider::lowerValue() * mStep; }

    float upperValue() const { return RangeSlider::upperValue() * mStep; }

    void setMinimum(float min)
    {
        RangeSlider::setMinimum(static_cast<int>(min / mStep));
    }

    void setMaximum(float max)
    {
        RangeSlider::setMaximum(static_cast<int>(max / mStep));
    }

public slots:

    void setLowerValue(float lowerValue)
    {
        RangeSlider::setLowerValue(static_cast<int>(lowerValue / mStep));
    }

    void setUpperValue(float upperValue)
    {
        RangeSlider::setUpperValue(static_cast<int>(upperValue / mStep));
    }

    void setRange(float min, float max)
    {
        RangeSlider::setRange(
            static_cast<int>(min / mStep), static_cast<int>(max / mStep));
    }

signals:
    void lowerValueChanged(float lowerValue);
    void upperValueChanged(float upperValue);

private slots:

    void notifyLowerValueChanged(int value)
    {
        float floatValue = value * mStep;
        emit lowerValueChanged(floatValue);
    }

    void notifyUpperValueChanged(int value)
    {
        float floatValue = value * mStep;
        emit upperValueChanged(floatValue);
    }
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_FLOAT_RANGE_SLIDER_H
