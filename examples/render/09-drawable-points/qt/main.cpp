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

#include <vclib/bgfx/drawable/drawable_points.h>

#include <vclib/qt/viewer_widget.h>

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <vclib/algorithms/core/random.h>

std::shared_ptr<vcl::DrawablePoints> getDrawablePoints(vcl::uint nPoints)
{
    auto points = std::make_shared<vcl::DrawablePoints>();

    std::vector<vcl::Point3d> positions(nPoints);
    std::vector<vcl::Color> colors(nPoints);

    for (vcl::uint i = 0; i < nPoints; ++i) {
        positions[i] = vcl::random<vcl::Point3d>();
        colors[i] = vcl::Color(
            vcl::random<uint8_t>(),
            vcl::random<uint8_t>(),
            vcl::random<uint8_t>()
        );
    }

    points->setSize(10);

    points->setVertices(positions);
    points->setVertexColors(colors);

    points->setColorSetting(vcl::Points::ColorSetting::PER_VERTEX);
    points->setShading(vcl::Points::Shading::NONE);
    points->setShape(vcl::Points::Shape::SQUARE);
    points->setGeneralColor(vcl::Color::Magenta);

    return points;
}

class ColorToUseComboBox : public QComboBox
{
public:
    ColorToUseComboBox(QWidget* parent = nullptr) : QComboBox(parent)
    {
        addItems({"Per Vertex", "General"});
    }
};

std::shared_ptr<vcl::DrawablePoints> getPoints(
    std::shared_ptr<vcl::DrawableObjectVector> vec)
{
    std::shared_ptr<vcl::DrawablePoints> lines =
        std::dynamic_pointer_cast<vcl::DrawablePoints>(vec->at(0));

    return lines;
}

int main(int argc, char** argv)
{
    auto app = vcl::qt::qAppl(argc, argv);

    QWidget w;

    // add the viewer tw to the layout
    QVBoxLayout* layout = new QVBoxLayout(&w);

    ColorToUseComboBox* ccb = new ColorToUseComboBox(&w);
    layout->addWidget(ccb);

    QSlider* tslider = new QSlider();
    tslider->setOrientation(Qt::Orientation::Horizontal);
    tslider->setMinimum(1);
    tslider->setMaximum(100);
    tslider->setValue(5);
    layout->addWidget(tslider);

    QGroupBox* shapeGroup = new QGroupBox("Shape");
    shapeGroup->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    QHBoxLayout* shapeLayout = new QHBoxLayout(shapeGroup);
    QRadioButton* rbShapeSquare = new QRadioButton("Square");
    QRadioButton* rbShapeCircle = new QRadioButton("Circle");
    rbShapeSquare->setChecked(true); // Default from getDrawablePoints
    shapeLayout->addWidget(rbShapeSquare);
    shapeLayout->addWidget(rbShapeCircle);

    QGroupBox* shadingGroup = new QGroupBox("Shading");
    shadingGroup->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    QHBoxLayout* shadingLayout = new QHBoxLayout(shadingGroup);
    QRadioButton* rbShadingNone = new QRadioButton("None");
    QRadioButton* rbShadingPerVertex = new QRadioButton("Per Vertex");
    rbShadingNone->setChecked(true); // Default from getDrawablePoints
    shadingLayout->addWidget(rbShadingNone);
    shadingLayout->addWidget(rbShadingPerVertex);

    QHBoxLayout* radioLayout = new QHBoxLayout();
    radioLayout->addWidget(shapeGroup);
    radioLayout->addWidget(shadingGroup);
    layout->addLayout(radioLayout);

    vcl::qt::ViewerWidget* tw = new vcl::qt::ViewerWidget(&w);
    layout->addWidget(tw);

    const vcl::uint N_POINTS = 100;

    std::shared_ptr<vcl::DrawableObjectVector> vec =
        std::make_shared<vcl::DrawableObjectVector>();

    vec->pushBack(getDrawablePoints(N_POINTS));

    tw->setDrawableObjectVector(vec);
    auto pts = getPoints(vec);
    tslider->setValue(pts->width());
    
    shadingGroup->setEnabled(pts->hasNormals());

    QObject::connect(
        ccb,
        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        [=](int index) {
            using ColorSetting = vcl::Points::ColorSetting;

            std::cerr << "Color to use: " << index << std::endl;

            getPoints(vec)->setColorSetting((ColorSetting) index);
            tw->update();
        });

    QObject::connect(tslider, &QSlider::valueChanged, [=](int value) {
        std::cerr << "Size: " << value << std::endl;
        getPoints(vec)->setSize((float) value);
        tw->update();
    });

    QObject::connect(rbShapeSquare, &QRadioButton::toggled, [=](bool checked) {
        if (checked) {
            getPoints(vec)->setShape(vcl::Points::Shape::SQUARE);
            tw->update();
        }
    });

    QObject::connect(rbShapeCircle, &QRadioButton::toggled, [=](bool checked) {
        if (checked) {
            getPoints(vec)->setShape(vcl::Points::Shape::CIRCLE);
            tw->update();
        }
    });

    QObject::connect(rbShadingNone, &QRadioButton::toggled, [=](bool checked) {
        if (checked) {
            getPoints(vec)->setShading(vcl::Points::Shading::NONE);
            tw->update();
        }
    });

    QObject::connect(rbShadingPerVertex, &QRadioButton::toggled, [=](bool checked) {
        if (checked) {
            getPoints(vec)->setShading(vcl::Points::Shading::PER_VERTEX);
            tw->update();
        }
    });

    w.resize(1024, 768);

    w.show();

    return app.exec();
}
