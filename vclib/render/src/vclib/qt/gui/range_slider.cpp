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

#include <vclib/qt/gui/range_slider.h>

namespace vcl::qt {

static const int HANDLE_SIDE_LENGTH = 11;
static const int SLIDER_BAR_HEIGHT  = 5;
static const int LEFT_RIGHT_MARGIN  = 1;

RangeSlider::RangeSlider(Qt::Orientation ori, Options t, QWidget* parent) :
        QWidget(parent), mOrientation(ori), mType(t)
{
    setMouseTracking(true);
}

RangeSlider::RangeSlider(QWidget* parent) :
        RangeSlider(Qt::Horizontal, DoubleHandles, parent)
{
}

QSize RangeSlider::minimumSizeHint() const
{
    return QSize(
        HANDLE_SIDE_LENGTH * 2 + LEFT_RIGHT_MARGIN * 2, HANDLE_SIDE_LENGTH);
}

int RangeSlider::minimum() const
{
    return mMinimum;
}

int RangeSlider::maximum() const
{
    return mMaximum;
}

int RangeSlider::lowerValue() const
{
    return mLowerValue;
}

int RangeSlider::upperValue() const
{
    return mUpperValue;
}

void RangeSlider::setMinimum(int min)
{
    setRange(min, mMaximum);
}

void RangeSlider::setMaximum(int max)
{
    setRange(mMinimum, max);
}

void RangeSlider::setLowerValue(int lowerValue)
{
    if (lowerValue > mMaximum) {
        lowerValue = mMaximum;
    }

    if (lowerValue < mMinimum) {
        lowerValue = mMinimum;
    }

    mLowerValue = lowerValue;
    emit lowerValueChanged(mLowerValue);

    update();
}

void RangeSlider::setUpperValue(int upperValue)
{
    if (upperValue > mMaximum) {
        upperValue = mMaximum;
    }

    if (upperValue < mMinimum) {
        upperValue = mMinimum;
    }

    mUpperValue = upperValue;
    emit upperValueChanged(mUpperValue);

    update();
}

void RangeSlider::setRange(int min, int max)
{
    if (min <= max) {
        mMinimum = min;
        mMaximum = max;
    }
    else {
        mMinimum = max;
        mMaximum = min;
    }
    mInterval = mMaximum - mMinimum;
    update();

    setLowerValue(mMinimum);
    setUpperValue(mMaximum);

    emit rangeChanged(mMinimum, mMaximum);
}

void RangeSlider::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    const QPalette&            pal = palette();
    const QPalette::ColorGroup group =
        isEnabled() ? QPalette::Active : QPalette::Disabled;

    // Background
    QRectF backgroundRect;
    if (mOrientation == Qt::Horizontal)
        backgroundRect = QRectF(
            LEFT_RIGHT_MARGIN,
            (height() - SLIDER_BAR_HEIGHT) / 2,
            width() - LEFT_RIGHT_MARGIN * 2,
            SLIDER_BAR_HEIGHT);
    else
        backgroundRect = QRectF(
            (width() - SLIDER_BAR_HEIGHT) / 2,
            LEFT_RIGHT_MARGIN,
            SLIDER_BAR_HEIGHT,
            height() - LEFT_RIGHT_MARGIN * 2);

    QPen pen(pal.color(group, QPalette::Mid), 0.8);
    painter.setPen(pen);
    QBrush backgroundBrush(pal.color(group, QPalette::Midlight));
    painter.setBrush(backgroundBrush);
    painter.drawRoundedRect(backgroundRect, 1, 1);

    // First value handle rect
    pen.setColor(pal.color(group, QPalette::Shadow));
    pen.setWidth(0);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing);
    QBrush handleBrush(pal.color(group, QPalette::Button));
    painter.setBrush(handleBrush);
    QRectF leftHandleRect = firstHandleRect();
    if (mType.testFlag(LeftHandle))
        painter.drawRoundedRect(leftHandleRect, 2, 2);

    // Second value handle rect
    QRectF rightHandleRect = secondHandleRect();
    if (mType.testFlag(RightHandle))
        painter.drawRoundedRect(rightHandleRect, 2, 2);

    // Selected range
    painter.setRenderHint(QPainter::Antialiasing, false);
    QRectF selectedRect(backgroundRect);
    if (mOrientation == Qt::Horizontal) {
        selectedRect.setLeft(
            (mType.testFlag(LeftHandle) ? leftHandleRect.right() :
                                          leftHandleRect.left()) +
            0.5);
        selectedRect.setRight(
            (mType.testFlag(RightHandle) ? rightHandleRect.left() :
                                           rightHandleRect.right()) -
            0.5);
    }
    else {
        selectedRect.setTop(
            (mType.testFlag(LeftHandle) ? leftHandleRect.bottom() :
                                          leftHandleRect.top()) +
            0.5);
        selectedRect.setBottom(
            (mType.testFlag(RightHandle) ? rightHandleRect.top() :
                                           rightHandleRect.bottom()) -
            0.5);
    }
    QBrush selectedBrush(pal.color(group, QPalette::Highlight));
    painter.setBrush(selectedBrush);
    painter.drawRect(selectedRect);
}

void RangeSlider::mousePressEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton) {
        int posCheck, posMax, posValue, firstHandleRectPosValue,
            secondHandleRectPosValue;
        posCheck = (mOrientation == Qt::Horizontal) ? event->pos().y() :
                                                      event->pos().x();
        posMax   = (mOrientation == Qt::Horizontal) ? height() : width();
        posValue = (mOrientation == Qt::Horizontal) ? event->pos().x() :
                                                      event->pos().y();
        firstHandleRectPosValue  = (mOrientation == Qt::Horizontal) ?
                                       firstHandleRect().x() :
                                       firstHandleRect().y();
        secondHandleRectPosValue = (mOrientation == Qt::Horizontal) ?
                                       secondHandleRect().x() :
                                       secondHandleRect().y();

        mSecondHandlePressed = secondHandleRect().contains(event->pos());
        mFirstHandlePressed =
            !mSecondHandlePressed && firstHandleRect().contains(event->pos());
        if (mFirstHandlePressed) {
            mDelta =
                posValue - (firstHandleRectPosValue + HANDLE_SIDE_LENGTH / 2);
        }
        else if (mSecondHandlePressed) {
            mDelta =
                posValue - (secondHandleRectPosValue + HANDLE_SIDE_LENGTH / 2);
        }

        if (posCheck >= 2 && posCheck <= posMax - 2) {
            int step = mInterval / 10 < 1 ? 1 : mInterval / 10;
            if (posValue < firstHandleRectPosValue)
                setLowerValue(mLowerValue - step);
            else if (
                ((posValue > firstHandleRectPosValue + HANDLE_SIDE_LENGTH) ||
                 !mType.testFlag(LeftHandle)) &&
                ((posValue < secondHandleRectPosValue) ||
                 !mType.testFlag(RightHandle))) {
                if (mType.testFlag(DoubleHandles))
                    if (posValue -
                            (firstHandleRectPosValue + HANDLE_SIDE_LENGTH) <
                        (secondHandleRectPosValue -
                         (firstHandleRectPosValue + HANDLE_SIDE_LENGTH)) /
                            2)
                        setLowerValue(
                            (mLowerValue + step < mUpperValue) ?
                                mLowerValue + step :
                                mUpperValue);
                    else
                        setUpperValue(
                            (mUpperValue - step > mLowerValue) ?
                                mUpperValue - step :
                                mLowerValue);
                else if (mType.testFlag(LeftHandle))
                    setLowerValue(
                        (mLowerValue + step < mUpperValue) ?
                            mLowerValue + step :
                            mUpperValue);
                else if (mType.testFlag(RightHandle))
                    setUpperValue(
                        (mUpperValue - step > mLowerValue) ?
                            mUpperValue - step :
                            mLowerValue);
            }
            else if (posValue > secondHandleRectPosValue + HANDLE_SIDE_LENGTH)
                setUpperValue(mUpperValue + step);
        }
    }
}

void RangeSlider::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton) {
        int posValue, firstHandleRectPosValue, secondHandleRectPosValue;
        posValue = (mOrientation == Qt::Horizontal) ? event->pos().x() :
                                                      event->pos().y();
        firstHandleRectPosValue  = (mOrientation == Qt::Horizontal) ?
                                       firstHandleRect().x() :
                                       firstHandleRect().y();
        secondHandleRectPosValue = (mOrientation == Qt::Horizontal) ?
                                       secondHandleRect().x() :
                                       secondHandleRect().y();

        if (mFirstHandlePressed && mType.testFlag(LeftHandle)) {
            if (posValue - mDelta + HANDLE_SIDE_LENGTH / 2 <=
                secondHandleRectPosValue) {
                setLowerValue(
                    (posValue - mDelta - LEFT_RIGHT_MARGIN -
                     HANDLE_SIDE_LENGTH / 2) *
                        1.0 / validLength() * mInterval +
                    mMinimum);
            }
            else {
                setLowerValue(mUpperValue);
            }
        }
        else if (mSecondHandlePressed && mType.testFlag(RightHandle)) {
            if (firstHandleRectPosValue +
                    HANDLE_SIDE_LENGTH *
                        (mType.testFlag(DoubleHandles) ? 1.5 : 0.5) <=
                posValue - mDelta) {
                setUpperValue(
                    (posValue - mDelta - LEFT_RIGHT_MARGIN -
                     HANDLE_SIDE_LENGTH / 2 -
                     (mType.testFlag(DoubleHandles) ? HANDLE_SIDE_LENGTH : 0)) *
                        1.0 / validLength() * mInterval +
                    mMinimum);
            }
            else {
                setUpperValue(mLowerValue);
            }
        }
    }
}

void RangeSlider::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);

    mFirstHandlePressed  = false;
    mSecondHandlePressed = false;
}

void RangeSlider::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::EnabledChange ||
        event->type() == QEvent::PaletteChange) {
        update();
    }
}

QRectF RangeSlider::firstHandleRect() const
{
    float percentage = (mLowerValue - mMinimum) * 1.0 / mInterval;
    return handleRect(percentage * validLength() + LEFT_RIGHT_MARGIN);
}

QRectF RangeSlider::secondHandleRect() const
{
    float percentage = (mUpperValue - mMinimum) * 1.0 / mInterval;
    return handleRect(
        percentage * validLength() + LEFT_RIGHT_MARGIN +
        (mType.testFlag(LeftHandle) ? HANDLE_SIDE_LENGTH : 0));
}

QRectF RangeSlider::handleRect(int value) const
{
    if (mOrientation == Qt::Horizontal)
        return QRect(
            value,
            (height() - HANDLE_SIDE_LENGTH) / 2,
            HANDLE_SIDE_LENGTH,
            HANDLE_SIDE_LENGTH);
    else
        return QRect(
            (width() - HANDLE_SIDE_LENGTH) / 2,
            value,
            HANDLE_SIDE_LENGTH,
            HANDLE_SIDE_LENGTH);
}

int RangeSlider::validLength() const
{
    int len = (mOrientation == Qt::Horizontal) ? width() : height();
    return len - LEFT_RIGHT_MARGIN * 2 -
           HANDLE_SIDE_LENGTH * (mType.testFlag(DoubleHandles) ? 2 : 1);
}

} // namespace vcl::qt
