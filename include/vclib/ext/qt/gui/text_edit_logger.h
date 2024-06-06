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

#include <QFrame>

#include <vclib/misc/logger/logger.h>

#include <vclib/ext/qt/gui/text_edit_logger/debug_io_device.h>

namespace vcl::qt {

namespace Ui {
class TextEditLogger;
} // namespace Ui

class TextEditLogger : public QFrame, public vcl::Logger<QTextStream>
{
    Q_OBJECT

    Ui::TextEditLogger* mUI;

    DebugIODevice* mDebugIoDevice;

    QTextStream* mDebugStream;
    QTextStream* mErrorStream;
    QTextStream* mMsgStream;
    QTextStream* mWarningStream;

public:
    using Logger<QTextStream>::startTimer;

    explicit TextEditLogger(QWidget* parent = nullptr);
    ~TextEditLogger();

    TextEditLogger(const TextEditLogger&)            = delete;
    TextEditLogger& operator=(const TextEditLogger&) = delete;

    void enableDebugLogging(bool enable);

    void setPercentage(uint newPerc) override;

protected:
    QTextStream* levelStream(LogLevel lvl) override;

    void alignLeft(QTextStream& stream) override;

    void alignRight(QTextStream& stream) override;

    void setWidth(QTextStream& stream, uint width) override;

    void flush(QTextStream& stream) override;
};

} // namespace vcl::qt

#endif // VCL_EXT_QT_GUI_TEXT_EDIT_LOGGER_H
