/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_EXT_QGLVIEWER_VIEWER_H
#define VCL_EXT_QGLVIEWER_VIEWER_H

#include <memory>

#include <QGLViewer/qglviewer.h>

#include <vclib/math/min_max.h>
#include <vclib/render/drawable_object_vector.h>
#include <vclib/space/box.h>

namespace vcl::qgl {

class Viewer : public QGLViewer
{
    // this GLArea does not normally own this drawList
    std::shared_ptr<DrawableObjectVector> drawList;

public:
    Viewer(QWidget* parent = nullptr) : QGLViewer(parent)
    {
        drawList = std::make_shared<DrawableObjectVector>();
    }

    Viewer(std::shared_ptr<DrawableObjectVector> v, QWidget* parent = nullptr) :
            QGLViewer(parent), drawList(v)
    {
    }

    void init()
    {
        for (DrawableObject* d : *drawList) {
            d->init();
        }
    }

    void setDrawableObjectVector(std::shared_ptr<DrawableObjectVector> v)
    {
        drawList = v;
    }

    std::shared_ptr<DrawableObjectVector> drawableObjectVector()
    {
        return drawList;
    }

    std::shared_ptr<const DrawableObjectVector> drawableObjectVector() const
    {
        return drawList;
    }

    void fitScene()
    {
        Box3d   bb          = fullBB();
        Point3d sceneCenter = bb.center();
        double  sceneRadius = bb.diagonal() / 2;

        setSceneCenter(qglviewer::Vec(
            sceneCenter.x(), sceneCenter.y(), sceneCenter.z()));
        setSceneRadius(sceneRadius);
        showEntireScene();
    }

protected:
    virtual void draw()
    {
        for (const DrawableObject* obj : *drawList)
            obj->draw();
    }

private:
    uint firstVisibleObject() const
    {
        uint i = 0;

        // if the current object i is not visible, check the next one
        while (i < drawList->size() && !drawList->at(i).isVisible())
            i++;

        return i;
    }

    vcl::Box3d fullBB() const
    {
        Box3d bb(Point3d(-1, -1, -1), Point3d(1, 1, 1));
        if (drawList->size() > 0) {
            uint i = firstVisibleObject();

            if (i < drawList->size()) {
                Point3d sc = drawList->at(i).center();
                bb.min()   = sc - drawList->at(i).radius();
                bb.max()   = sc + drawList->at(i).radius();

                for (i = i + 1; i < drawList->size(); i++) { // rest of the list
                    Point3d sc  = drawList->at(i).center();
                    Point3d tmp = sc - drawList->at(i).radius();
                    bb.min()    = vcl::min(bb.min(), tmp);
                    tmp         = sc + drawList->at(i).radius();
                    bb.max()    = vcl::max(bb.max(), tmp);
                }
            }
        }
        return bb;
    }
};

} // namespace vcl::qgl

#endif // VCL_EXT_QGLVIEWER_VIEWER_H
