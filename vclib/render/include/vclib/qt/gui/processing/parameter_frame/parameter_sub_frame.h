/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_QT_GUI_PROCESSING_PARAMETER_FRAME_PARAMETER_SUB_FRAME_H
#define VCL_QT_GUI_PROCESSING_PARAMETER_FRAME_PARAMETER_SUB_FRAME_H

#include <QFrame>

#include <QGridLayout>

namespace vcl::qt {

namespace Ui {
class ParameterSubFrame;
} // namespace Ui

class ParameterSubFrame : public QFrame
{
    Q_OBJECT

    Ui::ParameterSubFrame* mUI;

public:
    explicit ParameterSubFrame(QWidget* parent = nullptr);
    ~ParameterSubFrame();

    void setTitleLabel(const std::string& title);

    void setSubFrameLayout(QGridLayout* layout);

    void setHeaderFrameVisible(bool visible);

    void setHeaderButtonVisible(bool visible);

    void setHeaderButtonChecked(bool checked);

private slots:
    void showPushButtonClicked(bool checked);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_PROCESSING_PARAMETER_FRAME_PARAMETER_SUB_FRAME_H
