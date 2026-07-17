// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/drawable/drawable_points.h>

#include <vclib/qt/viewer_widget.h>

#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

std::shared_ptr<vcl::DrawablePoints> getDrawablePoints(vcl::uint nPoints)
{
    auto points = std::make_shared<vcl::DrawablePoints>();

    if (nPoints == vcl::UINT_NULL) {
        // load bunny and use its vertices as points
        auto m =
            vcl::loadMesh<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");
        vcl::updatePerVertexAndFaceNormals(m);

        nPoints = m.vertexCount();

        points->setVertices(m.vertices() | vcl::views::positions);
        points->setVertexNormals(m.vertices() | vcl::views::normals);

        std::vector<bool> selections(nPoints);
        for (vcl::uint i = 0; i < nPoints; ++i) {
            selections[i] = m.vertex(i).position().x() > 0.3;
        }
        points->setVertexSelection(selections);
    }
    else {
        std::vector<vcl::Point3d> positions(nPoints);

        for (vcl::uint i = 0; i < nPoints; ++i) {
            positions[i] = vcl::random<vcl::Point3d>();
        }

        points->setVertices(positions);

        std::vector<bool> selections(nPoints);
        for (vcl::uint i = 0; i < nPoints; ++i) {
            selections[i] = positions[i].x() > 0.3;
        }
        points->setVertexSelection(selections);
    }

    std::vector<vcl::Color> colors(nPoints);
    for (vcl::uint i = 0; i < nPoints; ++i) {
        colors[i] = vcl::Color(
            vcl::random<uint8_t>(),
            vcl::random<uint8_t>(),
            vcl::random<uint8_t>());
    }
    points->setVertexColors(colors);

    points->setSize(10);
    points->setColorSetting(vcl::Points::ColorSetting::GENERAL);
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
        addItems({"General", "Per Vertex"});
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
    QHBoxLayout*  shapeLayout   = new QHBoxLayout(shapeGroup);
    QRadioButton* rbShapeSquare = new QRadioButton("Square");
    QRadioButton* rbShapeCircle = new QRadioButton("Circle");
    rbShapeSquare->setChecked(true);
    shapeLayout->addWidget(rbShapeSquare);
    shapeLayout->addWidget(rbShapeCircle);

    QGroupBox* shadingGroup = new QGroupBox("Shading");
    shadingGroup->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    QHBoxLayout*  shadingLayout      = new QHBoxLayout(shadingGroup);
    QRadioButton* rbShadingNone      = new QRadioButton("None");
    QRadioButton* rbShadingPerVertex = new QRadioButton("Per Vertex");
    rbShadingNone->setChecked(true);
    shadingLayout->addWidget(rbShadingNone);
    shadingLayout->addWidget(rbShadingPerVertex);

    QGroupBox* selectionGroup = new QGroupBox("Selection");
    selectionGroup->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    QHBoxLayout* selectionLayout = new QHBoxLayout(selectionGroup);
    QCheckBox*   cbShowSelection = new QCheckBox("Show Selection");
    QPushButton* btnSelectionColor = new QPushButton("Selection Color...");
    selectionLayout->addWidget(cbShowSelection);
    selectionLayout->addWidget(btnSelectionColor);

    QHBoxLayout* radioLayout = new QHBoxLayout();
    radioLayout->addWidget(shapeGroup);
    radioLayout->addWidget(shadingGroup);
    radioLayout->addWidget(selectionGroup);
    layout->addLayout(radioLayout);

    vcl::qt::ViewerWidget* tw = new vcl::qt::ViewerWidget(&w);
    layout->addWidget(tw);

    const vcl::uint N_POINTS = 100;

    std::shared_ptr<vcl::DrawableObjectVector> vec =
        std::make_shared<vcl::DrawableObjectVector>();

    vec->pushBack(getDrawablePoints(vcl::UINT_NULL));

    tw->setDrawableObjectVector(vec);
    auto pts = getPoints(vec);
    tslider->setValue(pts->width());
    cbShowSelection->setChecked(pts->isSelectionVisible());

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

    QObject::connect(
        rbShadingPerVertex, &QRadioButton::toggled, [=](bool checked) {
            if (checked) {
                getPoints(vec)->setShading(vcl::Points::Shading::PER_VERTEX);
                tw->update();
            }
        });

    QObject::connect(cbShowSelection, &QCheckBox::toggled, [=](bool checked) {
        getPoints(vec)->setSelectionVisibility(checked);
        tw->update();
    });

    QObject::connect(btnSelectionColor, &QPushButton::clicked, [=]() {
        vcl::Color currentSelectionCol = getPoints(vec)->selectionColor();
        QColor     qtCurrentCol(
            currentSelectionCol.red(),
            currentSelectionCol.green(),
            currentSelectionCol.blue(),
            currentSelectionCol.alpha());

        QColor col = QColorDialog::getColor(
            qtCurrentCol, tw, "Select Selection Color", QColorDialog::ShowAlphaChannel);
        if (col.isValid()) {
            getPoints(vec)->setSelectionColor(
                vcl::Color(col.red(), col.green(), col.blue(), 128));
            tw->update();
        }
    });

    w.resize(1024, 768);

    w.show();

    return app.exec();
}
