// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_TEXT_EDIT_LOGGER_H
#define VCL_QT_GUI_TEXT_EDIT_LOGGER_H

#include <vclib/base.h>
#include <vclib/qt/gui/text_edit_logger/debug_io_device.h>

#include <QFrame>

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
    QTextStream* levelStream(LogLevel lvl) const override;

    void alignLeft(QTextStream& stream) const override;

    void alignRight(QTextStream& stream) const override;

    void setWidth(QTextStream& stream, uint width) const override;

    void flush(QTextStream& stream) const override;
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_TEXT_EDIT_LOGGER_H
