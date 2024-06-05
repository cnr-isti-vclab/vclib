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

#include <vclib/ext/qt/gui/text_edit_logger/progress_io_device.h>

#include "ui_text_edit_logger.h"

namespace vcl::qt {

TextEditLogger::TextEditLogger(QWidget* parent) :
        QFrame(parent), ui(new Ui::TextEditLogger)
{
    ui->setupUi(this);
    ui->textEdit->setFontFamily("Monospace");
    ui->textEdit->setFontPointSize(8);

    mTextStream = new QTextStream(new ProgressIoDevice(ui->textEdit, this));
}

QTextStream* TextEditLogger::levelStream(LogLevel lvl)
{
    return mTextStream;
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
    delete ui;
    delete mTextStream;
}

} // namespace vcl::qt
