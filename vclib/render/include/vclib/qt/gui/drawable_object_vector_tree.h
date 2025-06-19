#ifndef DRAWABLE_OBJECT_VECTOR_TREE_H
#define DRAWABLE_OBJECT_VECTOR_TREE_H

#include <vclib/render/drawable/drawable_object_vector.h>

#include <QFrame>

namespace vcl::qt {

namespace Ui {
class DrawableObjectVectorTree;
} // namespace Ui

class DrawableObjectVectorTree : public QFrame
{
public:
    using IconFunction =
        std::function<std::pair<QIcon, std::string>(const DrawableObject&)>;

private:
    Q_OBJECT

    Ui::DrawableObjectVectorTree* mUI;

    // this frame does not normally own this drawList
    std::shared_ptr<DrawableObjectVector> mDrawList;

    // icon function
    IconFunction mIconFunction = nullptr;

public:
    explicit DrawableObjectVectorTree(QWidget* parent = nullptr);
    ~DrawableObjectVectorTree();
};

} // namespace vcl::qt

#endif // DRAWABLE_OBJECT_VECTOR_TREE_H
