// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/drawable_object_vector_tree.h>

#include "ui_drawable_object_vector_tree.h"

#include <vclib/qt/gui/drawable_object_item.h>

#include <QApplication>
#include <QMouseEvent>

namespace vcl::qt {

DrawableObjectVectorTree::DrawableObjectVectorTree(QWidget* parent) :
        QFrame(parent), mUI(new Ui::DrawableObjectVectorTree)
{
    mUI->setupUi(this);

    mUI->treeWidget->header()->setStretchLastSection(false);
    mUI->treeWidget->header()->setSectionResizeMode(2, QHeaderView::Fixed);
    mUI->treeWidget->setColumnWidth(2, 28);
    mUI->treeWidget->header()->setSectionResizeMode(1, QHeaderView::Stretch);

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

    mUI->treeWidget->viewport()->installEventFilter(this);
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

bool DrawableObjectVectorTree::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == mUI->treeWidget->viewport()) {
        if (event->type() == QEvent::MouseButtonPress ||
            event->type() == QEvent::MouseButtonRelease ||
            event->type() == QEvent::MouseButtonDblClick ||
            event->type() == QEvent::MouseMove) {

            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QModelIndex  index = mUI->treeWidget->indexAt(mouseEvent->pos());

            if (event->type() == QEvent::MouseButtonPress ||
                event->type() == QEvent::MouseButtonDblClick) {
                if (index.isValid() && index.column() == 0) {
                    QRect visualRect = mUI->treeWidget->visualRect(index);
                    if (mouseEvent->pos().x() >= visualRect.left() &&
                        mouseEvent->pos().x() <= visualRect.left() + 26) {
                        mCheckboxPressed = true;
                        mPressedItem = mUI->treeWidget->itemFromIndex(index);
                        return true; // Consume press
                    }
                }
            }
            else if (event->type() == QEvent::MouseMove) {
                if (mCheckboxPressed) {
                    return true; // Consume drag so QTreeView doesn't start selection drag
                }
            }
            else if (event->type() == QEvent::MouseButtonRelease) {
                if (mCheckboxPressed) {
                    mCheckboxPressed = false;

                    // Only toggle if we released over the same item's checkbox area
                    if (index.isValid() && index.column() == 0) {
                        QTreeWidgetItem* item = mUI->treeWidget->itemFromIndex(index);
                        if (item && item == mPressedItem) {
                            QRect visualRect = mUI->treeWidget->visualRect(index);
                            if (mouseEvent->pos().x() >= visualRect.left() &&
                                mouseEvent->pos().x() <= visualRect.left() + 26) {

                                Qt::CheckState state = item->checkState(0);
                                item->setCheckState(
                                    0,
                                    state == Qt::Checked ? Qt::Unchecked
                                                           : Qt::Checked);
                            }
                        }
                    }
                    mPressedItem = nullptr;
                    return true; // Consume release
                }
            }
        }
    }
    return QFrame::eventFilter(obj, event);
}

void DrawableObjectVectorTree::updateDrawableVectorTree()
{
    mUI->treeWidget->clear();

    for (auto& d : *mDrawList) {
        DrawableObjectItem* item =
            new DrawableObjectItem(d, mIconFunction, mUI->treeWidget);

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
        int selectedIndex =
            mUI->treeWidget->indexOfTopLevelItem(sItems.first());
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
    if (item && column == 0) {
        bool isCtrlPressed =
            QApplication::keyboardModifiers() & Qt::ControlModifier;

        if (isCtrlPressed) {
            mUI->treeWidget->blockSignals(true);

            for (int i = 0; i < mUI->treeWidget->topLevelItemCount(); ++i) {
                auto childItem = mUI->treeWidget->topLevelItem(i);
                auto drawableItem =
                    dynamic_cast<DrawableObjectItem*>(childItem);
                if (drawableItem) {
                    bool visible = (childItem == item);
                    childItem->setCheckState(
                        0, visible ? Qt::Checked : Qt::Unchecked);
                    auto obj = drawableItem->drawableObject();
                    if (obj) {
                        obj->setVisibility(visible);
                    }
                }
            }

            mUI->treeWidget->blockSignals(false);
        }
        else {
            // update the visibility of the drawable object
            auto drawableItem = dynamic_cast<DrawableObjectItem*>(item);
            if (drawableItem) {
                auto obj = drawableItem->drawableObject();
                if (obj) {
                    obj->setVisibility(item->checkState(column) == Qt::Checked);
                }
            }
        }

        // emit the visibility changed signal
        emit drawableObjectVisibilityChanged();
    }
}

} // namespace vcl::qt
