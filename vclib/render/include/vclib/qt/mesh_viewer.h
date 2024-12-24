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

#ifndef VCL_QT_MESH_VIEWER_H
#define VCL_QT_MESH_VIEWER_H

#include <vclib/qt/gui/text_edit_logger.h>
#include <vclib/render/drawable/drawable_object_vector.h>

#include <QWidget>

namespace vcl::qt {

namespace Ui {
class MeshViewer;
} // namespace Ui

class MeshViewer : public QWidget
{
    Q_OBJECT

    Ui::MeshViewer* mUI;

    std::shared_ptr<vcl::DrawableObjectVector> mDrawableObjectVector;

    std::shared_ptr<vcl::DrawableObjectVector> mListedDrawableObjects;
    std::shared_ptr<vcl::DrawableObjectVector> mUnlistedDrawableObjects;

public:
    explicit MeshViewer(QWidget* parent = nullptr);
    ~MeshViewer();

    void setDrawableObjectVector(
        const std::shared_ptr<vcl::DrawableObjectVector>& v);

    void setUnlistedDrawableObjectVector(
        const std::shared_ptr<vcl::DrawableObjectVector>& v);

    uint selectedDrawableObject() const;

    TextEditLogger& logger();

public slots:
    void visibilityDrawableObjectChanged();

    void selectedDrawableObjectChanged(uint i);

    void renderSettingsUpdated();

    void fitScene();

    void updateGUI();
};

} // namespace vcl::qt

#endif // VCL_QT_MESH_VIEWER_H
