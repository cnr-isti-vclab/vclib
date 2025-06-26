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

#ifndef VCL_QT_GUI_DRAWABLE_OBJECT_ITEM_H
#define VCL_QT_GUI_DRAWABLE_OBJECT_ITEM_H

#include <vclib/render/drawable/drawable_object.h>

#include <QTreeWidgetItem>

namespace vcl::qt {

class DrawableObjectItem : public QTreeWidgetItem
{
    std::shared_ptr<DrawableObject> mObj;

public:
    explicit DrawableObjectItem(
        const std::shared_ptr<DrawableObject>& obj,
        QTreeWidget*                           parent = nullptr);

    std::shared_ptr<DrawableObject> drawableObject() const;

private:
    void addMeshInfoItem();

    static void makeItemNotSelectable(QTreeWidgetItem* item);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_DRAWABLE_OBJECT_ITEM_H
