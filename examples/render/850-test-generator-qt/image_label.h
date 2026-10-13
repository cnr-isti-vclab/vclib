// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCLIB_RENDER_EXAMPLES_TEST_GENERATOR_IMAGE_LABEL_H
#define VCLIB_RENDER_EXAMPLES_TEST_GENERATOR_IMAGE_LABEL_H

#include <QLabel>
#include <QMouseEvent>

class ImageLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ImageLabel(QWidget* parent = nullptr);
    ~ImageLabel() override = default;

    void setImage(const QImage& img);

signals:
    void mouseHovered(int x, int y);
    void mouseClicked(int x, int y, Qt::MouseButton button);

protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void    updateDisplay();
    QPixmap mOriginalPixmap;
};

#endif // VCLIB_RENDER_EXAMPLES_TEST_GENERATOR_IMAGE_LABEL_H
