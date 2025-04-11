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

#ifndef VCL_QT_GUI_DRAWABLE_OBJECT_VECTOR_FRAME_H
#define VCL_QT_GUI_DRAWABLE_OBJECT_VECTOR_FRAME_H

#include "drawable_object_frame.h"

#include <vclib/render/drawable/drawable_object_vector.h>

#include <QFrame>

namespace vcl::qt {

namespace Ui {
class DrawableObjectVectorFrame;
} // namespace Ui

class DrawableObjectVectorFrame : public QFrame
{
public:
    using IconFunction =
        std::function<std::pair<QIcon, std::string>(const DrawableObject&)>;

private:
    Q_OBJECT

    // this frame does not normally own this drawList
    std::shared_ptr<DrawableObjectVector> mDrawList;
    Ui::DrawableObjectVectorFrame*        mUI;

    // icon function
    IconFunction mIconFunction = nullptr;

public:
    explicit DrawableObjectVectorFrame(QWidget* parent = nullptr);
    explicit DrawableObjectVectorFrame(
        const std::shared_ptr<DrawableObjectVector>& v,
        QWidget*                                     parent = nullptr);
    ~DrawableObjectVectorFrame();

    void setIconFunction(const IconFunction& f);

    void setDrawableObjectVector(
        const std::shared_ptr<vcl::DrawableObjectVector>& v);

    uint selectedDrawableObject() const;

    void update(); // todo: rename this to refresh

    // function to set the selected item
    bool setSelectedItem(uint i);

signals:
    void drawableObjectVisibilityChanged();
    void drawableObjectSelectionChanged(uint i);

private slots:
    void on_listWidget_itemSelectionChanged();

private:
    void updateDrawableVectorWidget();
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_DRAWABLE_OBJECT_VECTOR_FRAME_H
