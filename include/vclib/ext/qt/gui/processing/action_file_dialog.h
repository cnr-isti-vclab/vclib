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

#ifndef VCL_EXT_QT_GUI_PROCESSING_ACTION_FILE_DIALOG_H
#define VCL_EXT_QT_GUI_PROCESSING_ACTION_FILE_DIALOG_H

#include <QFileDialog>
#include <QComboBox>

#include <vclib/ext/qt/utils/file_format.h>
#include <vclib/processing/action_manager.h>


#include "parameters_frame.h"

namespace vcl::qt {

template<typename IOAction, bool OPEN = true>
class ActionFileDialog : public QFileDialog
{
    const proc::IOActionManager<IOAction>* mActionManager = nullptr;
    ParametersFrame* mParameterFrame = nullptr;

public:
    explicit ActionFileDialog(
        const proc::IOActionManager<IOAction>& actionManager,
        const QString&                         caption,
        const QString&                         directory,
        QWidget*                               parent = nullptr) :
            QFileDialog(parent, caption, directory),
            mActionManager(&actionManager)
    {
        if constexpr (OPEN) {
            setAcceptMode(QFileDialog::AcceptOpen);
        }
        else {
            setAcceptMode(QFileDialog::AcceptSave);
        }

        std::vector<proc::FileFormat> formats = actionManager.formats();
        QString                       filter  = filterFormatsToQString(formats);
        setNameFilter(filter);

        setOption(QFileDialog::DontUseNativeDialog);
        QGridLayout* layout = gLayout();

        QWidget* w20 = layout->itemAtPosition(2, 0)->widget();
        QWidget* w21 = layout->itemAtPosition(2, 1)->widget();
        QWidget* w22 = layout->itemAtPosition(2, 2)->widget();
        QWidget* w30 = layout->itemAtPosition(3, 0)->widget();
        QWidget* w31 = layout->itemAtPosition(3, 1)->widget();

        layout->removeWidget(w20);
        layout->removeWidget(w21);
        layout->removeWidget(w22);
        layout->removeWidget(w30);
        layout->removeWidget(w31);

        mParameterFrame = new ParametersFrame(this);

        mParameterFrame->setParameters(
            actionManager.get(formats.front())->parameters());

        layout->addWidget(mParameterFrame, 2, 0, 1, 3);

        layout->addWidget(w20, 3, 0);
        layout->addWidget(w21, 3, 1);
        layout->addWidget(w22, 3, 2, 2, 1);
        layout->addWidget(w30, 4, 0);
        layout->addWidget(w31, 4, 1);

        QComboBox* cb = qobject_cast<QComboBox*>(w31);

        connect(cb, &QComboBox::currentIndexChanged, [&](int index) {
            auto format = mActionManager->formats()[index];
            auto params = mActionManager->get(format)->parameters();
            mParameterFrame->setParameters(params);
        });
    }

    ~ActionFileDialog() {}

    proc::ParameterVector parameters() const
    {
        return mParameterFrame->parameters();
    }

    proc::FileFormat selectedFormat() const
    {
        QGridLayout* layout = gLayout();
        QWidget* w31 = layout->itemAtPosition(4, 1)->widget();
        QComboBox* cb = qobject_cast<QComboBox*>(w31);
        return mActionManager->formats()[cb->currentIndex()];
    }

private:
    QGridLayout* gLayout() const
    {
        return qobject_cast<QGridLayout*>(this->layout());
    }
};

template<typename IOAction>
using ActionOpenFileDialog = ActionFileDialog<IOAction, true>;

template<typename IOAction>
using ActionSaveFileDialog = ActionFileDialog<IOAction, false>;

} // namespace vcl::qt

#endif // VCL_EXT_QT_GUI_PROCESSING_ACTION_FILE_DIALOG_H
