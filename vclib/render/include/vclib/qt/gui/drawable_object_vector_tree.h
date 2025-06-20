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
    explicit DrawableObjectVectorTree(
        const std::shared_ptr<DrawableObjectVector>& v,
        QWidget*                                     parent = nullptr);
    ~DrawableObjectVectorTree();

    void setIconFunction(const IconFunction& f);

    void setDrawableObjectVector(
        const std::shared_ptr<vcl::DrawableObjectVector>& v);

    uint selectedDrawableObject() const;

    // function to set the selected item
    bool setSelectedItem(uint i);

signals:
    void drawableObjectSelectionChanged(uint i);

private:
    void updateDrawableVectorTree();

private slots:
    void itemSelectionChanged();
};

} // namespace vcl::qt

#endif // DRAWABLE_OBJECT_VECTOR_TREE_H
