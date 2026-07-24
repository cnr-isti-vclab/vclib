// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/drawable/drawable_lines.h>

#include <vclib/qt/viewer_widget.h>

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <vclib/algorithms.h>
#include <vclib/meshes.h>
#include <vclib/io.h>

std::shared_ptr<vcl::DrawableLines> getDrawableLines(vcl::uint nPoints, vcl::Lines::Topology topo)
{
    auto lines = std::make_shared<vcl::DrawableLines>();
    lines->setTopology(topo);

    if (nPoints == vcl::UINT_NULL) {
        // load bunny and use its vertices as points
        auto m =
            vcl::loadMesh<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");
        vcl::updatePerVertexAndFaceNormals(m);

        lines->setVertices(m.vertices() | vcl::views::positions);
        lines->setVertexNormals(m.vertices() | vcl::views::normals);

        if (topo == vcl::Lines::Topology::LINES) {
            std::vector<vcl::uint> indices;
            std::vector<vcl::Color> lineColors;
            std::vector<vcl::Point3d> lineNormals;
            
            for (const auto& f : m.faces()) {
                indices.push_back(f.vertexIndex(0));
                indices.push_back(f.vertexIndex(1));
                indices.push_back(f.vertexIndex(1));
                indices.push_back(f.vertexIndex(2));
                indices.push_back(f.vertexIndex(2));
                indices.push_back(f.vertexIndex(0));
                
                auto c1 = vcl::Color(vcl::random<uint8_t>(), vcl::random<uint8_t>(), vcl::random<uint8_t>());
                auto c2 = vcl::Color(vcl::random<uint8_t>(), vcl::random<uint8_t>(), vcl::random<uint8_t>());
                auto c3 = vcl::Color(vcl::random<uint8_t>(), vcl::random<uint8_t>(), vcl::random<uint8_t>());
                
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
        } else {
            vcl::uint nLines = m.vertexCount() - 1;
            std::vector<vcl::Color> lineColors(nLines);
            std::vector<vcl::Point3d> lineNormals(nLines);
            for (vcl::uint i = 0; i < nLines; ++i) {
                lineColors[i] = vcl::Color(vcl::random<uint8_t>(), vcl::random<uint8_t>(), vcl::random<uint8_t>());
                lineNormals[i] = vcl::Point3d(0, 1, 0); // dummy normal
            }
            lines->setLineColors(lineColors);
            lines->setLineNormals(lineNormals);
        }
        
        nPoints = m.vertexCount();
    }
    else {
        std::vector<vcl::Point3d> positions(nPoints);
        for (vcl::uint i = 0; i < nPoints; ++i) {
            positions[i] = vcl::random<vcl::Point3d>();
        }
        lines->setVertices(positions);
        
        std::vector<vcl::Point3d> normals(nPoints);
        for (vcl::uint i = 0; i < nPoints; ++i) {
            normals[i] = vcl::random<vcl::Point3d>().normalized();
        }
        lines->setVertexNormals(normals);

        vcl::uint nLines = topo == vcl::Lines::Topology::LINES ? nPoints / 2 : nPoints - 1;
        
        std::vector<vcl::Color> lineColors(nLines);
        std::vector<vcl::Point3d> lineNormals(nLines);
        for (vcl::uint i = 0; i < nLines; ++i) {
            lineColors[i] = vcl::Color(vcl::random<uint8_t>(), vcl::random<uint8_t>(), vcl::random<uint8_t>());
            lineNormals[i] = vcl::random<vcl::Point3d>().normalized();
        }
        lines->setLineColors(lineColors);
        lines->setLineNormals(lineNormals);
    }

    std::vector<vcl::Color> colors(nPoints);
    for (vcl::uint i = 0; i < nPoints; ++i) {
        colors[i] = vcl::Color(
            vcl::random<uint8_t>(),
            vcl::random<uint8_t>(),
            vcl::random<uint8_t>());
    }
    lines->setVertexColors(colors);

    lines->setWidth(5);
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

    QGroupBox* topoGroup = new QGroupBox("Topology");
    topoGroup->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    QHBoxLayout* topoLayout = new QHBoxLayout(topoGroup);
    QRadioButton* rbTopoLines = new QRadioButton("Lines");
    QRadioButton* rbTopoStrip = new QRadioButton("Line Strip");
    rbTopoLines->setChecked(true);
    topoLayout->addWidget(rbTopoLines);
    topoLayout->addWidget(rbTopoStrip);

    QGroupBox* shadingGroup = new QGroupBox("Shading");
    shadingGroup->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    QHBoxLayout* shadingLayout = new QHBoxLayout(shadingGroup);
    QRadioButton* rbShadingNone = new QRadioButton("None");
    QRadioButton* rbShadingPerVertex = new QRadioButton("Per Vertex");
    QRadioButton* rbShadingPerLine = new QRadioButton("Per Line");
    rbShadingNone->setChecked(true);
    shadingLayout->addWidget(rbShadingNone);
    shadingLayout->addWidget(rbShadingPerVertex);
    shadingLayout->addWidget(rbShadingPerLine);

    QHBoxLayout* radioLayout = new QHBoxLayout();
    radioLayout->addWidget(topoGroup);
    radioLayout->addWidget(shadingGroup);
    layout->addLayout(radioLayout);

    vcl::qt::ViewerWidget* tw = new vcl::qt::ViewerWidget(&w);
    layout->addWidget(tw);

    std::shared_ptr<vcl::DrawableObjectVector> vec =
        std::make_shared<vcl::DrawableObjectVector>();

    vec->pushBack(getDrawableLines(vcl::UINT_NULL, vcl::Lines::Topology::LINES));

    tw->setDrawableObjectVector(vec);
    auto lns = getLines(vec);
    tslider->setValue(lns->width());
    
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

    auto recreateLines = [=](vcl::Lines::Topology topo) {
        auto oldLines = getLines(vec);
        auto newLines = getDrawableLines(vcl::UINT_NULL, topo);
        
        // Preserve settings
        newLines->setWidth(oldLines->width());
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

    QObject::connect(rbShadingNone, &QRadioButton::toggled, [=](bool checked) {
        if (checked) {
            getLines(vec)->setShading(vcl::Lines::Shading::NONE);
            tw->update();
        }
    });

    QObject::connect(rbShadingPerVertex, &QRadioButton::toggled, [=](bool checked) {
        if (checked) {
            getLines(vec)->setShading(vcl::Lines::Shading::PER_VERTEX);
            tw->update();
        }
    });

    QObject::connect(rbShadingPerLine, &QRadioButton::toggled, [=](bool checked) {
        if (checked) {
            getLines(vec)->setShading(vcl::Lines::Shading::PER_LINE);
            tw->update();
        }
    });

    w.resize(1024, 768);

    w.show();

    return app.exec();
}
