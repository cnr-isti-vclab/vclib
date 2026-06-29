// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_FLOAT_SLIDER_H
#define VCL_QT_GUI_FLOAT_SLIDER_H

#include <QSlider>

namespace vcl::qt {

class FloatSlider : public QSlider
{
    Q_OBJECT

    const float STEP = 0.01f;

public:
    FloatSlider(float step, QWidget* parent = nullptr) :
            QSlider(parent), STEP(step)
    {
        connect(
            dynamic_cast<QSlider*>(this),
            SIGNAL(valueChanged(int)),
            this,
            SLOT(notifyValueChanged(int)));
    }

    FloatSlider(QWidget* parent = nullptr) : FloatSlider(0.01f, parent) {}

    float step() const { return STEP; }

    float minimum() const { return QSlider::minimum() * STEP; }

    float maximum() const { return QSlider::maximum() * STEP; }

    float value() const { return QSlider::value() * STEP; }

    void setMinimum(float min)
    {
        QSlider::setMinimum(static_cast<int>(min / STEP));
    }

    void setMaximum(float max)
    {
        QSlider::setMaximum(static_cast<int>(max / STEP));
    }

public slots:

    void setValue(float value)
    {
        QSlider::setValue(static_cast<int>(value / STEP));
    }

    void setRange(float min, float max)
    {
        QSlider::setRange(
            static_cast<int>(min / STEP), static_cast<int>(max / STEP));
    }

signals:
    void valueChanged(float value);

private slots:

    void notifyValueChanged(int value)
    {
        float floatValue = value * STEP;
        emit valueChanged(floatValue);
    }
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_FLOAT_SLIDER_H
