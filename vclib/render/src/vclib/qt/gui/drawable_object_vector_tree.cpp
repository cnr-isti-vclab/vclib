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

        // connect(
        //     item,
        //     &DrawableObjectItem::drawableObjectVisibilityChanged,
        //     this,
        //     &DrawableObjectVectorTree::drawableObjectVisibilityChanged);
    }
}

void DrawableObjectVectorTree::itemSelectionChanged()
{
    auto sItems = mUI->treeWidget->selectedItems();
    if (sItems.size() > 0) {
        // emit the index of the selected item
        emit drawableObjectSelectionChanged(
            mUI->treeWidget->indexOfTopLevelItem(sItems.first()));
    }
    else {
        // if no item is selected, select the first one
        if (mUI->treeWidget->topLevelItemCount() > 0) {
            mUI->treeWidget->topLevelItem(0)->setSelected(true);
        }
    }
}

} // namespace vcl::qt
