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

#include <QComboBox>
#include <QFileDialog>

#include <vclib/ext/qt/utils/file_format.h>
#include <vclib/processing/action_manager.h>

#include "parameters_frame.h"

namespace vcl::qt {

template<typename IOAction, bool OPEN = true>
class ActionFileDialog : public QFileDialog
{
    const proc::IOActionManager<IOAction>* mActionManager  = nullptr;

    std::vector<ParametersFrame*> mParameterFrames;

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
            // allow multiple files selection
            setFileMode(QFileDialog::ExistingFiles);
        }
        else {
            setAcceptMode(QFileDialog::AcceptSave);
        }

        std::vector<proc::FileFormat> formats = actionManager.formats();

        bool allSupportedFormats = OPEN;
        QString filter = filterFormatsToQString(formats, allSupportedFormats);
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

        QFrame* frame = new QFrame(this);
        frame->setLayout(new QBoxLayout(QBoxLayout::TopToBottom, frame));

        for (const auto& format : formats) {
            auto* pf = new ParametersFrame(this);
            pf->setHeaderLabel(format.description() + " Parameters: ");
            pf->setParameters(actionManager.get(format)->parameters());
            frame->layout()->addWidget(pf);
            pf->setVisible(pf->parameters().size() > 0);
            mParameterFrames.push_back(pf);
        }

        if constexpr (!OPEN) {
            mParameterFrames[0]->setVisible(
                mParameterFrames[0]->parameters().size() > 0);
            for (uint i = 1; i < mParameterFrames.size(); i++) {
                mParameterFrames[i]->setVisible(false);
            }
        }

        layout->addWidget(frame, 2, 0, 1, 3);

        layout->addWidget(w20, 3, 0);
        layout->addWidget(w21, 3, 1);
        layout->addWidget(w22, 3, 2, 2, 1);
        layout->addWidget(w30, 4, 0);
        layout->addWidget(w31, 4, 1);

        QComboBox* cb = qobject_cast<QComboBox*>(w31);

        connect(cb, &QComboBox::currentIndexChanged, [&](int index) {
            if constexpr (OPEN) {
                if (index == 0) {
                    for (uint i = 0; i < mParameterFrames.size(); i++) {
                        uint np = mParameterFrames[i]->parameters().size();
                        mParameterFrames[i]->setVisible(np > 0);
                    }
                    return;
                }
            }
            uint i = OPEN ? 1 : 0;
            for (auto* pf : mParameterFrames) {
                pf->setVisible(i == index && pf->parameters().size() > 0);
                i++;
            }
        });

        resize(sizeHint().width(), sizeHint().height());
    }

    ~ActionFileDialog() {}

    proc::ParameterVector parameters() const
    {
        QGridLayout* layout = gLayout();
        QWidget*     w31    = layout->itemAtPosition(4, 1)->widget();
        QComboBox*   cb     = qobject_cast<QComboBox*>(w31);
        int ci = cb->currentIndex();
        if (OPEN && ci == 0) {
            assert(0);
        }
        else {
            if (OPEN) ci--;
            auto* pf = mParameterFrames[ci];
            return pf->parameters();
        }
    }

    proc::ParameterVector parametes(const proc::FileFormat& format) const
    {
        for (int i = 0; i < mActionManager->formats().size(); i++) {
            if (mActionManager->formats()[i] == format) {
                auto* pf = mParameterFrames[i];
                return pf->parameters();
            }
        }
        assert(0);
    }

    /**
     * @brief Will return the selected file format, or a format with empty
     * extension and description if the user selected "All Files".
     *
     * @return
     */
    proc::FileFormat selectedFormat() const
    {
        QGridLayout* layout = gLayout();
        QWidget*     w31    = layout->itemAtPosition(4, 1)->widget();
        QComboBox*   cb     = qobject_cast<QComboBox*>(w31);
        int ci = cb->currentIndex();
        if (OPEN && ci == 0)
            return proc::FileFormat("", "");
        else if (OPEN)
            return mActionManager->formats()[cb->currentIndex() - 1];
        else
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
