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
#include <vclib/space/core/vector/pointer_vector.h>

namespace vcl {

class DrawableObjectVector :
        public PointerVector<std::shared_ptr<DrawableObject>>,
        public DrawableObject
{
    using Base = PointerVector<std::shared_ptr<DrawableObject>>;

    bool mVisible = true;

    uint mSelectedObjectId = 0;

public:
    DrawableObjectVector() = default;

    using Base::pushBack;
    using Base::insert;
    using Base::set;

    template<typename U>
    requires std::derived_from<std::remove_cvref_t<U>, DrawableObject>
    void pushBack(U&& v)
    {
        using ObjType = std::remove_cvref_t<U>;
        Base::pushBack(std::make_shared<ObjType>(std::forward<U>(v)));
    }
    
    template<typename U>
    requires std::derived_from<std::remove_cvref_t<U>, DrawableObject>
    void insert(uint i, U&& v)
    {
        using ObjType = std::remove_cvref_t<U>;
        Base::insert(i, std::make_shared<ObjType>(std::forward<U>(v)));
    }
    
    template<typename U>
    requires std::derived_from<std::remove_cvref_t<U>, DrawableObject>
    void set(uint i, U&& e)
    {
        using ObjType = std::remove_cvref_t<U>;
        Base::at(i) = std::make_shared<ObjType>(std::forward<U>(e));
    }
    
    template<typename U>
    requires std::derived_from<std::remove_cvref_t<U>, DrawableObject>
    void set(Base::ConstIterator it, U&& e)
    {
        using ObjType = std::remove_cvref_t<U>;
        auto constBegin =
            static_cast<const DrawableObjectVector*>(this)->begin();
        Base::at(it - constBegin) =
            std::make_shared<ObjType>(std::forward<U>(e));
    }

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

    bool isVisible() const;

    void setVisibility(bool vis);

private:
    uint firstVisibleObject() const;
};

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_DRAWABLE_OBJECT_VECTOR_H
