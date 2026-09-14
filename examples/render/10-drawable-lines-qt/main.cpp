// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/drawable/drawable_lines.h>

#include <vclib/qt/application.h>
#include <vclib/qt/viewer_widget.h>

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

std::shared_ptr<vcl::DrawableLines> getDrawableLines(
    vcl::uint            nPoints,
    vcl::Lines::Topology topo)
{
    auto lines = std::make_shared<vcl::DrawableLines>();
    lines->setTopology(topo);

    if (nPoints == vcl::UINT_NULL) {
        // load bunny and use its vertices as points
        auto m = vcl::loadMesh<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH
                                             "/bunny_simplified.obj");
        vcl::updatePerVertexAndFaceNormals(m);

        if (topo == vcl::Lines::Topology::VECTORS) {
            std::vector<vcl::Point3d> pts;
            std::vector<vcl::Point3d> norms;
            std::vector<vcl::Color>   lineColors;
            std::vector<vcl::Point3d> lineNormals;

            for (const auto& v : m.vertices()) {
                pts.push_back(v.position());
                pts.push_back(v.normal()); // direction

                norms.push_back(v.normal());
                norms.push_back(v.normal());

                lineColors.push_back(vcl::Color(
                    vcl::random<uint8_t>(),
                    vcl::random<uint8_t>(),
                    vcl::random<uint8_t>()));
                lineNormals.push_back(v.normal());
            }
            lines->setVertices(pts);
            lines->setVertexNormals(norms);
            lines->setLineColors(lineColors);
            lines->setLineNormals(lineNormals);
        }
        else {
            lines->setVertices(m.vertices() | vcl::views::positions);
            lines->setVertexNormals(m.vertices() | vcl::views::normals);

            if (topo == vcl::Lines::Topology::LINES) {
            std::vector<vcl::uint>    indices;
            std::vector<vcl::Color>   lineColors;
            std::vector<vcl::Point3d> lineNormals;

            for (const auto& f : m.faces()) {
                indices.push_back(f.vertexIndex(0));
                indices.push_back(f.vertexIndex(1));
                indices.push_back(f.vertexIndex(1));
                indices.push_back(f.vertexIndex(2));
                indices.push_back(f.vertexIndex(2));
                indices.push_back(f.vertexIndex(0));

                auto c1 = vcl::Color(
                    vcl::random<uint8_t>(),
                    vcl::random<uint8_t>(),
                    vcl::random<uint8_t>());
                auto c2 = vcl::Color(
                    vcl::random<uint8_t>(),
                    vcl::random<uint8_t>(),
                    vcl::random<uint8_t>());
                auto c3 = vcl::Color(
                    vcl::random<uint8_t>(),
                    vcl::random<uint8_t>(),
                    vcl::random<uint8_t>());

                lineColors.push_back(c1);
                lineColors.push_back(c2);
                lineColors.push_back(c3);

                lineNormals.push_back(f.normal());
                lineNormals.push_back(f.normal());
                lineNormals.push_back(f.normal());
            }
            lines->setIndices(indices);
            lines->setLineColors(lineColors);
            lines->setLineNormals(lineNormals);
        }
        else {
            vcl::uint                 nLines = m.vertexCount() - 1;
            std::vector<vcl::Color>   lineColors(nLines);
            std::vector<vcl::Point3d> lineNormals(nLines);
            for (vcl::uint i = 0; i < nLines; ++i) {
                lineColors[i] = vcl::Color(
                    vcl::random<uint8_t>(),
                    vcl::random<uint8_t>(),
                    vcl::random<uint8_t>());
                lineNormals[i] = vcl::Point3d(0, 1, 0); // dummy normal
            }
            lines->setLineColors(lineColors);
            lines->setLineNormals(lineNormals);
            }
        }
    }
    else {
        std::vector<vcl::Point3d> positions(nPoints);
        for (vcl::uint i = 0; i < nPoints; ++i) {
            if (topo == vcl::Lines::Topology::VECTORS && i % 2 == 1) {
                positions[i] = vcl::random<vcl::Point3d>().normalized();
            } else {
                positions[i] = vcl::random<vcl::Point3d>();
            }
        }
        lines->setVertices(positions);

        std::vector<vcl::Point3d> normals(nPoints);
        for (vcl::uint i = 0; i < nPoints; ++i) {
            normals[i] = vcl::random<vcl::Point3d>().normalized();
        }
        lines->setVertexNormals(normals);

        vcl::uint nLines =
            topo == vcl::Lines::Topology::LINE_STRIP ? nPoints - 1 : nPoints / 2;

        std::vector<vcl::Color>   lineColors(nLines);
        std::vector<vcl::Point3d> lineNormals(nLines);
        for (vcl::uint i = 0; i < nLines; ++i) {
            lineColors[i] = vcl::Color(
                vcl::random<uint8_t>(),
                vcl::random<uint8_t>(),
                vcl::random<uint8_t>());
            lineNormals[i] = vcl::random<vcl::Point3d>().normalized();
        }
        lines->setLineColors(lineColors);
        lines->setLineNormals(lineNormals);
    }

    vcl::uint totalVertices = lines->vertexCount();
    std::vector<vcl::Color> colors(totalVertices);
    for (vcl::uint i = 0; i < totalVertices; ++i) {
        colors[i] = vcl::Color(
            vcl::random<uint8_t>(),
            vcl::random<uint8_t>(),
            vcl::random<uint8_t>());
    }
    lines->setVertexColors(colors);

    lines->setWidth(5);
    lines->setVectorLength(0.1f);
    lines->setColorSetting(vcl::Lines::ColorSetting::PER_VERTEX);
    lines->setShading(vcl::Lines::Shading::NONE);
    lines->setGeneralColor(vcl::Color::Magenta);

    return lines;
}

class ColorToUseComboBox : public QComboBox
{
public:
    ColorToUseComboBox(QWidget* parent = nullptr) : QComboBox(parent)
    {
        addItems({"Per Vertex", "Per Line", "General"});
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
    auto app = vcl::qt::Application(argc, argv);

    QWidget w;

    // add the viewer tw to the layout
    QVBoxLayout* layout = new QVBoxLayout(&w);

    ColorToUseComboBox* ccb = new ColorToUseComboBox(&w);
    layout->addWidget(ccb);

    QHBoxLayout* slidersLayout = new QHBoxLayout();
    
    QLabel* widthLabel = new QLabel("Width:");
    slidersLayout->addWidget(widthLabel);
    QSlider* tslider = new QSlider();
    tslider->setOrientation(Qt::Orientation::Horizontal);
    tslider->setMinimum(1);
    tslider->setMaximum(100);
    tslider->setValue(5);
    slidersLayout->addWidget(tslider);

    QLabel* lengthLabel = new QLabel("Vector Length:");
    slidersLayout->addWidget(lengthLabel);
    QSlider* lengthSlider = new QSlider();
    lengthSlider->setOrientation(Qt::Orientation::Horizontal);
    lengthSlider->setMinimum(1);
    lengthSlider->setMaximum(20);
    lengthSlider->setValue(10);
    slidersLayout->addWidget(lengthSlider);
    
    layout->addLayout(slidersLayout);

    QGroupBox* topoGroup = new QGroupBox("Topology");
    topoGroup->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    QHBoxLayout*  topoLayout  = new QHBoxLayout(topoGroup);
    QRadioButton* rbTopoLines   = new QRadioButton("Lines");
    QRadioButton* rbTopoStrip   = new QRadioButton("Line Strip");
    QRadioButton* rbTopoVectors = new QRadioButton("Vectors");
    rbTopoLines->setChecked(true);
    topoLayout->addWidget(rbTopoLines);
    topoLayout->addWidget(rbTopoStrip);
    topoLayout->addWidget(rbTopoVectors);

    QGroupBox* shadingGroup = new QGroupBox("Shading");
    shadingGroup->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    QHBoxLayout*  shadingLayout      = new QHBoxLayout(shadingGroup);
    QRadioButton* rbShadingNone      = new QRadioButton("None");
    QRadioButton* rbShadingPerVertex = new QRadioButton("Per Vertex");
    QRadioButton* rbShadingPerLine   = new QRadioButton("Per Line");
    rbShadingNone->setChecked(true);
    shadingLayout->addWidget(rbShadingNone);
    shadingLayout->addWidget(rbShadingPerVertex);
    shadingLayout->addWidget(rbShadingPerLine);

    QHBoxLayout* radioLayout = new QHBoxLayout();
    radioLayout->addWidget(topoGroup);
    radioLayout->addWidget(shadingGroup);
    layout->addLayout(radioLayout);

    vcl::qt::ViewerWidget* tw = new vcl::qt::ViewerWidget(&w);
    layout->addWidget(tw, 1);

    std::shared_ptr<vcl::DrawableObjectVector> vec =
        std::make_shared<vcl::DrawableObjectVector>();

    vec->pushBack(
        getDrawableLines(vcl::UINT_NULL, vcl::Lines::Topology::LINES));

    tw->setDrawableObjectVector(vec);
    auto lns = getLines(vec);
    tslider->setValue(lns->width());
    lengthSlider->setValue(lns->vectorLength() * 100);

    shadingGroup->setEnabled(lns->hasVertexNormals() || lns->hasLineNormals());

    QObject::connect(
        ccb,
        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        [=](int index) {
            using ColorSetting = vcl::Lines::ColorSetting;

            std::cerr << "Color to use: " << index << std::endl;

            getLines(vec)->setColorSetting((ColorSetting) index);
            tw->update();
        });

    QObject::connect(tslider, &QSlider::valueChanged, [=](int value) {
        std::cerr << "Width: " << value << std::endl;
        getLines(vec)->setWidth((float) value);
        tw->update();
    });

    QObject::connect(lengthSlider, &QSlider::valueChanged, [=](int value) {
        std::cerr << "Vector Length: " << value / 100.0f << std::endl;
        getLines(vec)->setVectorLength(value / 100.0f);
        tw->update();
    });

    auto recreateLines = [=](vcl::Lines::Topology topo) {
        auto oldLines = getLines(vec);
        auto newLines = getDrawableLines(vcl::UINT_NULL, topo);

        // Preserve settings
        newLines->setWidth(oldLines->width());
        newLines->setVectorLength(oldLines->vectorLength());
        newLines->setColorSetting(oldLines->colorSetting());
        newLines->setShading(oldLines->shading());
        newLines->setGeneralColor(oldLines->generalColor());

        vec->at(0) = newLines;
        tw->update();
    };

    QObject::connect(rbTopoLines, &QRadioButton::toggled, [=](bool checked) {
        if (checked) {
            recreateLines(vcl::Lines::Topology::LINES);
        }
    });

    QObject::connect(rbTopoStrip, &QRadioButton::toggled, [=](bool checked) {
        if (checked) {
            recreateLines(vcl::Lines::Topology::LINE_STRIP);
        }
    });

    QObject::connect(rbTopoVectors, &QRadioButton::toggled, [=](bool checked) {
        if (checked) {
            recreateLines(vcl::Lines::Topology::VECTORS);
        }
    });

    QObject::connect(rbShadingNone, &QRadioButton::toggled, [=](bool checked) {
        if (checked) {
            getLines(vec)->setShading(vcl::Lines::Shading::NONE);
            tw->update();
        }
    });

    QObject::connect(
        rbShadingPerVertex, &QRadioButton::toggled, [=](bool checked) {
            if (checked) {
                getLines(vec)->setShading(vcl::Lines::Shading::PER_VERTEX);
                tw->update();
            }
        });

    QObject::connect(
        rbShadingPerLine, &QRadioButton::toggled, [=](bool checked) {
            if (checked) {
                getLines(vec)->setShading(vcl::Lines::Shading::PER_LINE);
                tw->update();
            }
        });

    w.resize(1024, 768);

    w.show();

    return app.exec();
}
