// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/text_edit_logger.h>

#include "ui_text_edit_logger.h"

namespace vcl::qt {

TextEditLogger::TextEditLogger(QWidget* parent) :
        QFrame(parent), mUI(new Ui::TextEditLogger)
{
    mUI->setupUi(this);
    mUI->textEdit->setFontFamily("Monospace");
    mUI->textEdit->setFontPointSize(8);

    disablePrintPercentage();
    disablePrintMessageDuringProgress();
    setPrintLevel(LogLevel::MESSAGE_LOG);
}

void TextEditLogger::enableDebugLogging(bool enable)
{
    mDebugEnabled = enable;
}

void TextEditLogger::setPercentage(uint newPerc)
{
    vcl::Logger::setPercentage(newPerc);
    mUI->progressBar->setValue(percentage());
    mUI->progressBar->update();
}

void TextEditLogger::write(std::string_view line, LogLevel lvl) const
{
    if (lvl == LogLevel::DEBUG_LOG && !mDebugEnabled) {
        return;
    }

    if (!mUI || !mUI->textEdit) {
        return;
    }

    QColor oldColor = mUI->textEdit->textColor();
    switch (lvl) {
    case LogLevel::DEBUG_LOG:
        mUI->textEdit->setTextColor(Qt::blue);
        break;
    case LogLevel::ERROR_LOG:
        mUI->textEdit->setTextColor(Qt::red);
        break;
    case LogLevel::WARNING_LOG:
        mUI->textEdit->setTextColor(Qt::darkYellow);
        break;
    default:
        break;
    }

    mUI->textEdit->append(
        QString::fromUtf8(line.data(), static_cast<qsizetype>(line.size())));
    mUI->textEdit->setTextColor(oldColor);
}

TextEditLogger::~TextEditLogger()
{
    delete mUI;
}

} // namespace vcl::qt
