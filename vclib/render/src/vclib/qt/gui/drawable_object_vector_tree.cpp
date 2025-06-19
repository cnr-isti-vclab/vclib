#include <vclib/qt/gui/drawable_object_vector_tree.h>

#include <vclib/qt/gui/drawable_object_item.h>

#include "ui_drawable_object_vector_tree.h"

namespace vcl::qt {

DrawableObjectVectorTree::DrawableObjectVectorTree(QWidget* parent) :
        QFrame(parent), mUI(new Ui::DrawableObjectVectorTree)
{
    mUI->setupUi(this);
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
        mUI->treeWidget->addTopLevelItem(item);
        // connect(
        //     item,
        //     &DrawableObjectItem::drawableObjectVisibilityChanged,
        //     this,
        //     &DrawableObjectVectorTree::drawableObjectVisibilityChanged);
    }
}

} // namespace vcl::qt
