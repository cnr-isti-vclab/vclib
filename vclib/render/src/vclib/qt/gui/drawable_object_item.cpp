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
    // clear any existing children
    auto l = takeChildren();
    for (auto* item : std::as_const(l)) {
        delete item;
    }

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

        auto transformMatrixItem = new QTreeWidgetItem(this);
        transformMatrixItem->setText(0, "Transform Matrix");
        makeItemNotSelectable(transformMatrixItem);

        // add 4 rows for the 4x4 matrix
        for (int i = 0; i < 4; ++i) {
            auto rowItem = new QTreeWidgetItem(transformMatrixItem);

            // use a monospace font for the matrix
            rowItem->setFont(1, QFont("Courier New", 10));

            rowItem->setText(0, "");
            QString rowLabel = "";
            for (int j = 0; j < 4; ++j) {
                rowLabel +=
                    QString::number(mesh->transformMatrix()(i, j), 'f', 3);
                if (j < 3)
                    rowLabel += "\t";
            }
            rowItem->setText(1, rowLabel);
            makeItemNotSelectable(rowItem);
        }

        if (mesh->textures().size() > 0) {
            auto texturesItem = new QTreeWidgetItem(this);
            texturesItem->setText(0, "Textures");
            makeItemNotSelectable(texturesItem);

            for (uint i = 0; const auto& texture : mesh->textures()) {
                auto textureItem = new QTreeWidgetItem(texturesItem);
                textureItem->setText(0, QString::number(i++));
                textureItem->setText(1, QString::fromStdString(texture));
                makeItemNotSelectable(textureItem);
            }
        }
    }
}

void DrawableObjectItem::makeItemNotSelectable(QTreeWidgetItem* item)
{
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
}

} // namespace vcl::qt
