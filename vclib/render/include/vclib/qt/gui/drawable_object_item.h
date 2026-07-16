// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_DRAWABLE_OBJECT_ITEM_H
#define VCL_QT_GUI_DRAWABLE_OBJECT_ITEM_H

#include <vclib/render/drawable/abstract_drawable_mesh.h>

#include <QTreeWidgetItem>

namespace vcl::qt {

class DrawableObjectItem : public QTreeWidgetItem
{
public:
    using IconFunction =
        std::function<std::pair<QIcon, std::string>(const DrawableObject&)>;

private:
    std::shared_ptr<DrawableObject> mObj;
    IconFunction                    mIconFunction;

public:
    explicit DrawableObjectItem(
        const std::shared_ptr<DrawableObject>& obj,
        IconFunction                           iconFunction,
        QTreeWidget*                           parent = nullptr);

    std::shared_ptr<DrawableObject> drawableObject() const;

    void updateMeshInfo();

private:
    void addMeshItem();

    void addMeshInfoItem(const AbstractDrawableMesh& mesh);
    void addTransformMatrixItem(const AbstractDrawableMesh& mesh);
    void addMaterialsItem(const AbstractDrawableMesh& mesh);
    void addMaterialData(
        const AbstractDrawableMesh& mesh,
        const Material&             material,
        QTreeWidgetItem*            parent);

    static void makeItemNotSelectable(QTreeWidgetItem* item);
    static void updateElementCountItem(QTreeWidgetItem* item, uint count, uint selectedCount);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_DRAWABLE_OBJECT_ITEM_H
