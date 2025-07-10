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

#include "../lines_common.h"

#include <vclib/qt/viewer_widget.h>

#include <QComboBox>
#include <QVBoxLayout>
#include <QApplication>

// a QWidget class having only a combo box with the list of available lines
class LinesComboBox : public QComboBox
{
public:
    LinesComboBox(QWidget* parent = nullptr) : QComboBox(parent)
    {
        addItem("CPUGeneratedLines");
    }
};


int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QWidget w;

    // add the viewer tw to the layout
    QVBoxLayout* layout = new QVBoxLayout(&w);

    // add the combo box to the layout
    LinesComboBox* cb = new LinesComboBox(&w);
    layout->addWidget(cb);

    vcl::qt::ViewerWidget* tw = new vcl::qt::ViewerWidget(&w);
    layout->addWidget(tw);

    const vcl::uint N_LINES = 100;

    std::shared_ptr<vcl::DrawableObjectVector> vec =
        std::make_shared<vcl::DrawableObjectVector>(getDrawableLines(N_LINES));

    tw->setDrawableObjectVector(vec);

    // connect the combo box signal of item changed to a lambda that sets
    // as visible the ith drawable object in the viewer widget

    QObject::connect(
        cb,
        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        [=](int index) {
            std::cerr << "Showing " << index << std::endl;
            for (vcl::uint i = 0; i < 5; ++i) {
                vec->at(i)->setVisibility(false);
            }
            vec->at(index)->setVisibility(true);

            tw->update();
        });

    w.resize(1024, 768);

    w.show();
    tw->setFocus();

    return app.exec();
}
