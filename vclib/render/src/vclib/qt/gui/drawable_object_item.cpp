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

#include <vclib/qt/gui/drawable_object_item.h>

#include <vclib/render/drawable/abstract_drawable_mesh.h>

namespace vcl::qt {

DrawableObjectItem::DrawableObjectItem(
    const std::shared_ptr<DrawableObject>& obj,
    QTreeWidget* parent) : QTreeWidgetItem(parent), mObj(obj)
{
    assert(obj);
    if (obj->isVisible())
        setCheckState(0, Qt::Checked);
    else
        setCheckState(0, Qt::Unchecked);

    // leave setText and setFlags AFTER setCheckState.
    // for some reason, they trigger the itemCheckStateChanged signal...
    setFlags(
        Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
    setText(0, QString::fromStdString(obj->name()));

    // add mesh info item
    addMeshInfoItem();
}

std::shared_ptr<DrawableObject> DrawableObjectItem::drawableObject() const
{
    return mObj;
}

void DrawableObjectItem::addMeshInfoItem()
{
    // if the DrawbaleObject is an AbstractDrawableMesh
    auto mesh = std::dynamic_pointer_cast<AbstractDrawableMesh>(mObj);
    if (mesh) {
        // make this item expandable, and add a child called "Mesh Info"
        auto meshInfoItem = new QTreeWidgetItem(this);
        meshInfoItem->setText(0, "Mesh Info");
        makeItemNotSelectable(meshInfoItem);

        // vertex number item
        auto vertexNumberItem = new QTreeWidgetItem(meshInfoItem);
        vertexNumberItem->setText(0, "# Vertices");
        vertexNumberItem->setText(1, QString::number(mesh->vertexNumber()));
        makeItemNotSelectable(vertexNumberItem);

        if (mesh->faceNumber() > 0) {
            auto faceNumberItem = new QTreeWidgetItem(meshInfoItem);
            faceNumberItem->setText(0, "# Faces");
            faceNumberItem->setText(1, QString::number(mesh->faceNumber()));
            makeItemNotSelectable(faceNumberItem);
        }

        if (mesh->edgeNumber() > 0) {
            auto edgeNumberItem = new QTreeWidgetItem(meshInfoItem);
            edgeNumberItem->setText(0, "# Edges");
            edgeNumberItem->setText(1, QString::number(mesh->edgeNumber()));
            makeItemNotSelectable(edgeNumberItem);
        }
    }
}

void DrawableObjectItem::makeItemNotSelectable(QTreeWidgetItem* item)
{
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
}

} // namespace vcl::qt
