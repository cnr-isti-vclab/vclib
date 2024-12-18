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

#ifndef VCL_QT_GUI_TEXT_EDIT_LOGGER_ERROR_IO_DEVICE_H
#define VCL_QT_GUI_TEXT_EDIT_LOGGER_ERROR_IO_DEVICE_H

#include <QIODevice>
#include <QTextEdit>

namespace vcl::qt {

class ErrorIODevice : public QIODevice
{
    Q_OBJECT

    QTextEdit* mTextEdit;

public:
    ErrorIODevice(QTextEdit* textEdit, QObject* parent) :
            QIODevice(parent), mTextEdit(textEdit)
    {
        open(QIODevice::WriteOnly | QIODevice::Text);
    }

protected:
    qint64 readData(char* data, qint64 maxSize) { return 0; }

    qint64 writeData(const char* data, qint64 maxSize)
    {
        if (mTextEdit) {
            QColor oldColor = mTextEdit->textColor();
            mTextEdit->setTextColor(Qt::red);
            mTextEdit->append(data);
            mTextEdit->setTextColor(oldColor);
        }
        return maxSize;
    }
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_TEXT_EDIT_LOGGER_ERROR_IO_DEVICE_H
