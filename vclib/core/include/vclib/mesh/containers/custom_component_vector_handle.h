/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_MESH_CONTAINERS_CUSTOM_COMPONENT_VECTOR_HANDLE_H
#define VCL_MESH_CONTAINERS_CUSTOM_COMPONENT_VECTOR_HANDLE_H

#include <vclib/types.h>

#include <any>
#include <vector>

namespace vcl {

/**
 * @brief Allows to access directly to a custom component.
 *
 * The class allows to access a custom component stored in a Contaner of
 * Elements without having to use the Container itself and avoiding copies, and
 * it can be used as a normal std::vector. The class stores references to the
 * custom components, therefore it allows to modify them.
 *
 * It is meant to be created by a Container, that constructs it from the vector
 * of custom components and then returns it to the user.
 *
 * @note A CustomComponentVectorHandle object is meant to be used to access the
 * custom components. It does not make sense to modify the size of the container
 * or to add or remove elements. Member functions that modify the size of the
 * container are not implemented.
 *
 * @note If the Element Container is modified after the creation of a
 * CustomComponentVectorHandle, the CustomComponentVectorHandle is not updated
 * and still contains the old references to the custom components (that may be
 * invalidated).
 *
 * @tparam T: The type of the custom component.
 */
template<typename T>
class CustomComponentVectorHandle
{
    std::vector<std::reference_wrapper<T>> mVec;

public:
    class Iterator : public std::vector<std::reference_wrapper<T>>::iterator
    {
        using Base = std::vector<std::reference_wrapper<T>>::iterator;

    public:
        using value_type = T;
        using reference  = value_type&;
        using pointer    = value_type*;

        Iterator(Base it) : Base(it) {}

        reference operator*() const { return Base::operator*().get(); }

        pointer operator->() const { return &(Base::operator*().get()); }
    };

    class ConstIterator :
            public std::vector<std::reference_wrapper<T>>::const_iterator
    {
        using Base = std::vector<std::reference_wrapper<T>>::const_iterator;

    public:
        using value_type = T;
        using reference  = const value_type&;
        using pointer    = const value_type*;

        ConstIterator(Base it) : Base(it) {}

        reference operator*() const { return Base::operator*().get(); }

        pointer operator->() const { return &(Base::operator*().get()); }
    };

    CustomComponentVectorHandle() {}

    CustomComponentVectorHandle(std::vector<std::any>& cc)
    {
        mVec.reserve(cc.size());
        for (uint i = 0; i < cc.size(); ++i) {
            mVec.push_back(std::any_cast<T&>(cc[i]));
        }
    }

    T& at(uint i) { return mVec[i].get(); }

    const T& at(uint i) const { return mVec[i].get(); }

    T& front() { return mVec.begin()->get(); }

    const T& front() const { return mVec.begin()->get(); }

    T& back() { return std::prev(mVec.end())->get(); }

    const T& back() const { return std::prev(mVec.end())->get(); }

    uint size() const { return mVec.size(); }

    T& operator[](uint i) { return mVec[i].get(); }

    const T& operator[](uint i) const { return mVec[i].get(); }

    Iterator begin() { return Iterator(mVec.begin()); }

    Iterator end() { return Iterator(mVec.end()); }

    ConstIterator begin() const { return ConstIterator(mVec.begin()); }

    ConstIterator end() const { return ConstIterator(mVec.end()); }
};

template<typename T>
using ConstCustomComponentVectorHandle = CustomComponentVectorHandle<const T>;

} // namespace vcl

#endif // VCL_MESH_CONTAINERS_CUSTOM_COMPONENT_VECTOR_HANDLE_H
