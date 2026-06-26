// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/text_edit_logger.h>

#include "ui_text_edit_logger.h"

#include <vclib/qt/gui/text_edit_logger/debug_io_device.h>
#include <vclib/qt/gui/text_edit_logger/error_io_device.h>
#include <vclib/qt/gui/text_edit_logger/message_io_device.h>
#include <vclib/qt/gui/text_edit_logger/warning_io_device.h>

namespace vcl::qt {

TextEditLogger::TextEditLogger(QWidget* parent) :
        QFrame(parent), mUI(new Ui::TextEditLogger)
{
    mUI->setupUi(this);
    mUI->textEdit->setFontFamily("Monospace");
    mUI->textEdit->setFontPointSize(8);

    mDebugIoDevice = new DebugIODevice(mUI->textEdit, this);

    mDebugStream   = new QTextStream(mDebugIoDevice);
    mErrorStream   = new QTextStream(new ErrorIODevice(mUI->textEdit, this));
    mMsgStream     = new QTextStream(new MessageIODevice(mUI->textEdit, this));
    mWarningStream = new QTextStream(new WarningIODevice(mUI->textEdit, this));

    disablePrintPercentage();
    disablePrintMessageDuringProgress();
    setPrintLevel(LogLevel::MESSAGE_LOG);
}

void TextEditLogger::enableDebugLogging(bool enable)
{
    if (enable) {
        mDebugIoDevice->enable();
    }
    else {
        mDebugIoDevice->disable();
    }
}

void TextEditLogger::setPercentage(uint newPerc)
{
    vcl::Logger<QTextStream>::setPercentage(newPerc);
    mUI->progressBar->setValue(percentage());
    mUI->progressBar->update();
}

QTextStream* TextEditLogger::levelStream(LogLevel lvl) const
{
    switch (lvl) {
    case LogLevel::DEBUG_LOG: return mDebugStream;
    case LogLevel::ERROR_LOG: return mErrorStream;
    case LogLevel::MESSAGE_LOG:
    case LogLevel::PROGRESS_LOG: return mMsgStream;
    case LogLevel::WARNING_LOG: return mWarningStream;
    default: return mMsgStream;
    }
}

void TextEditLogger::alignLeft(QTextStream& stream) const
{
    stream.setFieldAlignment(QTextStream::AlignLeft);
}

void TextEditLogger::alignRight(QTextStream& stream) const
{
    stream.setFieldAlignment(QTextStream::AlignRight);
}

void TextEditLogger::setWidth(QTextStream& stream, uint width) const
{
    stream.setFieldWidth(width);
}

void TextEditLogger::flush(QTextStream& stream) const
{
    stream.flush();
}

TextEditLogger::~TextEditLogger()
{
    delete mUI;
    delete mMsgStream;
}

} // namespace vcl::qt
