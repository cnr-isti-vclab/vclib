// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCLIB_RENDER_EXAMPLES_TEST_GENERATOR_MAIN_WINDOW_H
#define VCLIB_RENDER_EXAMPLES_TEST_GENERATOR_MAIN_WINDOW_H

#include "image_label.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QMainWindow>
#include <QSpinBox>
#include <QTextEdit>
#include <functional>
#include <string>
#include <utility>
#include <vclib/render/headless_mesh_viewer.h>
#include <vector>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void onImageHovered(int x, int y);
    void onImageClicked(int x, int y, Qt::MouseButton button);
    void simulateMousePress();
    void simulateMouseRelease();
    void simulateMouseDoubleClick();
    void simulateMouseMove();
    void simulateMouseScroll();
    void simulateKeyPress();
    void simulateKeyRelease();
    void simulateSetModifiers();
    void simulateTrackballRotate();
    void simulateTrackballZoom();
    void executeCustomAction();
    void updateScreenshot();

private:
    void setupUI();
    void setupMeshViewer();
    void appendCode(const QString& code);

    // Headless viewer
    vcl::HeadlessMeshViewer mViewer;

    // UI elements
    ImageLabel*     mImageLabel;
    QLabel*         mStatusLabel;
    QTextEdit*      mCodeOutput;
    QSpinBox*       mSpinX;
    QSpinBox*       mSpinY;
    QSpinBox*       mSpinScrollX;
    QSpinBox*       mSpinScrollY;
    QDoubleSpinBox* mSpinAxisX;
    QDoubleSpinBox* mSpinAxisY;
    QDoubleSpinBox* mSpinAxisZ;
    QDoubleSpinBox* mSpinAngle;
    QDoubleSpinBox* mSpinZoom;
    QComboBox*      mComboMouseButton;
    QComboBox*      mComboKey;
    QCheckBox*      mCheckShift;
    QCheckBox*      mCheckCtrl;
    QCheckBox*      mCheckAlt;
    QCheckBox*      mCheckSuper;
    QComboBox*      mComboCustomActions;

    // Custom actions from setupViewer
    std::vector<std::pair<std::string, std::function<void()>>> mCustomActions;
};

#endif // VCLIB_RENDER_EXAMPLES_TEST_GENERATOR_MAIN_WINDOW_H
