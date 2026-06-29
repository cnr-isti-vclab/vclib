// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_TEXT_EDIT_LOGGER_DEBUG_IO_DEVICE_H
#define VCL_QT_GUI_TEXT_EDIT_LOGGER_DEBUG_IO_DEVICE_H

#include <QIODevice>
#include <QTextEdit>

namespace vcl::qt {

class DebugIODevice : public QIODevice
{
    Q_OBJECT

    QTextEdit* mTextEdit;
    bool       mEnabled = false;

public:
    DebugIODevice(QTextEdit* textEdit, QObject* parent) :
            QIODevice(parent), mTextEdit(textEdit)
    {
        open(QIODevice::WriteOnly | QIODevice::Text);
    }

    void enable() { mEnabled = true; }

    void disable() { mEnabled = false; }

protected:
    qint64 readData(char* data, qint64 maxSize) { return 0; }

    qint64 writeData(const char* data, qint64 maxSize)
    {
        if (mTextEdit && mEnabled) {
            QColor oldColor = mTextEdit->textColor();
            mTextEdit->setTextColor(Qt::blue);
            mTextEdit->append(data);
            mTextEdit->setTextColor(oldColor);
        }
        return maxSize;
    }
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_TEXT_EDIT_LOGGER_DEBUG_IO_DEVICE_H
