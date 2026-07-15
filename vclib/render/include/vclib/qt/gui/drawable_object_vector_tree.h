// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_DRAWABLE_OBJECT_VECTOR_TREE_H
#define VCL_QT_GUI_DRAWABLE_OBJECT_VECTOR_TREE_H

#include "drawable_object_item.h"

#include <vclib/render/drawable/drawable_object_vector.h>

#include <QFrame>
#include <QTreeWidgetItem>

namespace vcl::qt {

namespace Ui {
class DrawableObjectVectorTree;
} // namespace Ui

class DrawableObjectVectorTree : public QFrame
{
public:
    using IconFunction = DrawableObjectItem::IconFunction;

private:
    Q_OBJECT

    Ui::DrawableObjectVectorTree* mUI;

    // this frame does not normally own this drawList
    std::shared_ptr<DrawableObjectVector> mDrawList;

    // icon function
    IconFunction mIconFunction = nullptr;

    bool mCheckboxPressed = false;
    QTreeWidgetItem* mPressedItem = nullptr;

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

    void update(); // todo: rename this to refresh

    // function to set the selected item
    bool setSelectedItem(uint i);

signals:
    void drawableObjectSelectionChanged(uint i);
    void drawableObjectVisibilityChanged();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    void updateDrawableVectorTree();

private slots:
    void itemSelectionChanged();
    void itemCheckStateChanged(QTreeWidgetItem* item, int column);


};

} // namespace vcl::qt

#endif // VCL_QT_GUI_DRAWABLE_OBJECT_VECTOR_TREE_H
