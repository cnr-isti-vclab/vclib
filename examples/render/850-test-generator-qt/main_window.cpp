// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "main_window.h"
#include "setup_viewer.h"

#include <vclib/render/drawable/drawable_mesh.h>

#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QImage>
#include <QPixmap>
#include <QClipboard>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    mViewer("Test Generator Viewer", 1920, 1080)
{
    setupUI();
    setupMeshViewer();
    updateScreenshot();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    // Left Panel (Controls)
    QVBoxLayout* controlLayout = new QVBoxLayout();
    controlLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->addLayout(controlLayout, 1);

    // Mouse Settings
    QFormLayout* formLayout = new QFormLayout();
    mSpinX = new QSpinBox();
    mSpinX->setRange(-9999, 9999);
    mSpinY = new QSpinBox();
    mSpinY->setRange(-9999, 9999);
    formLayout->addRow("Mouse X:", mSpinX);
    formLayout->addRow("Mouse Y:", mSpinY);

    mComboMouseButton = new QComboBox();
    mComboMouseButton->addItem("vcl::MouseButton::Enum::LEFT", QVariant(vcl::MouseButton::LEFT));
    mComboMouseButton->addItem("vcl::MouseButton::Enum::RIGHT", QVariant(vcl::MouseButton::RIGHT));
    mComboMouseButton->addItem("vcl::MouseButton::Enum::MIDDLE", QVariant(vcl::MouseButton::MIDDLE));
    mComboMouseButton->addItem("vcl::MouseButton::Enum::NO_BUTTON", QVariant(vcl::MouseButton::NO_BUTTON));
    formLayout->addRow("Button:", mComboMouseButton);
    
    controlLayout->addLayout(formLayout);

    QPushButton* btnPress = new QPushButton("Simulate Mouse Press");
    connect(btnPress, &QPushButton::clicked, this, &MainWindow::simulateMousePress);
    controlLayout->addWidget(btnPress);

    QPushButton* btnRelease = new QPushButton("Simulate Mouse Release");
    connect(btnRelease, &QPushButton::clicked, this, &MainWindow::simulateMouseRelease);
    controlLayout->addWidget(btnRelease);

    QPushButton* btnMove = new QPushButton("Simulate Mouse Move");
    connect(btnMove, &QPushButton::clicked, this, &MainWindow::simulateMouseMove);
    controlLayout->addWidget(btnMove);

    QPushButton* btnDClick = new QPushButton("Simulate Double Click");
    connect(btnDClick, &QPushButton::clicked, this, &MainWindow::simulateMouseDoubleClick);
    controlLayout->addWidget(btnDClick);

    // Scroll settings
    QFormLayout* scrollFormLayout = new QFormLayout();
    mSpinScrollX = new QSpinBox();
    mSpinScrollX->setRange(-9999, 9999);
    mSpinScrollY = new QSpinBox();
    mSpinScrollY->setRange(-9999, 9999);
    scrollFormLayout->addRow("Scroll X:", mSpinScrollX);
    scrollFormLayout->addRow("Scroll Y:", mSpinScrollY);
    controlLayout->addLayout(scrollFormLayout);

    QPushButton* btnScroll = new QPushButton("Simulate Mouse Scroll");
    connect(btnScroll, &QPushButton::clicked, this, &MainWindow::simulateMouseScroll);
    controlLayout->addWidget(btnScroll);

    // Key Settings
    controlLayout->addSpacing(20);
    QFormLayout* keyFormLayout = new QFormLayout();
    mComboKey = new QComboBox();
    mComboKey->addItem("vcl::Key::Enum::SPACE", QVariant(vcl::Key::SPACE));
    mComboKey->addItem("vcl::Key::Enum::A", QVariant(vcl::Key::A));
    mComboKey->addItem("vcl::Key::Enum::ESCAPE", QVariant(vcl::Key::ESCAPE));
    keyFormLayout->addRow("Key:", mComboKey);
    controlLayout->addLayout(keyFormLayout);

    QPushButton* btnKeyPress = new QPushButton("Simulate Key Press");
    connect(btnKeyPress, &QPushButton::clicked, this, &MainWindow::simulateKeyPress);
    controlLayout->addWidget(btnKeyPress);

    QPushButton* btnKeyRelease = new QPushButton("Simulate Key Release");
    connect(btnKeyRelease, &QPushButton::clicked, this, &MainWindow::simulateKeyRelease);
    controlLayout->addWidget(btnKeyRelease);

    // Modifiers Settings
    controlLayout->addSpacing(20);
    QHBoxLayout* modLayout = new QHBoxLayout();
    mCheckShift = new QCheckBox("Shift");
    mCheckCtrl = new QCheckBox("Ctrl");
    mCheckAlt = new QCheckBox("Alt");
    mCheckSuper = new QCheckBox("Super");
    modLayout->addWidget(mCheckShift);
    modLayout->addWidget(mCheckCtrl);
    modLayout->addWidget(mCheckAlt);
    modLayout->addWidget(mCheckSuper);
    controlLayout->addLayout(modLayout);

    QPushButton* btnSetModifiers = new QPushButton("Simulate Set Modifiers");
    connect(btnSetModifiers, &QPushButton::clicked, this, &MainWindow::simulateSetModifiers);
    controlLayout->addWidget(btnSetModifiers);

    // Trackball Settings
    controlLayout->addSpacing(20);
    QFormLayout* trackballFormLayout = new QFormLayout();
    mSpinAxisX                       = new QDoubleSpinBox();
    mSpinAxisX->setRange(-1.0, 1.0);
    mSpinAxisX->setValue(0.0);
    mSpinAxisX->setSingleStep(0.1);
    mSpinAxisY = new QDoubleSpinBox();
    mSpinAxisY->setRange(-1.0, 1.0);
    mSpinAxisY->setValue(1.0);
    mSpinAxisY->setSingleStep(0.1);
    mSpinAxisZ = new QDoubleSpinBox();
    mSpinAxisZ->setRange(-1.0, 1.0);
    mSpinAxisZ->setValue(0.0);
    mSpinAxisZ->setSingleStep(0.1);
    mSpinAngle = new QDoubleSpinBox();
    mSpinAngle->setRange(-360.0, 360.0);
    mSpinAngle->setValue(45.0);
    mSpinAngle->setSingleStep(15.0);
    mSpinZoom = new QDoubleSpinBox();
    mSpinZoom->setRange(-1000.0, 1000.0);
    mSpinZoom->setValue(-60);
    mSpinZoom->setSingleStep(60);

    QHBoxLayout* axisLayout = new QHBoxLayout();
    axisLayout->addWidget(mSpinAxisX);
    axisLayout->addWidget(mSpinAxisY);
    axisLayout->addWidget(mSpinAxisZ);
    trackballFormLayout->addRow("Axis (X,Y,Z):", axisLayout);
    trackballFormLayout->addRow("Angle (deg):", mSpinAngle);
    trackballFormLayout->addRow("Zoom Factor:", mSpinZoom);
    controlLayout->addLayout(trackballFormLayout);

    QPushButton* btnRotate = new QPushButton("Simulate Trackball Rotate");
    connect(btnRotate, &QPushButton::clicked, this, &MainWindow::simulateTrackballRotate);
    controlLayout->addWidget(btnRotate);

    QPushButton* btnZoom = new QPushButton("Simulate Trackball Zoom");
    connect(btnZoom, &QPushButton::clicked, this, &MainWindow::simulateTrackballZoom);
    controlLayout->addWidget(btnZoom);

    // Custom Actions Settings
    controlLayout->addSpacing(20);
    QFormLayout* customActionsLayout = new QFormLayout();
    mComboCustomActions = new QComboBox();
    customActionsLayout->addRow("Custom Actions:", mComboCustomActions);
    controlLayout->addLayout(customActionsLayout);

    QPushButton* btnExecuteCustomAction = new QPushButton("Execute Selected Action");
    connect(btnExecuteCustomAction, &QPushButton::clicked, this, &MainWindow::executeCustomAction);
    controlLayout->addWidget(btnExecuteCustomAction);

    // Code Output
    controlLayout->addSpacing(20);
    controlLayout->addWidget(new QLabel("Generated Code:"));
    mCodeOutput = new QTextEdit();
    mCodeOutput->setReadOnly(true);
    controlLayout->addWidget(mCodeOutput);

    QPushButton* btnCopy = new QPushButton("Copy to Clipboard");
    connect(btnCopy, &QPushButton::clicked, [this](){
        QApplication::clipboard()->setText(mCodeOutput->toPlainText());
    });
    controlLayout->addWidget(btnCopy);

    // Status Label
    controlLayout->addSpacing(20);
    mStatusLabel = new QLabel("Hover over image for coordinates");
    controlLayout->addWidget(mStatusLabel);

    controlLayout->addStretch();

    // Right Panel (Image Label)
    mImageLabel = new ImageLabel();
    mImageLabel->setStyleSheet("background-color: black;");
    mImageLabel->setAlignment(Qt::AlignCenter);
    mImageLabel->setMinimumSize(800, 600); // scaled down view
    connect(mImageLabel, &ImageLabel::mouseHovered, this, &MainWindow::onImageHovered);
    connect(mImageLabel, &ImageLabel::mouseClicked, this, &MainWindow::onImageClicked);
    
    // allow the image label to resize and we scale the pixmap
    mainLayout->addWidget(mImageLabel, 3);
}

void MainWindow::setupMeshViewer()
{
    mCustomActions = setupViewer(mViewer);

    // Populate combobox with custom actions
    for (size_t i = 0; i < mCustomActions.size(); ++i) {
        mComboCustomActions->addItem(QString::fromStdString(mCustomActions[i].first), QVariant(static_cast<int>(i)));
    }
}

void MainWindow::updateScreenshot()
{
    vcl::Image img;
    mViewer.screenshot(img);

    if (img.width() > 0 && img.height() > 0) {
        QImage qimg(img.data(), img.width(), img.height(), QImage::Format_RGBA8888);
        mImageLabel->setImage(qimg);
    }
}

void MainWindow::onImageHovered(int x, int y)
{
    // calculate the actual pixel position in the 1920x1080 image based on the scaled pixmap
    if (mImageLabel->pixmap(Qt::ReturnByValue).isNull()) return;

    QPixmap px = mImageLabel->pixmap(Qt::ReturnByValue);
    
    int pxWidth = px.width();
    int pxHeight = px.height();

    int labelWidth = mImageLabel->width();
    int labelHeight = mImageLabel->height();

    int xOffset = (labelWidth - pxWidth) / 2;
    int yOffset = (labelHeight - pxHeight) / 2;

    int realX = x - xOffset;
    int realY = y - yOffset;

    if (realX >= 0 && realX < pxWidth && realY >= 0 && realY < pxHeight) {
        // Map from displayed size to actual viewer size
        int origX = (realX * mViewer.width()) / pxWidth;
        int origY = (realY * mViewer.height()) / pxHeight;

        mStatusLabel->setText(QString("Hover: X: %1, Y: %2").arg(origX).arg(origY));
    } else {
        mStatusLabel->setText("Hover: out of bounds");
    }
}

void MainWindow::onImageClicked(int x, int y, Qt::MouseButton button)
{
    if (button == Qt::LeftButton) {
        if (mImageLabel->pixmap(Qt::ReturnByValue).isNull()) return;

        QPixmap px = mImageLabel->pixmap(Qt::ReturnByValue);
        
        int pxWidth = px.width();
        int pxHeight = px.height();

        int labelWidth = mImageLabel->width();
        int labelHeight = mImageLabel->height();

        int xOffset = (labelWidth - pxWidth) / 2;
        int yOffset = (labelHeight - pxHeight) / 2;

        int realX = x - xOffset;
        int realY = y - yOffset;

        if (realX >= 0 && realX < pxWidth && realY >= 0 && realY < pxHeight) {
            int origX = (realX * mViewer.width()) / pxWidth;
            int origY = (realY * mViewer.height()) / pxHeight;

            mSpinX->setValue(origX);
            mSpinY->setValue(origY);
        }
    }
}

void MainWindow::appendCode(const QString& code)
{
    mCodeOutput->append(code);
}

void MainWindow::simulateMousePress()
{
    int x = mSpinX->value();
    int y = mSpinY->value();
    vcl::MouseButton::Enum btn = static_cast<vcl::MouseButton::Enum>(mComboMouseButton->currentData().toInt());
    QString btnText = mComboMouseButton->currentText();
    
    mViewer.simulateMousePress(btn, x, y);
    appendCode(QString("mv.simulateMousePress(%1, %2, %3);").arg(btnText).arg(x).arg(y));
    updateScreenshot();
}

void MainWindow::simulateMouseRelease()
{
    int x = mSpinX->value();
    int y = mSpinY->value();
    vcl::MouseButton::Enum btn = static_cast<vcl::MouseButton::Enum>(mComboMouseButton->currentData().toInt());
    QString btnText = mComboMouseButton->currentText();
    
    mViewer.simulateMouseRelease(btn, x, y);
    appendCode(QString("mv.simulateMouseRelease(%1, %2, %3);").arg(btnText).arg(x).arg(y));
    updateScreenshot();
}

void MainWindow::simulateMouseDoubleClick()
{
    int x = mSpinX->value();
    int y = mSpinY->value();
    vcl::MouseButton::Enum btn = static_cast<vcl::MouseButton::Enum>(mComboMouseButton->currentData().toInt());
    QString btnText = mComboMouseButton->currentText();
    
    mViewer.simulateMouseDoubleClick(btn, x, y);
    appendCode(QString("mv.simulateMouseDoubleClick(%1, %2, %3);").arg(btnText).arg(x).arg(y));
    updateScreenshot();
}

void MainWindow::simulateMouseMove()
{
    int x = mSpinX->value();
    int y = mSpinY->value();
    
    mViewer.simulateMouseMove(x, y);
    appendCode(QString("mv.simulateMouseMove(%1, %2);").arg(x).arg(y));
    updateScreenshot();
}

void MainWindow::simulateMouseScroll()
{
    int x = mSpinScrollX->value();
    int y = mSpinScrollY->value();
    
    mViewer.simulateMouseScroll(x, y);
    appendCode(QString("mv.simulateMouseScroll(%1, %2);").arg(x).arg(y));
    updateScreenshot();
}

void MainWindow::simulateKeyPress()
{
    vcl::Key::Enum key = static_cast<vcl::Key::Enum>(mComboKey->currentData().toInt());
    QString keyText = mComboKey->currentText();
    mViewer.simulateKeyPress(key);
    appendCode(QString("mv.simulateKeyPress(%1);").arg(keyText));
    updateScreenshot();
}

void MainWindow::simulateKeyRelease()
{
    vcl::Key::Enum key = static_cast<vcl::Key::Enum>(mComboKey->currentData().toInt());
    QString keyText = mComboKey->currentText();
    mViewer.simulateKeyRelease(key);
    appendCode(QString("mv.simulateKeyRelease(%1);").arg(keyText));
    updateScreenshot();
}

void MainWindow::simulateSetModifiers()
{
    vcl::KeyModifiers mods;
    QStringList activeMods;

    if (mCheckShift->isChecked()) {
        mods[vcl::KeyModifier::SHIFT] = true;
        activeMods << "vcl::KeyModifier::SHIFT";
    }
    if (mCheckCtrl->isChecked()) {
        mods[vcl::KeyModifier::CONTROL] = true;
        activeMods << "vcl::KeyModifier::CONTROL";
    }
    if (mCheckAlt->isChecked()) {
        mods[vcl::KeyModifier::ALT] = true;
        activeMods << "vcl::KeyModifier::ALT";
    }
    if (mCheckSuper->isChecked()) {
        mods[vcl::KeyModifier::SUPER] = true;
        activeMods << "vcl::KeyModifier::SUPER";
    }

    mViewer.simulateSetModifiers(mods);
    appendCode(QString("mv.simulateSetModifiers(vcl::KeyModifiers{ %1 });").arg(activeMods.join(", ")));
    updateScreenshot();
}

void MainWindow::simulateTrackballRotate()
{
    float x = mSpinAxisX->value();
    float y = mSpinAxisY->value();
    float z = mSpinAxisZ->value();
    float angleDeg = mSpinAngle->value();
    float angleRad = vcl::toRad(angleDeg);

    mViewer.trackballRotate(vcl::Point3f(x, y, z), angleRad);
    appendCode(QString("mv.trackballRotate(vcl::Point3f(%1, %2, %3), vcl::toRad(%4f));").arg(x).arg(y).arg(z).arg(angleDeg));
    updateScreenshot();
}

void MainWindow::simulateTrackballZoom()
{
    float factor = mSpinZoom->value();
    mViewer.trackballZoom(factor);
    appendCode(QString("mv.trackballZoom(%1f);").arg(factor));
    updateScreenshot();
}

void MainWindow::executeCustomAction()
{
    if (mCustomActions.empty()) return;

    int index = mComboCustomActions->currentData().toInt();
    if (index >= 0 && index < static_cast<int>(mCustomActions.size())) {
        mCustomActions[index].second(); // execute lambda
        appendCode(QString("\n// Executed custom action: %1\n").arg(QString::fromStdString(mCustomActions[index].first)));
        updateScreenshot();
    }
}
