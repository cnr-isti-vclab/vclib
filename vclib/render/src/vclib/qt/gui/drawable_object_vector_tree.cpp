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

#include <vclib/qt/gui/drawable_object_vector_tree.h>

#include <vclib/qt/gui/drawable_object_item.h>

#include "ui_drawable_object_vector_tree.h"

namespace vcl::qt {

DrawableObjectVectorTree::DrawableObjectVectorTree(QWidget* parent) :
        QFrame(parent), mUI(new Ui::DrawableObjectVectorTree)
{
    mUI->setupUi(this);

    // each time the user selects an item, call the itemSelectionChanged slot
    connect(
        mUI->treeWidget,
        &QTreeWidget::itemSelectionChanged,
        this,
        &DrawableObjectVectorTree::itemSelectionChanged);

    // each time that the user checks or unchecks an item, call the
    // itemCheckStateChanged slot
    connect(
        mUI->treeWidget,
        &QTreeWidget::itemChanged,
        this,
        &DrawableObjectVectorTree::itemCheckStateChanged);
}

DrawableObjectVectorTree::DrawableObjectVectorTree(
    const std::shared_ptr<DrawableObjectVector>& v,
    QWidget*                                     parent)
{
    setDrawableObjectVector(v);
}

DrawableObjectVectorTree::~DrawableObjectVectorTree()
{
    delete mUI;
}

void DrawableObjectVectorTree::setIconFunction(const IconFunction& f)
{
    mIconFunction = f;
    updateDrawableVectorTree();
}

uint DrawableObjectVectorTree::selectedDrawableObject() const
{
    auto sItems = mUI->treeWidget->selectedItems();
    if (sItems.size() > 0) {
        // emit the index of the selected item
        return mUI->treeWidget->indexOfTopLevelItem(sItems.first());
    }
    return UINT_NULL;
}

void DrawableObjectVectorTree::update()
{
    uint selected = selectedDrawableObject();
    updateDrawableVectorTree();
    if (selected == UINT_NULL && mDrawList->size() > 0) {
        selected = 0;
    }

    if (selected != UINT_NULL && selected < mDrawList->size()) {
        auto item = mUI->treeWidget->topLevelItem(selected);
        if (item) {
            mUI->treeWidget->setCurrentItem(item);
        }
    }
}

bool DrawableObjectVectorTree::setSelectedItem(uint i)
{
    if (i >= mDrawList->size()) {
        return false;
    }

    // get the ith top-level item
    auto item = mUI->treeWidget->topLevelItem(i);
    if (!item) {
        return false;
    }
    mUI->treeWidget->setCurrentItem(item);
    emit drawableObjectSelectionChanged(i);
    return true;
}

void DrawableObjectVectorTree::setDrawableObjectVector(
    const std::shared_ptr<DrawableObjectVector>& v)
{
    mDrawList = v;
    updateDrawableVectorTree();
    if (mDrawList->size() > 0) {
        mUI->treeWidget->topLevelItem(0)->setSelected(true);
    }
}

void DrawableObjectVectorTree::updateDrawableVectorTree()
{
    mUI->treeWidget->clear();

    for (auto& d : *mDrawList) {
        DrawableObjectItem* item = new DrawableObjectItem(d, mUI->treeWidget);

        // TODO: add icon. was:
        // if (mIconFunction) {
        //     std::pair<QIcon, std::string> p = mIconFunction(*d);

        //     frame->setIcon(p.first, QString::fromStdString(p.second));
        // }

        mUI->treeWidget->addTopLevelItem(item);

        // if d is visible, set the check state to checked
        item->setCheckState(0, d->isVisible() ? Qt::Checked : Qt::Unchecked);
    }
}

void DrawableObjectVectorTree::itemSelectionChanged()
{
    auto sItems = mUI->treeWidget->selectedItems();
    if (sItems.size() > 0) {
        // emit the index of the selected item
        int selectedIndex = mUI->treeWidget->indexOfTopLevelItem(sItems.first());
        if (selectedIndex >= 0) {
            emit drawableObjectSelectionChanged(selectedIndex);
        }
    }
    else {
        // if no item is selected, select the first one
        if (mUI->treeWidget->topLevelItemCount() > 0) {
            mUI->treeWidget->topLevelItem(0)->setSelected(true);
        }
    }
}

void DrawableObjectVectorTree::itemCheckStateChanged(
    QTreeWidgetItem* item,
    int              column)
{
    if (item) {
        // update the visibility of the drawable object
        auto drawableItem = dynamic_cast<DrawableObjectItem*>(item);
        if (drawableItem) {
            auto obj = drawableItem->drawableObject();
            if (obj) {
                obj->setVisibility(item->checkState(column) == Qt::Checked);
            }
        }

        // emit the visibility changed signal
        emit drawableObjectVisibilityChanged();
    }
}

} // namespace vcl::qt
