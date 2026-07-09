// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_DRAWABLE_DRAWABLE_OBJECT_VECTOR_H
#define VCL_RENDER_DRAWABLE_DRAWABLE_OBJECT_VECTOR_H

#include "drawable_object.h"

#include <vclib/space/core/box.h>
#include <vclib/space/core/vector/polymorphic_object_vector.h>

namespace vcl {

class DrawableObjectVector :
        public PolymorphicObjectVector<DrawableObject>,
        public DrawableObject
{
    using Base = PolymorphicObjectVector<DrawableObject>;

    bool mVisible = true;

    uint mSelectedObjectId = 0;

public:
    DrawableObjectVector() = default;

    uint selectedObjectId() const { return mSelectedObjectId; }

    void setSelectedObjectId(uint id)
    {
        if (id < Base::size())
            mSelectedObjectId = id;
    }

    // DrawableObject interface
    void init();

    void draw(const DrawObjectSettings& settings);

    void drawId(const DrawObjectSettings& settings);

    Box3d boundingBox() const;

    Point3d center() const;

    std::shared_ptr<DrawableObject> clone() const&;

    std::shared_ptr<DrawableObject> clone() &&;

    bool isVisible() const;

    void setVisibility(bool vis);

private:
    uint firstVisibleObject() const;
};

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_DRAWABLE_OBJECT_VECTOR_H
