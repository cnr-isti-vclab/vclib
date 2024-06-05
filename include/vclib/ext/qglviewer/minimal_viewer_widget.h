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

#ifndef VCL_EXT_QGLVIEWER_MINIMAL_VIEWER_WIDGET_H
#define VCL_EXT_QGLVIEWER_MINIMAL_VIEWER_WIDGET_H

#include <QGLViewer/qglviewer.h>

#include <vclib/render/drawable/drawable_object_vector.h>

namespace vcl::qgl {

class MinimalViewerWidget : public QGLViewer
{
    // this GLArea does not normally own this drawList
    std::shared_ptr<DrawableObjectVector> mDrawList;

public:
    MinimalViewerWidget(QWidget* parent = nullptr);

    MinimalViewerWidget(
        const std::shared_ptr<DrawableObjectVector>& v,
        QWidget*                              parent = nullptr);

    void init();

    void setDrawableObjectVector(
        const std::shared_ptr<DrawableObjectVector>& v);

    std::shared_ptr<const DrawableObjectVector> drawableObjectVector()
        const;

    void fitScene();

protected:
    // QGLViever function, does not require a viewId
    virtual void draw();
};

} // namespace vcl::qgl

#endif // VCL_EXT_QGLVIEWER_MINIMAL_VIEWER_WIDGET_H
