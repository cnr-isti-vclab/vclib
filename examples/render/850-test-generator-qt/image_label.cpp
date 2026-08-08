// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "image_label.h"

ImageLabel::ImageLabel(QWidget* parent) : QLabel(parent)
{
    setMouseTracking(true);
}

void ImageLabel::setImage(const QImage& img)
{
    mOriginalPixmap = QPixmap::fromImage(img);
    updateDisplay();
}

void ImageLabel::updateDisplay()
{
    if (!mOriginalPixmap.isNull()) {
        setPixmap(mOriginalPixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void ImageLabel::resizeEvent(QResizeEvent* event)
{
    updateDisplay();
    QLabel::resizeEvent(event);
}

void ImageLabel::mouseMoveEvent(QMouseEvent* event)
{
    emit mouseHovered(event->position().x(), event->position().y());
    QLabel::mouseMoveEvent(event);
}

void ImageLabel::mousePressEvent(QMouseEvent* event)
{
    emit mouseClicked(event->position().x(), event->position().y(), event->button());
    QLabel::mousePressEvent(event);
}
