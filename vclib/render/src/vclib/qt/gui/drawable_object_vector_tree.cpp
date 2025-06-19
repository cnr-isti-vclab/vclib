#include <vclib/qt/gui/drawable_object_vector_tree.h>

#include "ui_drawable_object_vector_tree.h"

namespace vcl::qt {

DrawableObjectVectorTree::DrawableObjectVectorTree(QWidget* parent) :
        QFrame(parent), mUI(new Ui::DrawableObjectVectorTree)
{
    mUI->setupUi(this);
}

DrawableObjectVectorTree::~DrawableObjectVectorTree()
{
    delete mUI;
}

} // namespace vcl::qt
