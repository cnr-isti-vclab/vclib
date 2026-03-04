/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QVBoxLayout>

// a QWidget class having only a combo box with the list of available lines
class LinesComboBox : public QComboBox
{
public:
    LinesComboBox(QWidget* parent = nullptr) : QComboBox(parent)
    {
        addItems({"Primitive", "CPU Generated", "GPU Generated"});
    }
};

class ColorToUseComboBox : public QComboBox
{
public:
    ColorToUseComboBox(QWidget* parent = nullptr) : QComboBox(parent)
    {
        addItems({"Per Vertex", "Per Edge", "General"});
    }
};

std::shared_ptr<vcl::DrawableLines> getLines(
    std::shared_ptr<vcl::DrawableObjectVector> vec)
{
    std::shared_ptr<vcl::DrawableLines> lines =
        std::dynamic_pointer_cast<vcl::DrawableLines>(vec->at(0));

    return lines;
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QWidget w;

    // add the viewer tw to the layout
    QVBoxLayout* layout = new QVBoxLayout(&w);

    QCheckBox* indexedCB = new QCheckBox("Indexed", &w);
    layout->addWidget(indexedCB);

    // add the combo box to the layout
    LinesComboBox* lcb = new LinesComboBox(&w);
    layout->addWidget(lcb);

    ColorToUseComboBox* ccb = new ColorToUseComboBox(&w);
    layout->addWidget(ccb);

    QSlider* tslider = new QSlider();
    tslider->setOrientation(Qt::Orientation::Horizontal);
    tslider->setMinimum(1);
    tslider->setMaximum(100);
    tslider->setValue(5);
    layout->addWidget(tslider);

    vcl::qt::ViewerWidget* tw = new vcl::qt::ViewerWidget(&w);
    layout->addWidget(tw);

    const vcl::uint N_LINES = 100;

    std::shared_ptr<vcl::DrawableObjectVector> vec =
        std::make_shared<vcl::DrawableObjectVector>();

    vec->pushBack(std::move(getDrawableLines(N_LINES)));

    tw->setDrawableObjectVector(vec);
    tslider->setValue(getLines(vec)->thickness());

    QObject::connect(
        indexedCB, &QCheckBox::checkStateChanged, [=](Qt::CheckState state) {
            bool indexed = (state == Qt::CheckState::Checked);
            std::cerr << "Indexed: " << indexed << std::endl;

            vec->clear();
            vec->pushBack(std::move(getDrawableLines(N_LINES, indexed)));
            getLines(vec)->setColorToUse(
                (vcl::Lines::ColorToUse) ccb->currentIndex());
            getLines(vec)->thickness() = tslider->value();
            getLines(vec)->setImplementationType(
                (vcl::Lines::ImplementationType) lcb->currentIndex());
            tw->update();
        });

    QObject::connect(
        lcb,
        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        [=](int index) {
            using ImplementationType = vcl::Lines::ImplementationType;

            std::cerr << "Lines implementation: " << index << std::endl;

            getLines(vec)->setImplementationType((ImplementationType) index);
            tw->update();
        });

    QObject::connect(
        ccb,
        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        [=](int index) {
            using ColorToUse = vcl::Lines::ColorToUse;

            std::cerr << "Color to use: " << index << std::endl;

            getLines(vec)->setColorToUse((ColorToUse) index);
            tw->update();
        });

    QObject::connect(tslider, &QSlider::valueChanged, [=](int value) {
        std::cerr << "Thickness: " << value << std::endl;
        getLines(vec)->thickness() = (float) value;
        tw->update();
    });

    w.resize(1024, 768);

    w.show();

    return app.exec();
}
