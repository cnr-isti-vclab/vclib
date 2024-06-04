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

#ifndef VCL_EXT_QT_GUI_TEXT_EDIT_LOGGER_H
#define VCL_EXT_QT_GUI_TEXT_EDIT_LOGGER_H

#include <QIODevice>
#include <QPointer>
#include <QTextEdit>

#include <vclib/misc/logger/logger.h>

namespace vcl::qt {

namespace detail {

class TextEditIoDevice : public QIODevice
{
    Q_OBJECT

public:
    TextEditIoDevice(QTextEdit* textEdit, QObject* parent) :
            QIODevice(parent), textEdit(textEdit)
    {
        open(QIODevice::WriteOnly | QIODevice::Text);
    }

    //...

protected:
    qint64 readData(char* data, qint64 maxSize) { return 0; }

    qint64 writeData(const char* data, qint64 maxSize)
    {
        if (textEdit) {
            textEdit->append(data);
        }
        return maxSize;
    }

private:
    QPointer<QTextEdit> textEdit;
};

} // namespace detail

class TextEditLogger : public QTextEdit, public vcl::Logger<QTextStream>
{
    Q_OBJECT

    QTextStream mStream = QTextStream(new detail::TextEditIoDevice(this, this));

public:
    using Logger<QTextStream>::startTimer;

    TextEditLogger(QWidget* parent) : QTextEdit(parent)
    {
        QTextEdit::setReadOnly(true);
        QTextEdit::setFont(QFont("Monospace", 8));
    }

protected:
    QTextStream* levelStream(LogLevel lvl) override { return &mStream; }

    void alignLeft(QTextStream& stream) override
    {
        stream.setFieldAlignment(QTextStream::AlignLeft);
    }

    void alignRight(QTextStream& stream) override
    {
        stream.setFieldAlignment(QTextStream::AlignRight);
    }

    void setWidth(QTextStream& stream, uint width) override
    {
        stream.setFieldWidth(width);
    }

    void flush(QTextStream& stream) override
    {
        stream.flush();
    }
};

} // namespace vcl::qt

#endif // VCL_EXT_QT_GUI_TEXT_EDIT_LOGGER_H
