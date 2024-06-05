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

#include <vclib/ext/qt/gui/text_edit_logger.h>

#include <vclib/ext/qt/gui/text_edit_logger/debug_io_device.h>
#include <vclib/ext/qt/gui/text_edit_logger/error_io_device.h>
#include <vclib/ext/qt/gui/text_edit_logger/message_io_device.h>
#include <vclib/ext/qt/gui/text_edit_logger/warning_io_device.h>

#include "ui_text_edit_logger.h"

namespace vcl::qt {

TextEditLogger::TextEditLogger(QWidget* parent) :
        QFrame(parent), mUI(new Ui::TextEditLogger)
{
    mUI->setupUi(this);
    mUI->textEdit->setFontFamily("Monospace");
    mUI->textEdit->setFontPointSize(8);

    mDebugIoDevice = new DebugIODevice(mUI->textEdit, this);

    mDebugStream = new QTextStream(mDebugIoDevice);
    mErrorStream = new QTextStream(new ErrorIODevice(mUI->textEdit, this));
    mMsgStream = new QTextStream(new MessageIODevice(mUI->textEdit, this));
    mWarningStream = new QTextStream(new WarningIODevice(mUI->textEdit, this));

    disablePrintPercentage();
    disablePrintMessageDuringProgress();
}


void TextEditLogger::enableDebugLogging(bool enable)
{
    if (enable) {
        mDebugIoDevice->enable();
    } else {
        mDebugIoDevice->disable();
    }
}

void TextEditLogger::setPercentage(uint newPerc)
{
    vcl::Logger<QTextStream>::setPercentage(newPerc);
    mUI->progressBar->setValue(percentage());
    mUI->progressBar->update();
}

QTextStream* TextEditLogger::levelStream(LogLevel lvl)
{
    switch (lvl) {
        case LogLevel::DEBUG:
            return mDebugStream;
        case LogLevel::ERROR:
            return mErrorStream;
        case LogLevel::MESSAGE:
        case LogLevel::PROGRESS:
            return mMsgStream;
        case LogLevel::WARNING:
            return mWarningStream;
    }
}

void TextEditLogger::alignLeft(QTextStream& stream)
{
    stream.setFieldAlignment(QTextStream::AlignLeft);
}

void TextEditLogger::alignRight(QTextStream& stream)
{
    stream.setFieldAlignment(QTextStream::AlignRight);
}

void TextEditLogger::setWidth(QTextStream& stream, uint width)
{
    stream.setFieldWidth(width);
}

void TextEditLogger::flush(QTextStream& stream)
{
    stream.flush();
}

TextEditLogger::~TextEditLogger()
{
    delete mUI;
    delete mMsgStream;
}

} // namespace vcl::qt
