/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_EXT_QT_BGFX_CANVAS_WINDOW_H
#define VCL_EXT_QT_BGFX_CANVAS_WINDOW_H

#include <QApplication>
#include <QWindow>

#include <vclib/ext/bgfx/canvas.h>

namespace vcl::qbgf {

class CanvasWindow : public QWindow, public vcl::bgf::Canvas
{
    Q_OBJECT

    using Canvas = vcl::bgf::Canvas;

public:
    CanvasWindow(
        const std::string& windowTitle,
        uint     width  = 1024,
        uint     height = 768,
        QWindow* parent = nullptr);

    CanvasWindow(
        uint     width  = 1024,
        uint     height = 768,
        QWindow* parent = nullptr);

    CanvasWindow(
        QWindow* parent);

    virtual ~CanvasWindow();

    const std::string windowTitle() const;

    void setWindowTitle(const std::string& title);

    virtual void draw(uint viewID) override;

    virtual void onResize(unsigned int w, unsigned int h);

    void update();

protected:
    bool event(QEvent* event) override;

    void paintEvent(QPaintEvent* event) override;

    void resizeEvent(QResizeEvent* event) override;

private:
    void paint();
};

} // namespace vcl::qbgf

#endif // VCL_EXT_QT_BGFX_CANVAS_WINDOW_H
