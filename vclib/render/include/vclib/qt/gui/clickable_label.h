// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_CLICKABLE_LABEL_H
#define VCL_QT_GUI_CLICKABLE_LABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>

namespace vcl::qt {

/**
 * @brief The ClickableLabel class is a QLabel class which can be clicked
 * (clicked event).
 *
 * Usage: just Promote a QLabel to a vcl::qt::ClickableLabel
 *
 * @link https://wiki.qt.io/Clickable_QLabel
 */
class ClickableLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ClickableLabel(
        QWidget*        parent = Q_NULLPTR,
        Qt::WindowFlags f      = Qt::WindowFlags());
    ~ClickableLabel() = default;

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_CLICKABLE_LABEL_H
