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

#include <vclib/ext/qt/gui/processing/parameters/parameter_row.h>

namespace vcl::qt {

ParameterRow::ParameterRow(const proc::Parameter& param) :
        mVisible(true), mHelpVisible(false)
{
    QString dl = param.description().c_str();
    QString tt = param.tooltip().c_str();

    mDescriptionLabel = new QClickableLabel();
    mDescriptionLabel->setText(dl);
    mDescriptionLabel->setToolTip(tt);
    mDescriptionLabel->setSizePolicy(
        QSizePolicy::Fixed, QSizePolicy::Preferred);

    mHelpLabel = new QLabel("<small>" + tt + "</small>");
    mHelpLabel->setTextFormat(Qt::RichText);
    mHelpLabel->setWordWrap(true);
    mHelpLabel->setVisible(false);
    mHelpLabel->setSizePolicy(
        QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    mHelpLabel->setMinimumWidth(250);
}

ParameterRow::~ParameterRow()
{
}

void ParameterRow::addRowToGridLayout(QGridLayout* lay, const int row)
{
    if (lay != NULL) {
        lay->addWidget(mDescriptionLabel, row, 0, 1, 1, Qt::AlignRight);
        lay->addWidget(parameterWidget(), row, 1);
        lay->addWidget(mHelpLabel, row, 2);
    }
}

bool ParameterRow::hasBeenModified() const
{
    return mHasBeenModified;
}

void ParameterRow::setVisible(bool b)
{
    mVisible = b;
    mDescriptionLabel->setVisible(b);
    parameterWidget()->setVisible(b);
    if (b && mHelpVisible)
        mHelpLabel->setVisible(true);
    else if (!b)
        mHelpLabel->setVisible(false);
}

void ParameterRow::setHelpVisible(bool b)
{
    mHelpVisible = b;
    mHelpLabel->setVisible(mVisible && mHelpVisible);
}

void ParameterRow::setModified(bool b)
{
    mHasBeenModified = b;
}

} // namespace vcl::qt
