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

/**
 * @brief A container that stores a collection of polymorphic drawable objects.
 *
 * This class inherits from PointerVector, storing std::shared_ptr to
 * DrawableObject instances. It provides templated convenience methods to add
 * concrete derived classes directly by value/reference, automatically wrapping 
 * them in a shared pointer to take ownership of a copy of the object.
 *
 * @ingroup render_drawable
 */
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

    /**
     * @brief Adds a copy of the given derived drawable object to the end of the vector.
     * 
     * Constructs a new shared pointer containing a copy (or moved instance) of `v`.
     * 
     * @tparam U The derived type of the drawable object.
     * @param v The object to add.
     */
    template<typename U>
    requires std::derived_from<std::remove_cvref_t<U>, DrawableObject> &&
             (!std::is_abstract_v<std::remove_cvref_t<U>>)
    void pushBack(U&& v)
    {
        using ObjType = std::remove_cvref_t<U>;
        // Create a shared_ptr of the concrete derived type to avoid object slicing,
        // and push it to the base PointerVector.
        Base::pushBack(std::make_shared<ObjType>(std::forward<U>(v)));
    }
    
    /**
     * @brief Inserts a copy of the given derived drawable object at the specified position.
     * 
     * @tparam U The derived type of the drawable object.
     * @param i The index at which to insert.
     * @param v The object to insert.
     */
    template<typename U>
    requires std::derived_from<std::remove_cvref_t<U>, DrawableObject> &&
             (!std::is_abstract_v<std::remove_cvref_t<U>>)
    void insert(uint i, U&& v)
    {
        using ObjType = std::remove_cvref_t<U>;
        Base::insert(i, std::make_shared<ObjType>(std::forward<U>(v)));
    }
    
    /**
     * @brief Replaces the element at the specified position with a copy of the given object.
     * 
     * @tparam U The derived type of the drawable object.
     * @param i The index of the element to replace.
     * @param e The new object.
     */
    template<typename U>
    requires std::derived_from<std::remove_cvref_t<U>, DrawableObject> &&
             (!std::is_abstract_v<std::remove_cvref_t<U>>)
    void set(uint i, U&& e)
    {
        using ObjType = std::remove_cvref_t<U>;
        Base::at(i) = std::make_shared<ObjType>(std::forward<U>(e));
    }
    
    /**
     * @brief Replaces the element at the specified iterator position with a copy of the given object.
     * 
     * @tparam U The derived type of the drawable object.
     * @param it The iterator pointing to the element to replace.
     * @param e The new object.
     */
    template<typename U>
    requires std::derived_from<std::remove_cvref_t<U>, DrawableObject> &&
             (!std::is_abstract_v<std::remove_cvref_t<U>>)
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
